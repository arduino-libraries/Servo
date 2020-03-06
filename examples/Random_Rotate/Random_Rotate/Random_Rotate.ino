/* Rotate
 by K.Abhijeet 
 This example code rotates the servo motor at random angle.

 Circuit:-
    Arduino Uno
    Micro Servo Motor connected at pin no 9

*/

#include<Servo.h>

Servo Smotor;           // Servo motor object 
 int pos=0;


void setup() {
     Serial.begin(9600);
     Smotor.attach(9);            //Servo Motor connected at pin no 9
      
     Smotor.write(0);
}

void loop() {

           pos= random(0,180);                           //calculates random angle
           Smotor.write(pos);
           Serial.print("Current Motor position is:");   
           Serial.println(pos);                         //prints motor angle
           delay(1000);                                 //Wait for 1 second
}
