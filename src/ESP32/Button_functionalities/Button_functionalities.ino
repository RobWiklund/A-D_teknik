/*
  Created by Robin Wiklund
  This example demonstrates the use of pinMode(INPUT_PULLUP) for 2 external buttons, an external buzzer and the built in LED.

  Last updated 13 Mar 2023
  by Robin Wiklund

*/

#define LED_BUILTIN 2;  // Built in led
#define BUZZER 18;      // External buzzer

const int ledPin = LED_BUILTIN;
const int buzzer = BUZZER;
int isMuted = false;
int ledState = LOW;


void setup() {

  Serial.begin(9600); // Serial Monitor Baud

  //configure pin 22, 23 as an inputs and enable the internal pull-up resistor
  pinMode(22, INPUT_PULLUP); // Wifi reset
  pinMode(23, INPUT_PULLUP); // Mute

  // Pin 2, 18 as output
  pinMode(buzzer, OUTPUT);
  pinMode(ledPin, OUTPUT);
}

//----------------------------- END OF SETUP -----------------------------//

void loop() {

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
}

//----------------------------- END OF LOOP -----------------------------//

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
      MutableBuzzer(50, 2); // 250ms Delay, buzz 2 times.
      digitalWrite(ledPin, LOW); // Led off
      
      // --!-- Wifi reset call here... --!-- //
      
      while (digitalRead(22) == LOW); // Wait for the button to be released
      break; // Exit the loop
    }
  }
}

//----------------------------- END OF WifiReset -----------------------------//

bool ToggleMuteBtn() 
{
  static bool muted = false;
  
  // --!-- Add check for alarm here, if alarm -> don't mute, instead cancel alarm. --!-- //
  
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

