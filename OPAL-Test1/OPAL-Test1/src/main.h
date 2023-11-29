#include <Arduino.h>

#include <XY2_100.h>

#include <Pins.h>
#include <Helpers.h>

#define X_MAX_POS_MM 250
#define Y_MAX_POS_MM 250

uint64_t _NOW = 0;
double CURRENT_DURATION = 1000;
uint64_t CURRENT_ENDNANOS = 0;
