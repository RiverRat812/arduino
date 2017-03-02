// FightStick Layout
// (4 way D Pad + A, B, X, Y, L1, R1, L2, R2 Buttons)
// Buttons are in an Array setup for a Pro Micro Board
// Digital 2-9 on one side of board
// Digital 10, 16, 14, 15 and A0 and A1 are used for 18 and 19
// all pins are grounded when they are pressed.
// Pin 2 = UP
// Pin 3 = RIGHT
// Pin 4 = DOWN
// Pin 5 = LEFT
// Pin 6 = A
// Pin 7 = B
// Pin 8 = X
// Pin 9 = Y
// Pin 10 = L1
// Pin 16 = R2
// Pin 14 = R1
// Pin 15 = L2
// Pin A0 = Select
// Pin A1 = Start
//
// NOTE: This sketch file is for use with Arduino Leonardo and
//       Arduino Micro only.
//
// Original by Matthew Heironimus
// 2016-11-24
//
// Modified version by Ray Tuholski
// 2017-02-26
//--------------------------------------------------------------------

#include <Joystick.h>

int             inputPin[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 14, 15, 16, 18, 19 };
int             pinCount = 14;


Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
                   10, 0,                  // Button Count, Hat Switch Count
                   true, true, false,     // X and Y, but no Z Axis
                   false, false, false,   // No Rx, Ry, or Rz
                   false, false,          // No rudder or throttle
                   false, false, false);  // No accelerator, brake, or steering

void setup() {

  // Initialize Button Pins
  for (int index = 0; index < pinCount; index++) {
    pinMode(inputPin[index], INPUT_PULLUP);
  }

  // Initialize Joystick Library
  Joystick.begin();
  Joystick.setXAxisRange(-1, 1);
  Joystick.setYAxisRange(-1, 1);
}

// Last state of the buttons
int lastButtonState[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void loop() {

  // Read pin values
  for (int index = 0; index < pinCount; index++) {
    int currentButtonState = !digitalRead(inputPin[index]);
    if (currentButtonState != lastButtonState[index])
    {
      switch (index) {
        case 0: // UP
          if (currentButtonState == 1) {
            Joystick.setYAxis(-1);
          } else {
            Joystick.setYAxis(0);
          }
          break;
        case 1: // RIGHT
          if (currentButtonState == 1) {
            Joystick.setXAxis(1);
          } else {
            Joystick.setXAxis(0);
          }
          break;
        case 2: // DOWN
          if (currentButtonState == 1) {
            Joystick.setYAxis(1);
          } else {
            Joystick.setYAxis(0);
          }
          break;
        case 3: // LEFT
          if (currentButtonState == 1) {
            Joystick.setXAxis(-1);
          } else {
            Joystick.setXAxis(0);
          }
          break;
        case 4: // A
          Joystick.setButton(0, currentButtonState);
          break;
        case 5: // B
          Joystick.setButton(1, currentButtonState);
          break;
        case 6: // X
          Joystick.setButton(2, currentButtonState);
          break;
        case 7: // Y
          Joystick.setButton(3, currentButtonState);
          break;
        case 8: // L1
          Joystick.setButton(4, currentButtonState);
          break;
        case 9: // R1
          Joystick.setButton(5, currentButtonState);
          break;
        case 10: // L2
          Joystick.setButton(6, currentButtonState);
          break;
        case 11: // R2
          Joystick.setButton(7, currentButtonState);
          break;
        case 12: // Select
          Joystick.setButton(8, currentButtonState);
          break;
        case 13: // Start
          Joystick.setButton(9, currentButtonState);
          break;
      }
      lastButtonState[index] = currentButtonState;
    }
  }
  delay(10);
}
