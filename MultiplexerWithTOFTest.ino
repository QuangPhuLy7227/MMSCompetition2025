#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include "multiplexer_with_tof.h"

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Wire.begin();

  Serial.println("Initializing TCA9548A...");
  initSensors(); 
}

void loop() {
  readSensorsDistance();
}
