/*
  Created by Robin Wiklund

  Last updated 13 Mar 2023
  by Robin Wiklund

*/
#include <Arduino.h>
#include <WiFiManager.h>

#define LED_BUILTIN 2;  // Built in led
#define BUZZER 18;      // External buzzer
#define ALARM_FREQUENCY_MILLISECS 1000

const int ledPin = LED_BUILTIN;
const int buzzer = BUZZER;
int isMuted = false;
int ledState = LOW;
int APtimeout = 120; // seconds to run for Access Point

// alarm 
int str = 255; // Buzzer strength
int buzzTimes = 0;
int maxBuzzTimes = 10; // 30 seconds
static unsigned long next_alarm_time_ms = 0; // time counter


void setupIO() {

  //configure pin 22, 23 as an inputs and enable the internal pull-up resistor
  pinMode(22, INPUT_PULLUP); // Wifi reset
  pinMode(23, INPUT_PULLUP); // Mute

  // Pin 2, 18 as output
  pinMode(buzzer, OUTPUT);
  pinMode(ledPin, OUTPUT);
}

//----------------------------- END OF setupIO -----------------------------//

void setupWifiConnection()  // open OnDemandAP
{
  WiFiManager wm;    

  Serial.println("Setting up access point");
  //reset settings - for testing
  //wm.resetSettings();

  // set configportal timeout
  wm.setConfigPortalTimeout(APtimeout);

  if (!wm.startConfigPortal("OnDemandAP")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
    }
}

//----------------------------- END OF setupWifiConnection -----------------------------//

bool ToggleMuteBtn() 
{
  static bool muted = false;
  
  muted = !muted; // Toggles value

  if (!muted) // Plays asc tone and blinks the LED
  {
    analogWrite(buzzer, 3);
    digitalWrite(ledPin, HIGH);
    delay(150);
    analogWrite(buzzer, 12);
    delay(150);
    analogWrite(buzzer, 30);
    digitalWrite(ledPin, LOW);
    delay(150);
    analogWrite(buzzer, 150);
    digitalWrite(ledPin, HIGH);
    delay(350);
  }
  else // muted & Plays desc tone and blinks the LED
  { 
    analogWrite(buzzer, 150);
    digitalWrite(ledPin, HIGH);
    delay(150);
    analogWrite(buzzer, 30);
    digitalWrite(ledPin, LOW);
    delay(150);
    analogWrite(buzzer, 12);
    delay(150);
    analogWrite(buzzer, 3);
    digitalWrite(ledPin, HIGH);
    delay(350);
    }
    
  digitalWrite(ledPin, LOW);
  analogWrite(buzzer, 0);
  return muted;
}

//----------------------------- END OF ToggleMuteBtn -----------------------------//

void MutableBuzzer(int interval, int nTimesBuzz) 
{
  int n = 1; // start count at

  while(!isMuted && nTimesBuzz >= n) {
    analogWrite(buzzer, 255);
    delay(interval);
    analogWrite(buzzer, 0);
    n++;
    delay(interval);
  }
}

//----------------------------- END OF MutableBuzzer -----------------------------//

void WifiResetBtn()
{
  unsigned long startTime = millis(); // Store the current time since start
  while (digitalRead(22) == LOW) { // Wait for the button to be released or held down for 3 seconds
    delay(250);
    if (ledState == LOW) { // Toggles values
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(ledPin, ledState);
    
    if (digitalRead(22) == HIGH) { // button released before holding 3 seconds.
      digitalWrite(ledPin, LOW);  // Led off
      break;
    }

    if (millis() - startTime >= 3000) { // If the button has been held down for 3 seconds
      Serial.println("Button was held down for 3 seconds!"); // Print a message to the serial monitor
      MutableBuzzer(50, 8); // 50ms Delay, buzz 8 times.
      digitalWrite(ledPin, LOW); // Led off
      setupWifiConnection(); // Setup wifi from AP
      while (digitalRead(22) == LOW); // Wait for the button to be released
      break; // Exit the loop
    }
  }
}

//----------------------------- END OF WifiReset -----------------------------//

void checkBtns() {

  // Update input value every loop 
  int WifiBtn = digitalRead(22);
  int MuteBtn = digitalRead(23);

  // Keep in mind the pull-up means the pushbutton's logic is inverted. It goes
  // HIGH when it's open, and LOW when it's pressed.

  if (WifiBtn == LOW) { // Wifi reset button is pressed | Also needs to be hold down for 3 seconds to reset
    WifiResetBtn(); // Calling function
  }

  if (MuteBtn == LOW) { // Mute button is pressed 
    isMuted = ToggleMuteBtn(); // Function sets isMuted to true or false 
    Serial.print("Buzzer is muted: ");
    Serial.println(isMuted ? "Yes" : "No"); // Boolean true | false
    delay(300); // Able to press button again after 0,3 seconds  
  } 
  return;
}

//----------------------------- END OF checkBtns -----------------------------//

void checkAlarm (bool activeAlarm)
{
  if (millis() > next_alarm_time_ms && activeAlarm && !isMuted)
  {
    if (buzzTimes <= maxBuzzTimes) 
    {
      analogWrite(buzzer, str);
      if (str == 255) {
        str = 0;
      } else {
        str = 255;
      }
      buzzTimes++;
    } 
    else // Buzzing for 60s, now silence
    { 
       analogWrite(buzzer, 0); 
    }
    next_alarm_time_ms = millis() + ALARM_FREQUENCY_MILLISECS; // next time = now + 1s
  } 
  else if (!activeAlarm) 
  {
    buzzTimes = 0;
    analogWrite(buzzer, 0); 
  }
}

//----------------------------- END OF checkAlarm -----------------------------//
