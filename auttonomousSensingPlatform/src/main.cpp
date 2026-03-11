#include <Arduino.h>
#include <Adafruit_BME280.h>

/* Environmental and Power Management Logic */

// The BME280 sensor instance is initialized
Adafruit_BME280 bme; 

// System constants are defined
#define BATTERY_CAP_MAH 2000.0
#define ACTIVE_CURRENT_MA 80.0
#define SLEEP_INTERVAL_MS 10000

// Cumulative power consumption is tracked
float totalConsumed_mAs = 0;

/**
 * The sensor task is executed as a separate FreeRTOS thread.
 * Environmental data is sampled and autonomy is estimated.+
 */
void vSensorTask(void *pvParameters) {
    for(;;) {
        // The start time of the sampling cycle is recorded
        unsigned long startTime = millis();
        
        // Temperature and pressure are read from the I2C bus
        float temp = bme.readTemperature();
        float pres = bme.readPressure() / 100.0F;

        // The duration of active processing is calculated
        unsigned long activeTime = millis() - startTime;
        
        // Power consumption is updated based on active and idle states
        totalConsumed_mAs += (activeTime / 1000.0) * ACTIVE_CURRENT_MA;
        totalConsumed_mAs += (SLEEP_INTERVAL_MS / 1000.0) * 0.05; // 50uA sleep estimate

        // Remaining battery capacity and system durability are estimated
        float remaining_mAh = BATTERY_CAP_MAH - (totalConsumed_mAs / 3600.0);
        float days_left = (remaining_mAh / (ACTIVE_CURRENT_MA / 24.0));

        // System telemetry is outputted to the serial interface
        Serial.printf("--- SENSOR DATA ---\n");
        Serial.printf("Ambient Temp: %.2f C\n", temp);
        Serial.printf("Air Pressure: %.2f hPa\n", pres);
        Serial.printf("Est. Autonomy: %.1f Days\n", days_left);
        Serial.printf("-------------------\n");

        // The task is suspended for the defined interval
        vTaskDelay(pdMS_TO_TICKS(SLEEP_INTERVAL_MS));
    }
}

void setup() {
    // The serial communication is initialized at 115200 baud
    Serial.begin(115200);
    
    // The I2C interface for the BME280 is verified
    if (!bme.begin(0x76)) {
        Serial.println("Sensor initialization failed.");
    }

    // The sensing task is created within the FreeRTOS scheduler
    xTaskCreate(vSensorTask, "Sensor_Logic", 4096, NULL, 1, NULL);
}

void loop() {
    // The main loop is left empty as task management is handled by FreeRTOS
}