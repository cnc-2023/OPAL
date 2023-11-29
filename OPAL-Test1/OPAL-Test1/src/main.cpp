#include "main.h"

XY2_100 *galvo;

// put function declarations here:
void setGalvoPosition(double x, double y)
{
  int tmp_x, tmp_y;
  // if (AXIS_INVERSE_X)
  //   tmp_x = map(x, 0.0, X_MAX_POS_MM, 65535, 0) + 0.5;
  // else
  tmp_x = map(x, 0.0, X_MAX_POS_MM, 0, 65535) + 0.5;

  // if (AXIS_INVERSE_Y)
  //   tmp_y = map(y, 0.0, Y_MAX_POS_MM, 65535, 0) + 0.5;
  // else
  tmp_y = map(y, 0.0, Y_MAX_POS_MM, 0, 65535) + 0.5;

  galvo->setPos(tmp_x, tmp_y);

  Serial.print(tmp_x);
  Serial.print(',');
  Serial.println(tmp_y);
}

int current_x = 0;
int current_y = 0;

// void tic()
// {
//   _NOW = micros();
//   if (_NOW > CURRENT_ENDNANOS)
//   {
//     current_x += 1;
//     if (current_x > X_MAX_POS_MM * 10)
//       current_x = 0;

//     current_y += 2;
//     if (current_y > Y_MAX_POS_MM * 10)
//       current_y = 0;
//     setGalvoPosition(current_x, current_y);
//     CURRENT_ENDNANOS = _NOW + CURRENT_DURATION;
//   }
// }
void setup()
{

  pinMode(GALVO_SSR_OUT_PIN, OUTPUT);
  digitalWrite(GALVO_SSR_OUT_PIN, 0);

  Serial.begin(115200);

  galvo = new XY2_100();
  galvo->begin(); // TODO:ADD define "Galvo has SSR" for galvo PSU

  printWelcome();
}

void loop()
{
  // 从串口读取坐标数据并设置振镜位置，格式为：x,y，如：100.2,100.3
  // 并将坐标数据回传给上位机
  if (Serial.available())
  {
    String str = Serial.readStringUntil('\n', 100U);
    int commaIndex = str.indexOf(',');
    if (commaIndex > 0)
    {
      String x_str = str.substring(0, commaIndex);
      String y_str = str.substring(commaIndex + 1);
      float x = x_str.toFloat();
      float y = y_str.toFloat();
      setGalvoPosition(x, y);

      // Serial.print(x);
      // Serial.print(',');
      // Serial.println(y);
    }
  }
}
