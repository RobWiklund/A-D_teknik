/*
  Input Pull-up Serial
  This example demonstrates the use of pinMode(INPUT_PULLUP). It reads a digital
  input on pin 22 and 23, then prints the results to the Serial Monitor.

  The circuit:
  - momentary switch attached from pin 22 and 23 to ground
  - built-in LED can be used on pin 13

  Unlike pinMode(INPUT), there is no pull-down resistor necessary. An internal
  20K-ohm resistor is pulled to 5V. This configuration causes the input to read
  HIGH when the switch is open, and LOW when it is closed.

  created 14 Mar 2012
  by Scott Fitzgerald
  This example code is in the public domain.
  https://www.arduino.cc/en/Tutorial/InputPullupSerial

  Last updated 23 Feb 2023
  by Robin Wiklund

*/

void setup() {

  //start serial connection

  Serial.begin(9600);

  //configure pin 23, 24 as an inputs and enable the internal pull-up resistor

  pinMode(22, INPUT_PULLUP); // Wifi reset
  pinMode(23, INPUT_PULLUP); // Extra function

  // Pin 18 as output
  pinMode(18, OUTPUT); // Buzzer

}

void loop() {

  int WifiBtn = digitalRead(22);
  int ExtraBtn = digitalRead(23);

  Serial.println(WifiBtn);
  Serial.println(ExtraBtn);

  // Keep in mind the pull-up means the pushbutton's logic is inverted. It goes
  // HIGH when it's open, and LOW when it's pressed. Turn on pin 18 when the
  // button's pressed, and off when it's not:

  if (WifiBtn == LOW) { // Wifi reset button is pressed

    digitalWrite(13, HIGH);
    digitalWrite(18, HIGH);
    Serial.println("HIGH output");

  } else if (ExtraBtn == LOW) { // Extra button is pressed

    digitalWrite(18, HIGH);
    digitalWrite(13, HIGH);
    Serial.println("HIGH output");
  
  }else {

    digitalWrite(13, LOW);
    digitalWrite(18, LOW);
  }
}