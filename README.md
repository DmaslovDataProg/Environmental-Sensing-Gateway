# Environmental-Sensing-Gateway
This will be a project with three boards that intercommunicate between each other to read environemntal sensing data, as well as spartial sensing and image capturing.

Hardware-Level Telemetry Validation by using Logic Anlyser
Protocol: UART (8-N-1) @ 115200 baud.
Observation: The Logic Analyzer capture confirms the successful execution of the FreeRTOS Sensor Task.
Verification: ASCII decoding of the TX stream (D0) validates the transmission of environmental data (Ambient Temp) formatted for the system dashboard. This confirms the end-to-end integrity from the I2C sensor layer to the UART telemetry layer.