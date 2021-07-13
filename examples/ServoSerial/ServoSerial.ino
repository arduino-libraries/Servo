#include <Servo.h>
Servo s1;
int angle =0;
void setup() {
  Serial.begin(38400);
  s1.attach(9);
  Serial.println("Enter angle from 0 to 180");
}
void loop() {
  static char input[4]; //Declaring a string to store value
  static uint8_t i=0;
  char c ; // Declaring a character to store the input
  int angle =0;
  s1.write(angle);
  if (Serial.available ())
  { 
    c = Serial.read (); 
    if ( c != '\r' && i <4 ) // assuming "Carriage Return" is chosen in the Serial monitor as the line ending character
    {    
      input[i] = c;
      i=i+1;
    }
    else
    {
      input[i] = '\0';
      i = 0; 
    }
    //As serial monitor consider entire number digit vise so stored every digit in string called input
    int pos =0;
    angle = atoi(input);//Converting string to an interger value
    Serial.println("angle is:");
    Serial.println(angle);
    for (pos = 0; pos <= angle; pos += 1) // goes from 0 degrees to angle given in degrees in steps of 1 degree
     { 
      s1.write(pos);                           // tell servo to go to position in variable 'pos'
      delay(15);                              // waits 15ms for the servo to reach the position
     }
     Serial.flush();
  }
}
