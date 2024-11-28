#include "VL53L0XTest.h"

// Create an instance of the VL53L0X sensor
Adafruit_VL53L0X lox;

void initializeSensor() {
    Serial.begin(115200);

    // Wait for Serial Monitor to open on USB-native devices
    while (!Serial) {
        delay(1);
    }

    Serial.println("Adafruit VL53L0X test");

    if (!lox.begin()) {
        Serial.println(F("Failed to boot VL53L0X"));
        while (true) {
            delay(1);
        }
    }

    Serial.println(F("VL53L0X API Simple Ranging example\n\n"));
}

void performMeasurement() {
    VL53L0X_RangingMeasurementData_t measure;

    Serial.print("Reading a measurement... ");
    lox.rangingTest(&measure, false); // Set to 'true' for debug output

    if (measure.RangeStatus != 4) { // Check for valid measurement
        Serial.print("Distance (mm): ");
        Serial.println(measure.RangeMilliMeter);
    } else {
        Serial.println(" out of range ");
    }
}
