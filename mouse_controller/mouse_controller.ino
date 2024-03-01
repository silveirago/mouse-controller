/* 

Mouse Controller
Control your mouse cursor with an Arduino Pro Micro

by Gustavo Silveira - a.k.a. the Nerd Musician

to learn more go to https://go.musiconerd.com/nerd-musician-pro

*/

#include <Encoder.h>
#include "Mouse.h"

// Encoder setup
Encoder myEnc(3, 2);  // Change these numbers to the pins connected to your encoder

// Mouse movement variables
int xVal, yVal, newPos = 0;
boolean mouseY = true;
int inc = 1;

// Button setup
const int N_BUTTONS = 1;                           // Total number of buttons
const int BUTTON_ARDUINO_PIN[N_BUTTONS] = { 10 };  // Pins of each button connected straight to the Arduino
int buttonCState[N_BUTTONS] = {};                  // Stores the button current value
int buttonPState[N_BUTTONS] = {};                  // Stores the button previous value

// Button debounce
unsigned long lastDebounceTime[N_BUTTONS] = { 0 };  // Last time the output pin was toggled
unsigned long debounceDelay = 50;                   // Debounce time; increase if the output flickers

void setup() {
  Serial.begin(9600);
  Serial.println("Basic Encoder Test:");

  // Initialize buttons with pull-up resistors
  for (int i = 0; i < N_BUTTONS; i++) {
    pinMode(BUTTON_ARDUINO_PIN[i], INPUT_PULLUP);
  }
}

long oldPosition = -999;

void loop() {
  // Check buttons for state changes
  buttons();

  // Read encoder position
  long newPosition = myEnc.read();

  // Process encoder movement
  if (newPosition != oldPosition) {
    if (newPosition > oldPosition) {
      // Encoder turned clockwise
      if (mouseY == true) {
        yVal = -inc;
        Mouse.move(0, yVal, 0);
        printY();
      } else {
        xVal = inc;
        Mouse.move(xVal, 0, 0);
        printX();
      }
    } else {
      // Encoder turned counter-clockwise
      if (mouseY == false) {
        xVal = -inc;
        Mouse.move(xVal, 0, 0);
        printX();
      } else {
        yVal = inc;
        Mouse.move(0, yVal, 0);
        printY();
      }
    }

    // Print encoder position
    Serial.print("  |  Pos: ");
    Serial.println(newPosition);

    oldPosition = newPosition;
  }
}

// Print Y-axis position
void printY() {
  Serial.print("Pos Y: ");
  Serial.print(yVal);
}

// Print X-axis position
void printX() {
  Serial.print("Pos X: ");
  Serial.print(xVal);
}

// Handle button state changes
void buttons() {
  for (int i = 0; i < N_BUTTONS; i++) {
    // Read buttons' states
    buttonCState[i] = digitalRead(BUTTON_ARDUINO_PIN[i]);

    // Invert pin 13 due to pull-down resistor
#ifdef pin13
    if (i == pin13index) {
      buttonCState[i] = !buttonCState[i];
    }
#endif

    // Check for button state changes with debounce
    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      if (buttonPState[i] != buttonCState[i]) {
        lastDebounceTime[i] = millis();

        // If button is pressed
        if (buttonCState[i] == LOW) {
          mouseY = !mouseY;
          Serial.print("BUTTON: ");
          Serial.print(mouseY);
        }

        // Update previous button state
        buttonPState[i] = buttonCState[i];
      }
    }
  }
}
