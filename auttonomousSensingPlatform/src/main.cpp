#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BME280.h>

// The BME280 instance is initialized
Adafruit_BME280 bme; 

/**
 * The system is initialized.
 * The I2C bus is scanned for the BME280 at both possible addresses.
 */
void setup() {
    Serial.begin(115200);
    Wire.begin(21, 22);
    Wire.setClock(100000); // Forces the bus to 100kHz (Slow and Stable)
    delay(2000); // The system is given time to stabilize
    Serial.println("--- BME280 Diagnostic Start ---");

    // The I2C bus is initialized
    Wire.begin(21, 22); // SDA on 21, SCL on 22

    // Attempt 1: Address 0x76
    if (bme.begin(0x76)) {
        Serial.println("Sensor found at 0x76");
    } 
    // Attempt 2: Address 0x77
    else if (bme.begin(0x77)) {
        Serial.println("Sensor found at 0x77");
    } 
    else {
        Serial.println("Error: Sensor not found at 0x76 or 0x77.");
        Serial.println("Check SDA/SCL wiring and power.");
        while (1); // The execution is halted if no sensor is detected
    }
}

/**
 * The sensing loop is executed.
 * Raw data is extracted and outputted via the UART interface.
 */
void loop() {
    // Atmospheric data is retrieved
    float temperature = bme.readTemperature();
    float pressure = bme.readPressure() / 100.0F;
    float humidity = bme.readHumidity();

    // Data is outputted for Serial Monitor and Logic Analyzer
    Serial.print("T:");
    Serial.print(temperature);
    Serial.print(" P:");
    Serial.print(pressure);
    Serial.print(" H:");
    Serial.println(humidity);

    // A 2-second delay is implemented between samples
    delay(2000);
}