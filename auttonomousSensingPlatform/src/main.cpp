// Application of autonomy calculation, 
// In connection with the LLC and with Uno board
// Author: Dmytro Maslov, Porto, Portugal
// Hardware: esp32 devkitc-v1, bme-280, tsx0108e between Uno and Esp32
#include <Arduino.h>
#include <HardwareSerial.h>
#include <Adafruit_BME280.h>

// UART2 is utilized for the 3.3V/5V bridge to the Arduino
HardwareSerial ArduinoNode(2); 
Adafruit_BME280 bme;

// System states for the autonomous manager
enum SystemState { STATE_IDLE, STATE_ALERT };
SystemState currentState = STATE_IDLE;

// Power management variables for autonomy to validate the remaining time
const float BATTERY_CAP_MAH = 2000.0; // arbitrary value
float totalConsumed_mAs = 0;

void setup() {
    Serial.begin(115200);
    
    // I2C communication is initialized on GPIO 21 (SDA) and GPIO 22 (SCL)
    Wire.begin(21, 22);
    Wire.setClock(100000); // 100kHz is enforced for signal integrity

    // UART2 is configured at 9600 baud for LLC interface (RX=16, TX=17)
    ArduinoNode.begin(9600, SERIAL_8N1, 16, 17); // on the used board pins are RX2 and TX2, accordingly
    
    // The BME280 sensor is initialized at address 0x76
    if (!bme.begin(0x76)) {
        Serial.println("SYSTEM_ERROR: BME280 initialization failed.");
    }
    
    Serial.println("SYSTEM_BOOT: Autonomous Gateway Online.");
}

void loop() {
    // Incoming UART telemetry is monitored
    if (ArduinoNode.available()) {
        String msg = ArduinoNode.readStringUntil('\n');
        msg.trim();

        if (msg.startsWith("DIST:")) {
            int distance = msg.substring(5).toInt();
            Serial.printf("[SENSOR_NODE] Acoustic Range: %d cm\n", distance);
            
            // The state is transitioned if proximity thresholds are violated
            // This siumlates the eternal physical trigger of a target proximity, 
            // where target might be a whale, ship or other platform
            if (distance > 0 && distance < 30) currentState = STATE_ALERT;
        }
    }

    // State machine execution
    if (currentState == STATE_ALERT) {
        Serial.println("[ALARM] Proximity violation detected.");
        
        // Environmental context is captured during the alert event
        // Insted of temperature reading, other physical reading might be implemented, 
        // such as light condition, humidity, time, etc
        float temp = bme.readTemperature();
        Serial.printf("[LOG] Event Temperature: %.2f C\n", temp);

        // System returns to IDLE after logging
        currentState = STATE_IDLE;
    }

    // Autonomy calculation is updated (Simulated current draw tracking)
    totalConsumed_mAs += (10.0 / 1000.0) * 80.0; // Estimate per loop iteration
    float days_left = (BATTERY_CAP_MAH - (totalConsumed_mAs / 3600.0)) / (80.0 / 24.0);
    
    // Periodic system heartbeat
    static uint32_t lastPrint = 0;
    if (millis() - lastPrint > 5000) {
        Serial.printf("[STATUS] Autonomy: %.1f Days | System: OK\n", days_left);
        lastPrint = millis();
    }
}