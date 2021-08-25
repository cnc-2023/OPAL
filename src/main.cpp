/*
  OPAL.ino - Main projectfile to run OPAL FW on PJRC Teensy 4.x board

  Part of OpenGalvo - OPAL Firmware

  Copyright (c) 2020-2021 Daniel Olsson

  OPAL Firmware is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OPAL Firmware is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OPAL Firmware.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <Arduino.h>
#include <HardwareSerial.h>
#include "Pins.h"
#include "configuration.h"
#include "Helpers.h"
#include "Mcodes.h"
#include "main.h"

#ifdef LASER_IS_SYNRAD

#include <Synrad48Ctrl.h>
#include <XY2_100.h>

Synrad48Ctrl laser;

#endif



XY2_100 galvo;

#include <CircularBuffer.h>
CircularBuffer<GCode, CMDBUFFERSIZE> commandBuffer;
CircularBuffer<GCode, MBUFFERSIZE> mBuffer;

#include "SerialCMDReader.h"
SerialCMDReader serialReciever;

uint64_t next_command_time = 0;

bool beginNext = true;
static uint64_t startNanos;
static uint64_t endNanos;

static uint64_t _now;

static coordinate lastMove;
coordinate to;

static double distx;
static double disty;
static double distz;

static double feedrate = DEFAULT_FEEDRATE;

GCode * oldPreviousMove;
GCode * previousMove;
GCode * currentMove;
GCode * nextMove;

coordinate target;
int lastLaserPWR = 0;
bool laserChanged = false;
int itcnt = 0;
static int interpolCnt = 0;

void setup() {
  lastMove.x = 0;
  lastMove.y = 0;
  lastMove.z = 0;

  to.x = 0;
  to.y = 0;
  to.z = 0;
  
  Serial.begin(115200);
  galvo.begin();
  serialReciever.begin(&commandBuffer);
}

void calculateMoveLengthNanos(double xdist, double ydist, double moveVelocity, double* result)  {  //Velocity is presumed to be in coordinates/s

  double lengthOfMove = sqrt( (0.0 + xdist)*(0.0 + xdist)  + (0.0 + ydist)*(0.0 + ydist) ); // calc hypo a^2+b^2=c^2
  //TODO: Verify unit conversion
  *result = ((lengthOfMove*1000*1000*1000/moveVelocity)); //s=v*t -> s/v =t   (movelength/moveVolocity) -> 2pos / (4pos/s) = 0.5s *1000 = 500ms
  return;
}

void processMcode(GCode* code)
{
  switch (code->code)
  {
  case 3: //M3
    /*    Code is Spindle CV Normally used for spindle on direction CV and 'Sxxxxx' to set RPM
        When using Laser laser has no rotaton so handles same as M4  */
        //Fallthrough intended
  case 4: //M4
  {
    int lpower = (*previousMove).s;
    if( lpower != MAX_VAL)
    {
      lastLaserPWR = lpower;
    }
    #ifdef LASER_IS_SYNRAD
    laser.setLaserPWM(lastLaserPWR); //Unknown code might cause unexpected behaviour Better turn off laser - SAFETY
    #else
    digitalWrite(LASER_PWM_OUT_PIN, lastLaserPWR);
    #endif    
    break;
  }
  case 5: //M5
    lastLaserPWR = 0;        
    #ifdef LASER_IS_SYNRAD
      laser.setLaserPWM(0); //Unknown code might cause unexpected behaviour Better turn off laser - SAFETY
    #else
      digitalWrite(LASER_PWM_OUT_PIN, 0);
    #endif
    break;

  case 17: //M17 - Turn all steppers ON -> Galvo & Stepper PSU Control (SSRs)
    digitalWrite(GALVO_SSR_OUT_PIN, 1);
    digitalWrite(STEPPER_SSR_OUT_PIN, 1); 
    break;
  case 18: //M18 - Turn all steppers OFF -> Galvo & Stepper PSU Control (SSRs)
    digitalWrite(GALVO_SSR_OUT_PIN, 0);
    digitalWrite(STEPPER_SSR_OUT_PIN, 0); 
    break;

case 80: //M80 - Laser PSU Control (SSR)
    // Implicit delay for SynradCtrl::laserInitTime milliseconds (5000ms)
    #ifdef LASER_IS_SYNRAD
    laser.begin(LASER_PWM_OUT_PIN, LASER_SSR_OUT_PIN);
    #else
    digitalWrite(LASER_SSR_OUT_PIN,1);
    #endif

    break;
  case 81: //M81 - Laser PSU Control (SSR)
    #ifdef LASER_IS_SYNRAD
    laser.stop();
    #else
    digitalWrite(LASER_SSR_OUT_PIN,0);
    #endif
    break;
  
  default:
    break;
  }
  //Serial.print("\nExecuting MCode: ");Serial.print(code->codeprefix);Serial.println(code->code);
  //Serial.print("-- Executing MCode: Not Implemented\n");
}

void process()  {
  _now = nanos();
  
  if(beginNext)  {
    while(!mBuffer.isEmpty())    {
      //processMCodes
      GCode* cmd = new GCode(mBuffer.shift());
        processMcode(cmd);
        delete cmd;
    }
    
    delete oldPreviousMove; 
    oldPreviousMove = previousMove;
    previousMove = currentMove;
    currentMove = nextMove;

    bool gcodeFound = false;
    while(!gcodeFound && !commandBuffer.isEmpty())
    {
      nextMove = new GCode((commandBuffer.pop()));
      if((*nextMove).codeprefix != 'G'){
        mBuffer.unshift(*nextMove);
      }
      else{
        gcodeFound = true;  
      }       
    }      
    if(!gcodeFound){
      nextMove = NULL;
    }

    // Buffer MGMT is done 

    //Allways update history...
    if(previousMove)  //SET UP PREVIOUS POSITION
    {
      if((*previousMove).x != MAX_VAL)
        lastMove.x = (*previousMove).x;
      if((*previousMove).y != MAX_VAL)
        lastMove.y = (*previousMove).y;
      if((*previousMove).z != MAX_VAL)
        lastMove.z = (*previousMove).z;
    }
    
    if(currentMove)  { 
      if((*currentMove).x != MAX_VAL)
        to.x = (*currentMove).x;
      if((*currentMove).y != MAX_VAL)
        to.y = (*currentMove).y;
      if((*currentMove).z != MAX_VAL)
        to.z = (*currentMove).z;
      if((*currentMove).f != MAX_VAL)
        feedrate = (*currentMove).f;
        
      // Set Laser Power
      if((*currentMove).s != MAX_VAL)
      {
        lastLaserPWR = (*currentMove).s;
        laserChanged = true;
      }

      if((*currentMove).code == 1) 
      {   // G1
        #ifdef LASER_G0_OFF_G1_ON
          #ifdef LASER_IS_SYNRAD
          laser.setLaserPWM(lastLaserPWR); //Unknown code might cause unexpected behaviour Better turn off laser - SAFETY
          #endif
        #endif
        startNanos = _now;      
        distx = to.x-lastMove.x;
        disty = to.y-lastMove.y;
        distz = to.z-lastMove.z; //TODO: Add implementation
        calculateMoveLengthNanos(distx, disty, feedrate, &((*currentMove).moveLengthNanos));
        endNanos = startNanos + (*currentMove).moveLengthNanos;
      }
      else if((*currentMove).code == 0)
      {   //G0 means no calculation and no interpolation as feedrate is ignored
        #ifdef LASER_G0_OFF_G1_ON
            #ifdef LASER_IS_SYNRAD
             laser.setLaserPWM(0); //Unknown code might cause unexpected behaviour Better turn off laser - SAFETY
            #endif
        #endif
      }
      else
      {   //
        Serial.println("Unknown GCode! code: ");
        Serial.println((*currentMove).codeprefix);
        Serial.println((*currentMove).code);
        #ifdef LASER_IS_SYNRAD
          laser.setLaserPWM(0); //Unknown code might cause unexpected behaviour Better turn off laser - SAFETY
        #endif
      }
      beginNext = false;
    }
  }
  
  //interpolate  
  if(currentMove && (_now > endNanos || (*currentMove).code == 0))  //We are out of time or G0
  {
    #ifdef LASER_IS_SYNRAD
      laser.handleLaser();    
    #endif
    galvo.goTo(map(to.x, 0.0,250.0, 65535,0)+0.5, map(to.y, 0.0,250.0, 0,65535)+0.5); //Make sure to hit the commanded position
    beginNext = true;
    interpolCnt=0;
    return;
  }
  else if (currentMove)
  {


    distx = to.x-lastMove.x;
    disty = to.y-lastMove.y;
    distz = to.z-lastMove.z; // not used.....
    uint64_t t = (*currentMove).moveLengthNanos; 
    double fraction_of_move = (double)(_now-startNanos)/t;
    double x = (lastMove.x + (distx*fraction_of_move));
    double y = (lastMove.y + (disty*fraction_of_move));
    interpolCnt++;
    #ifdef LASER_IS_SYNRAD
      laser.handleLaser();    
    #endif
    galvo.goTo( map(x, 0.0,250.0, 65535,0)+0.5, map(y, 0.0,250.0, 0,65535)+0.5 );
    return ;
  }
  else 
  {
    //Serial.println("Idle");
  }
  return;
}

void loop() {  

    serialReciever.handleSerial();
    process();
    


}


