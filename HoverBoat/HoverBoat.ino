/* Created by : Richard Blair
   Creation Date: 25/06/2018
   Version Number: 1.02
   Gitrepository - https://github.com/CompEng0001/HoverBoat

   Notes - This sketch is for use with Year 0's project 3. The sketch has been written to take instructions from the serial specifically when used with btle HC-05.
         - Sections of this sketch should not be changed.
         - Void Setup, voide serialListener
         -
*/
#include <Servo.h> //  DON'T NOT CHANGE THIS LIBRARY IS ESSIENTIAL

Servo myservo;  // create servo object to control a servo
int pos;    // variable to store the servo position - NOTE - this can only be 0 to 180.
String serialCommand; // variable to hold incoming string

// Here you can change your EXPECTED serial input string or add more
String left = "l";
String right = "r";
String center = "c";

// define type for leds to be used with accelerometer CAN BE CHANGED TO MATCH YOUR INPUTS/OUTPUTS
int xLevelLeftLed = 10;
int xLevelRightLed = 11;
int zLevelIndicatorLed = 9; #

double x y z;

void setup()
{
  Serial.begin(9600); // open serial port and declare the Baud Rate
  myservo.attach(9); // give servo power
  Serial.println(myservo.read()); // for debugging to see intial postition of servo should be between 88-93

  // for accelerometer level indicators
  pinMode(xLevelLeftLed, OUTPUT);
  pinMode(xLevelRightLed, OUTPUT);
  pinMode(zLevelIndicatorLed, OUTPUT);
}

/* The SerialListener function
   DON'T CHANGE THE FUNCTION
   Uses a while loop with the condition Serial.available() this will remain true forever as the setup calls Serial.begin.
   Could this work for voice??
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
  if (serialCommand.length() > 0)
  {
    //Serial.println(pos);
    if (serialCommand == left) // turn servo left
    {
      pos = myservo.read();
      if (pos != 0) // servos can't go lower 0 degrees
      {
        pos -= 20;
        myservo.write(pos);  // sets the servos position
        Serial.println(pos); //for debugging
      }
    }
    else if (serialCommand == right) // turn servo right
    {
      pos = myservo.read();
      if (pos != 180) // servos can't go greater than 180  degrees
      {
        pos += 20;
        myservo.write(pos); // sets the servos position
        Serial.println(pos); //for debugging
      }
    }
    else if (serialCommand == center) // centres the servo
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
  /* CAN BE CHANGED
     Analog read pins
  */
  const int xPin = 0;
  const int yPin = 1;
  const int zPin = 2;

  /* YOU MAY NEED TO CHANGE THESE VALUES
     The minimum and maximum values that came from
     the accelerometer while standing still in my case
  */
  int minVal = 265;
  int maxVal = 402;

  /* DON'T CHANGE
     Reads the analog values from the accelerometer
  */
  int xRead = analogRead(xPin);
  int yRead = analogRead(yPin);
  int zRead = analogRead(zPin);

  /*DON'T CHANGE
    Convert read values to degrees -90 t'o 90 - Needed for atan2
  */
  int xAng = map(xRead, minVal, maxVal, -90, 90);
  int yAng = map(yRead, minVal, maxVal, -90, 90);
  int zAng = map(zRead, minVal, maxVal, -90, 90);

  /* DON'T CHANGE
    Caculates 360deg values like so: atan2(-yAng, -zAng)
    atan2 outputs the value of -π to π (radians)
    We are then converting the radians to degrees
  */
  x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);


  /* Output the caculations, you can comment out these lines after DEBUGGING
     HINT: highlight -> right click -> select comment/uncomment
  */
  Serial.print("x: ");
  Serial.print(x);
  Serial.print(" | y: ");
  Serial.print(y);
  Serial.print(" | z: ");
  Serial.println(z);

  delay(200) // slows down the serial print for ease of viewing 
}

void equilibrium()
{
  /* The space below is for you to try and use the led indicators to display if you are level
     use x, y, z and implement a switch or if statement to set the digitalWrite( var, HIGH/LOW )
     Calibrate your values 
  */
  if (x >= 400 && x <= 450) 
  {
    digitalWrite(ledxGreen, HIGH);
    digitalWrite(ledxRed, LOW;
  }
  else if (x < 400  || x > 450)
  {
    digitalWrite(ledxRed, HIGH);
    digitalWrite(ledxGreen, LOW);
  }

  if (z >= 400 && z <= 450)
  {
    digitalWrite(ledzGreen, HIGH);
    digitalWrite(ledzRed, LOW);
  }
  else if (z < 400 || z > 450)
  {
    digitalWrite(ledzRed, HIGH);
    digitalWrite(ledzGreen, LOW);
  }
}

void loop()
{
  /* CODE RUNS FOREVER
     Calls functions sequentially
  */
  accelerometer(); // clear this comment if you wish to use the accelerometer
  equilibrium(); // visual indication if level
  serialListener(); // calls the serialListener function to pass incoming data to be used in moveCommand
  moveCommand(); // calls moveCommand and processes the data from serialListener to control the servo(s)
  serialCommand = ""; //  clears string ready for next input
}
