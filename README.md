# Environmental-Sensing-Gateway
[![PlatformIO CI](https://github.com/DmaslovDataProg/Environmental-Sensing-Gateway/actions/workflows/main.yml/badge.svg)](https://github.com/DmaslovDataProg/Environmental-Sensing-Gateway/actions/workflows/main.yml)

This repository describes the process of building a multi-processor embedded network (ESP32/Arduino Uno) utilizing Real-Time Operating Systems (FreeRTOS) and validating hardware communication via logic-level protocol analysis. The gateway function is perfromed using ESP32 and the sensing node with the ultrasonic sensor is set on Uno.
![demo assembly](https://github.com/DmaslovDataProg/Environmental-Sensing-Gateway/blob/main/systemView01.jpg)

## Current status
Physical Layer: Multi-voltage gateway (3.3V/5V) is fully integrated and operational.
Firmware: State-machine logic and environmental sensing (BME280) are validated.
Validation: All inter-processor communication (I2C/UART) has been verified via 24MHz Logic Analyzer captures.

## Demonstration
See attached picture of the entire setup and check the [video](https://youtu.be/sx0AgZBQyC4?si=KCqVP1MtJ-Sy3Rkw).
After booting, the ultrasonic sensor starts measuring the distance and Uno passes this information by TX and RX through the 5V to 3.3V shifter to ESP32. The resulting value is provided in the serial output terminal, shown on video. Once the object of detection is distant, only status message appears. However after the object appearing this event is registered and the envirnonemtal data is provided.

## How to Launch This Code
To run this firmware Install VS Code with the PlatformIO extension.
Open: Open the /Firmware_Gateway folder.
Build/Upload: Connect the ESP32 DevKitV1 and utilize the PlatformIO "Upload" button to flash the firmware.
Monitor: Open the Serial Monitor at 115200 baud to view real-time environmental telemetry and autonomy logs.
Repeat the same process for Uno board, however note that the baudrate selected is 9600. 

## Hardware Limitations
- High input capacitance of diagnostic probes (24 MHz 8-channel Logic Analyzer) was found to cause I2C timeouts at high frequencies; the bus is currently restricted to 100kHz for stability during debugging, however should be used with caution. The lab oscilloscope may handle this part better.
- The system relies on a TXS0108E logic level shifter (LLC); signal propagation is limited by the rise-time of the 5V rail pull-ups on the Sensor Node (Uno) side.

## Future Implementations
- Vision-Based Verification: Integration of an ESP32-CAM node triggered via the "Alert" state to capture visual evidence of events that triggered the alert on the Uno.
- RF Telemetry Mesh: Deployment of NRF24L01 transceivers to replace the wired UART link between the sensor node and the gateway for the wireless deployment avoiding Wifi and Bluetooth.
- Edge AI: Implementing TensorFlow Lite for Microcontrollers on the ESP32 to categorize environmental noise patterns directly on the node.

## Lessons Learned 
- A critical 4.2V back-feeding leak was identified on the 3.3V I2C bus during multi-rail integration. The issue was diagnosed via logic analyzer and resolved by isolating the LLC bias rail from the primary power supply.
- The description on the BME280 sensor is misleading, as the SCL and SDA pins swapped in reality, which was very complex to debug. 

