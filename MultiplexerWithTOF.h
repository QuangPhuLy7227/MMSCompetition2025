#ifndef MULTIPLEXER_WITH_TOF_H
#define MULTIPLEXER_WITH_TOF_H

#include <Wire.h>
#include <Adafruit_VL53L0X.h>

// TCA9548A I2C Address (default is 0x70)
#define TCA9548A_ADDRESS 0x70

// create VL53L0X sensor objects
extern Adafruit_VL53L0X sensor1;
extern Adafruit_VL53L0X sensor2;

void selectChannel(uint8_t channel);
void initSensors();
void readSensorsDistance();

#endif 
