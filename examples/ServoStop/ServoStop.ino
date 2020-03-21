#include <Servo.h>
byte trigPin = 4;
byte echoPin = 2;
byte servoPin = 9;

Servo myservo;  // create servo object to control a servo
int pos = 0; // variable to store the servo position
void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myservo.attach(servoPin);  // attaches the Servo on pin 9 to the servo object
}

void loop() {
  unsigned long duration;
  unsigned int distance;
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance < 17) {
    myservo.write(0);   // tell servo to go to position 0
  } else {
    myservo.write(90);   // tell servo to go to position 90
  }
}
