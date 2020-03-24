//Load in the required main file
Servo = require('../../src/JerryScript/main')

//Define the servo Pin here
const Servopin = 11

//Define the potentiometer Pin here
const dataPin = A0

//sweep the servo 
setInterval(() => {
    var data = analogRead(dataPin) //This returns a value in between 0 & 1
    //send the data value to set the servo arm
    Servo.setServo(Servopin, data)
}, 10); // hault for 10 milli second before next reading

