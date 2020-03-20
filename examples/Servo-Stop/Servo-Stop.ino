#include <Servo.h>
#define trigPin  4
#define echoPin  2

Servo myservo;  // create servo object to control a servo
int pos = 0; // variable to store the servo position
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  // put your main code here, to run repeatedly:
  long duration, distance;
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
  digitalWrite(trigPin, LOW);

  Serial.print(distance);
  Serial.print("\n");

  if (distance < 17)
    myservo.write(0);   // tell servo to go to position 0
  else
    myservo.write(90);   // tell servo to go to position 90



}
