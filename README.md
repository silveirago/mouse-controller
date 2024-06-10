# Mouse Controller

Control your mouse cursor with an Arduino Pro Micro.

by Gustavo Silveira - a.k.a. the Nerd Musician

To learn more go to [Nerd Musician Pro](https://go.musiconerd.com/nerd-musician-pro)

## Introduction

This project demonstrates how to control your computer's mouse cursor using an Arduino Pro Micro. It utilizes an encoder for directional input and a button to switch between X and Y-axis control.

## Components

- Arduino Pro Micro
- Rotary Encoder with push button
- Connecting wires

## Code Explanation

### Include Libraries

The required libraries for this project are the `Encoder` and `Mouse` libraries. Ensure you have them installed in your Arduino IDE.

```cpp
#include <Encoder.h>
#include "Mouse.h"
```

### Encoder Setup

Initialize the encoder with the pins it's connected to.

```cpp
Encoder myEnc(3, 2);  // Change these numbers to the pins connected to your encoder
```

### Mouse Movement Variables

Define variables for mouse movement and encoder position.

```cpp
int xVal, yVal, newPos = 0;
boolean mouseY = true;
int inc = 1;
```

### Button Setup

Setup the button input and debounce variables.

```cpp
const int N_BUTTONS = 1;                           // Total number of buttons
const int BUTTON_ARDUINO_PIN[N_BUTTONS] = { 10 };  // Pins of each button connected straight to the Arduino
int buttonCState[N_BUTTONS] = {};                  // Stores the button current value
int buttonPState[N_BUTTONS] = {};                  // Stores the button previous value

unsigned long lastDebounceTime[N_BUTTONS] = { 0 };  // Last time the output pin was toggled
unsigned long debounceDelay = 50;                   // Debounce time; increase if the output flickers
```

### Setup Function

Initialize serial communication and set up the button pin with a pull-up resistor.

```cpp
void setup() {
  Serial.begin(9600);
  Serial.println("Basic Encoder Test:");

  // Initialize buttons with pull-up resistors
  for (int i = 0; i < N_BUTTONS; i++) {
    pinMode(BUTTON_ARDUINO_PIN[i], INPUT_PULLUP);
  }
}
```

### Main Loop

The main loop checks for button state changes and processes encoder movements to control the mouse cursor.

```cpp
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
```

### Print Functions

These functions print the current X or Y position to the serial monitor.

```cpp
void printY() {
  Serial.print("Pos Y: ");
  Serial.print(yVal);
}

void printX() {
  Serial.print("Pos X: ");
  Serial.print(xVal);
}
```

### Button Handling

This function handles the button state changes with debounce.

```cpp
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
```

## Conclusion

This project showcases how to control your mouse using an Arduino and an encoder. You can expand this project by adding more buttons or adjusting the encoder resolution.
