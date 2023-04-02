/*
  Created 16 mar 2023 by Robin Wiklund

  Last updated 26 mar 2023
  by Robin Wiklund
*/


int buzzerPin = 18;  // set the buzzer pin
int ledpin = 2;      // set the internal led pin


void setup() {
  Serial.begin(9600); // baud rate. Default Data Bits: 8, stop bit: 1, No parity.
  pinMode(buzzerPin, OUTPUT); // set buzzer as output
  pinMode(ledpin, OUTPUT);    // set internal led as output
  } 

void loop() {
  if (Serial.available() > 0) // if there is data available on the serial port
  {  
    digitalWrite(ledpin, HIGH); // Serial has successfully received data 

    String SerialString = Serial.readString();  //read until timeout
    SerialString.trim();                        // remove any \r \n whitespace at the end of the String
    if (SerialString.charAt(0) == '1') {  // First character is: 1
    digitalWrite(buzzerPin, HIGH);        // turn on the buzzer
      delay(100);  // wait for 500ms
      digitalWrite(buzzerPin, LOW);  // turn off the buzzer
      delay(100);
    } else { }
  }
}