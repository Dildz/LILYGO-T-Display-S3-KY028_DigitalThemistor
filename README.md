LILYGO T-Display-S3 KY-028 Digital Thermistor Sensor Project

This sketch reads the analog and digital signals from the KY-028 module and displays them on the built-in screen of the LilyGO T-Display-S3 using the TFT_eSPI library.
The KY-028 module includes an NTC thermistor and a comparator circuit with a potentiometer.

Description:

  This code reads analog and digital signals from the KY-028 module and displays them on the built-in screen of the LilyGO T-Display-S3 using the TFT_eSPI library.
  The code uses state machine logic to avoid using delays, which is code blocking.
  The screen is only updated if there is a change in readings.

How to Use the KY-028 Potentiometer:

 1. The potentiometer on the KY-028 module adjusts the threshold voltage for the digital output.
 2. Rotate the potentiometer clockwise to increase the threshold
     (higher temperature required to trigger digital output).
 3. Rotate the potentiometer counterclockwise to decrease the threshold
     (lower temperature required to trigger digital output).
 4. The digital output will go HIGH when the temperature exceeds the threshold set by the potentiometer.
 5. Use the analog output to read the raw analog value.

Pin Connections:

 - KY-028 VCC  -> 3.3V
 - KY-028 GND  -> GND
 - KY-028 A0   -> A0 (pin 1)
 - KY-028 D0   -> GPIO2 (pin 2)

KY-028 Specications:

 - Protocol: Analog and Digital
 - Operating Voltage: 3.3V to 5.5V
 - Temperature Measurement Range: -55°C to +125°C
 - Measurement Accuracy: ±0.5°C
