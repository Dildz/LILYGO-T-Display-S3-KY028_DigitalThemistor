/*********************************************************************************************************
* LILYGO T-Display-S3 KY-028 Digital Thermistor Sensor Project
*
* This sketch reads the analog and digital signals from the KY-028 module and displays them on the
*  built-in screen of the LilyGO T-Display-S3 using the TFT_eSPI library.
* The KY-028 module includes an NTC thermistor and a comparator circuit with a potentiometer.

* Description:
*   This code reads analog and digital signals from the KY-028 module and displays them on the
*  built-in screen of the LilyGO T-Display-S3 using the TFT_eSPI library.
*   The code uses state machine logic to avoid using delays, which is code blocking.
*   The screen is only updated if there is a change in readings.

* How to Use the KY-028 Potentiometer:
*  1. The potentiometer on the KY-028 module adjusts the threshold voltage for the digital output.
*  2. Rotate the potentiometer clockwise to increase the threshold
*      (higher temperature required to trigger digital output).
*  3. Rotate the potentiometer counterclockwise to decrease the threshold
*      (lower temperature required to trigger digital output).
*  4. The digital output will go HIGH when the temperature exceeds the threshold set by the potentiometer.
*  5. Use the analog output to read the raw analog value.
*
* Pin Connections:
*  - KY-028 VCC  -> 3.3V
*  - KY-028 GND  -> GND
*  - KY-028 A0   -> A0 (pin 1)
*  - KY-028 D0   -> GPIO2 (pin 2)
*
* KY-028 Specications:
*  - Protocol: Analog and Digital
*  - Operating Voltage: 3.3V to 5.5V
*  - Temperature Measurement Range: -55°C to +125°C
*  - Measurement Accuracy: ±0.5°C
**********************************************************************************************************/

/*************************************************************
******************* INCLUDES & DEFINITIONS *******************
**************************************************************/

#include <Arduino.h>
#include <TFT_eSPI.h>

// TFT_eSPI
TFT_eSPI tft = TFT_eSPI();

// Pin definitions
const int analogPin = A0;   // analog pin for thermistor value
const int digitalPin = 2;   // digital pin for threshold trigger

// Variables for timing
unsigned long previousMillis = 0;
const long interval = 1000;  // interval for reading and printing (ms)

// State Machine States
enum class State {
  READ_SENSOR,    // state for reading sensor data
  UPDATE_DISPLAY, // state for updating the display
  WAIT            // state for waiting between sensor reads
};

// Global variables
State currentState = State::READ_SENSOR; // initial state
int rawValue = 0;                        // raw analog value from the sensor
int digitalState = LOW;                  // digital state from the sensor
bool redrawRequired = true;              // flag to redraw the screen


/*************************************************************
********************** HELPER FUNCTIONS **********************
**************************************************************/

// Function to draw static elements on the TFT screen
void drawStaticElements() {
  // Clear the screen and set text properties
  tft.fillScreen(TFT_BLACK);
  tft.setTextFont(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  // Reset cursor to the top-left corner
  tft.setCursor(0, 0);

  // Display header
  tft.println("---------------------------");
  tft.println("KY028 Thermistor Module");
  tft.println("---------------------------");

  // Display static labels
  tft.setCursor(0, 70); // position for analog value
  tft.print("Analog Value: ");

  tft.setCursor(0, 100); // position for digital state
  tft.print("Digital State: ");
}

// Function to update dynamic elements on the TFT screen
void updateDynamicElements() {
  // Update analog value
  tft.setCursor(100, 70); // position for analog value
  tft.print("     ");     // clear previous value (5 spaces for padding)
  tft.setCursor(100, 70); // reset cursor
  tft.print(rawValue);

  // Update digital state
  tft.setCursor(100, 100); // position for digital state
  tft.print("     ");      // clear previous value (5 spaces for padding)
  tft.setCursor(100, 100); // reset cursor
  tft.print(digitalState == HIGH ? "HIGH" : "LOW");
}


/*************************************************************
*********************** MAIN FUNCTIONS ***********************
**************************************************************/

// SETUP
void setup() {
  // Set pin modes
  pinMode(analogPin, INPUT);
  pinMode(digitalPin, INPUT);

  // Initialize the TFT display
  tft.init();
  tft.setRotation(0);                     // adjust rotation (0 & 2 portrait | 1 & 3 landscape)
  tft.fillScreen(TFT_BLACK);              // black background
  tft.setTextFont(2);                     // set the font size
  tft.setTextColor(TFT_WHITE, TFT_BLACK); // set text colour (white) and background colour (black)

  // Draw static elements once
  drawStaticElements();

  // Initial display update
  redrawRequired = true; // update display
}


// MAIN LOOP
void loop() {
  unsigned long currentMillis = millis(); // Get the current millis time

  // State Machine Logic
  switch (currentState) {
    case State::READ_SENSOR:
      // Read the analog value from the sensor
      rawValue = analogRead(analogPin);

      // Read the digital value from the sensor
      digitalState = digitalRead(digitalPin);

      // Print the raw analog value and digital state to the serial monitor
      Serial.print("\nAnalog Value: ");
      Serial.print(rawValue);
      Serial.print("\nDigital State: ");
      Serial.println(digitalState);

      // Move to the next state
      currentState = State::UPDATE_DISPLAY;
      break;

    case State::UPDATE_DISPLAY:
      // Update the display with the new sensor data
      updateDynamicElements();

      // Move to the WAIT state
      currentState = State::WAIT;
      previousMillis = currentMillis; // record the previous millis time
      break;

    case State::WAIT:
      // Wait for the specified interval before reading the sensor again
      if (currentMillis - previousMillis >= interval) {
        currentState = State::READ_SENSOR;
        redrawRequired = true; // update display
      }
      break;

    default:
      // Default case (should not happen)
      currentState = State::READ_SENSOR;
      break;
  }
}