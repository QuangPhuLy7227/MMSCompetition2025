#include "multiplexer_with_tof.h"

Adafruit_VL53L0X sensor1 = Adafruit_VL53L0X();
Adafruit_VL53L0X sensor2 = Adafruit_VL53L0X();

// Select I2C channel on the multiplexer
void selectChannel(uint8_t channel) {
  if (channel > 7) return; // Ensure channel is valid
  Wire.beginTransmission(TCA9548A_ADDRESS);
  Wire.write(1 << channel); // Select the channel (1 shifted by 'channel')
  Wire.endTransmission();
}

void initSensors() {
  Serial.println("Initializing sensors...");

  selectChannel(0); // Channel 0 - Sensor 1
  if (!sensor1.begin()) {
    Serial.println("Failed to initialize sensor 1 on channel 0");
    while (1);
  }

  selectChannel(1); // Channel 1 - Sensor 2
  if (!sensor2.begin()) {
    Serial.println("Failed to initialize sensor 2 on channel 1");
    while (1);
  }

  Serial.println("Initialization complete.");
}

void readSensorsDistance() {
  VL53L0X_RangingMeasurementData_t measure;

  // Read sensor 1
  selectChannel(0); // Switch to channel 0 for sensor 1
  sensor1.rangingTest(&measure, false);
  if (measure.RangeStatus != 4) {
    Serial.print("Sensor 1 Distance: ");
    Serial.print(measure.RangeMilliMeter);
    Serial.println(" mm");
  } else {
    Serial.println("Sensor 1 out of range");
  }
  delay(100);

  // Read sensor 2
  selectChannel(1); // Switch to channel 1 for sensor 2
  sensor2.rangingTest(&measure, false);
  if (measure.RangeStatus != 4) {
    Serial.print("Sensor 2 Distance: ");
    Serial.print(measure.RangeMilliMeter);
    Serial.println(" mm");
  } else {
    Serial.println("Sensor 2 out of range");
  }
  delay(100);
}
