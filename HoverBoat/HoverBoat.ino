/*------------------------------------------------------------------------
  Copyright [2019] [Richard Blair]
  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
  ------------------------------------------------------------------------
  Creation Date: 25/06/2018
  Latest Modified Date: 16/02/2019
  Version Number: 1.5
  License: Apache 2.0
  Gitrepository - https://github.com/CompEng0001/HoverBoat
  -----------------------------------------------------------------------
  Notes - This sketch is for use with Year 0's project 3. The sketch has been written to take instructions from the serial specifically when used with HC-05.
*/
#include <Servo.h> //  DON'T NOT CHANGE THIS LIBRARY IS ESSIENTIAL

Servo myservo;  // create servo object to control a servo
int pos;    // variable to store the servo position - NOTE - this can only be 0 to 180.
String serialCommand; // variable to hold incoming string

// Here you can change your EXPECTED serial input string or add more
String left = "L";
String right = "R";
String center = "C";

// variables for accelerometer
int xRead, yRead, zRead;

// define type for leds to be used with accelerometer CAN BE CHANGED TO MATCH YOUR INPUTS/OUTPUTS
int ledXGreen = 10, ledXRed = 11;

void setup()
{
  Serial.begin(9600); // open serial port and declare the Baud Rate
  myservo.attach(9); // Note that in Arduino 0016 and earlier, the Servo library supports only servos on only two pins: 9 and 10
  Serial.println(myservo.read()); // for debugging to see intial postition of servo should be between 88-93

  // for accelerometer level indicators
  pinMode(ledXGreen, OUTPUT);
  pinMode(ledXRed, OUTPUT);

}

/* CODE RUNS FOREVER
   Calls functions sequentially
*/
void loop()
{

  accelerometer(); // clear this comment if you wish to use the accelerometer
  equilibrium(); // visual indication if level
  serialListener(); // calls the serialListener function to pass incoming data to be used in moveCommand
  moveCommand(); // calls moveCommand and processes the data from serialListener to control the servo(s)
  serialCommand = ""; //  clears string ready for next input
}

/* The SerialListener function
   DON'T CHANGE THE FUNCTION
   Uses a while loop with the condition Serial.available() is true when something is in the buffer.
   Buffer can take 64 Bytes.
   Could this work for Speech?
*/
void serialListener()
{
  while (Serial.available())
  {
    delay(10);
    char c = Serial.read();// reading the string sent by google voice
    //delay(10); //  adds buffer to catcher whole data.
    if (c == '#')
    {
      break; // will terminate the while loop
    }
    serialCommand += c;
    Serial.println(serialCommand); // for Debugging to show what was communicated to the serial port
  }
}

/* The moveCommand function
   You are encouraged to change this by adding more functionality and fine tuning, for instance try different angles.
   When the function is called the first if statement checks the serialCommand length which must be greater than zero
   The imbedded *if* statements check for the content of the string, left, right or centre then initiates a change in position.
*/
void moveCommand()
{
  pos = myservo.read(); 
  if (serialCommand.length() > 0)
  {
    Serial.println("Greater than 0");
    //Serial.println(pos);
    if (serialCommand.indexOf(left)) // turn servo left
    {
      if (pos != 0) // servos can't go lower 0 degrees
      {
        pos -= 20;
        myservo.write(pos);  // sets the servos position
        Serial.println(pos); //for debugging
      }
    }
    else if (serialCommand.indexOf(right)) // turn servo right
    {
      if (pos != 180) // servos can't go greater than 180  degrees
      {
        pos += 20;
        myservo.write(pos); // sets the servos position
        Serial.println(pos); //for debugging
      }
    }
    else if (serialCommand.indexOf(center)) // centres the servo
    {
      pos = 90;
      myservo.write(pos); // sets the servos position
      Serial.println(pos); //for debugging
    }
  }
}
/*The accelerometer function is not mandatory.
   You can use this function if you feel up for a challenge
*/
void accelerometer()
{
  //CAN CHANGE THE PINS
  //Reads the analog values from the accelerometer
  xRead = analogRead(A0);
  yRead = analogRead(A1);
  zRead = analogRead(A2);

  /*IF YOU ARE UP FOR CHALLENGE CAN YOU USE THE CODE BELOW
    the code below is converts the ADC values to angle in degrees,
    you'll then need to setup equilibrium function to be more/less receptive
    to the values.
  */
  /*
    //The minimum and maximum values that came from
    //the accelerometer while standing still in my case
    //YOU MAY NEED TO CHANGE THESE VALUES
    int minVal = 265;
    int maxVal = 402;

    //DON'T CHANGE
    //Convert read values to degrees -90 t'o 90 - Needed for atan2
    int xAng = map(xRead, minVal, maxVal, -90, 90);
    int yAng = map(yRead, minVal, maxVal, -90, 90);
    int zAng = map(zRead, minVal, maxVal, -90, 90);

    //DON'T CHANGE
    //Caculates 360deg values like: atan2(-yAng, -zAng)
    //atan2 outputs the value of -π to π (radians)
    //We are then converting the radians to degrees
    float rad2Deg = 57.3249;
    xRead = rad2Deg * (atan2(-yAng, -zAng) + PI);
    yRead = rad2Deg * (atan2(-xAng, -zAng) + PI);
    zRead = rad2Deg * (atan2(-yAng, -xAng) + PI);
  */

 /* //Output the caculations, you can comment out these lines after DEBUGGING
 
  Serial.print("x: ");

  Serial.print(xRead);
  Serial.print(" | y: ");
  Serial.print(yRead);
  Serial.print(" | z: ");
  Serial.println(zRead);

  delay(200); // slows down the serial print for ease of viewing
 */ 
}

void equilibrium()
{
  /* The space below is for you to try and use the led indicators to display if you are level
     use xRead, yRead and zRead and implement a switch or if statement to set the digitalWrite( var, HIGH/LOW )
     xRead is done as example, don't forget to calibrate your values
  */

  //XPLANE
  if (xRead >= 340 && xRead <= 360)
  {
    digitalWrite(ledXGreen, HIGH);
    digitalWrite(ledXRed, LOW);
  }
  else if (xRead < 340  || xRead > 360)
  {
    digitalWrite(ledXRed, HIGH);
    digitalWrite(ledXGreen, LOW);
  }
  //YPLANE
  //ZPLANE 
}
