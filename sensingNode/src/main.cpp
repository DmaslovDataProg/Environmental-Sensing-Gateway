// Uno reads the ultrasound sensor and sends the readings as strings by TX and RX to LLC
// Author: Dmytro Maslov, Porto, Portugal
// Hardware: Arduino Uno R3, HC-SR04, tsx0108e between Uno and Esp32
#include <Arduino.h>

/* Sensor Node Hardware Configuration */
const int trigPin = 12;
const int echoPin = 11;

// Timing constants for the 3-second sampling interval
unsigned long previousMillis = 0;
const long interval = 3000; 

/**
 * The hardware and communication interfaces are initialized.
 */
void setup() {
    // The UART interface is initialized at 9600 baud for the LLC link
    Serial.begin(9600);
    
    // The ultrasonic sensor pins are configured
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    
    Serial.println("INFO: Acoustic Sensor Node initialized.");
}

/**
 * The main execution loop.
 * A non-blocking timer is utilized to trigger measurements every 3 seconds.
 */
void loop() {
    unsigned long currentMillis = millis();

    // The measurement is triggered only when the 3-second interval has elapsed
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        // A 10-microsecond pulse is emitted to the ultrasonic trigger pin
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        // The duration of the echo return is measured
        long duration = pulseIn(echoPin, HIGH);
        
        // The duration is converted to distance in centimeters
        int distance = duration * 0.034 / 2;

        // The telemetry is formatted and transmitted via UART
        // Format: DIST:X\n allows for easy parsing by the Gateway
        Serial.print("DIST:");
        Serial.println(distance);

        // A secondary alert is triggered if the object is in close proximity, less than 30 cm
        if (distance > 0 && distance < 30) {
            Serial.println("ALERT:PROXIMITY_VIOLATION");
        }
    }
}