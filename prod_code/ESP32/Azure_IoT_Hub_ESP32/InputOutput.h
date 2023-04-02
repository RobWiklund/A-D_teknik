/*
  Created by Robin Wiklund

  Last updated 13 Mar 2023
  by Robin Wiklund

*/

#ifndef INPUTOUTPUT_H
#define INPUTOUTPUT_H

#include <Arduino.h>

// Define constants
#define LED_BUILTIN 2  // Built-in LED
#define BUZZER 18      // External buzzer

// Declare functions
void setupIO();
void checkBtns(); // Interrupt by pressing buttons
void WifiResetBtn();
bool ToggleMuteBtn();
void MutableBuzzer(int interval, int nTimesBuzz);
void checkAlarm (bool AlarmStatus);

#endif
