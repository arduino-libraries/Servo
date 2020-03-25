/*
Controling multiple servo motors using attach() and detach()
by Bhargav Patel <https://github.com/Engineer1999/>

*/

#include <Servo.h> 

Servo myservo;  // create servo object to control a servo

void setup() 
{ 
  Serial.begin(9600);              // initializing serial communication with 9600 baud rate
  myservo.attach(10);              // attaches the servo on pin 10 to the servo object
  myservo.writeMicroseconds(500);  // setting initial angle to 0 (analog value = 500 [any value less than 700])
  
} 

void loop() {
  angle = myservo.read();          // read the value of the current angle on servo motor
  Serial.print("Inital angle:- "); 
  Serial.println(angle);           // print angle on servo motor
  myservo.writeMicroseconds(1000); // setting angle to 45 (analog value = 1000)
  delay(1000);                     // delay of 1 second
  
  angle = myservo.read();         
  Serial.print("Angle at input 1000:- ");
  Serial.println(angle);          
  myservo.writeMicroseconds(1500); // setting angle to 90 (analog value = 1000)
  delay(1000);                    
  
  angle = myservo.read();          
  Serial.print("Angle at input 1500:- ");
  Serial.println(angle);           
  myservo.writeMicroseconds(3000); // setting angle to 180 (analog value = 3000 [any value greater than 2300])
  delay(1000);                    
  
  angle = myservo.read();         
  Serial.print("Angle at input 3000:- ");
  Serial.println(angle);           
  
} 
