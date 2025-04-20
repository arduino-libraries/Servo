# ServoStop

## Sketch Objective

Set servo position to 0 when the ultrasonic sensor detects any object closer
than 17cm. Otherwise, set servo position to 90.
by Madhur Dixit https://github.com/Chester-King

## Sketch Details and Working

**Connections:**

- Trig Pin on Ultrasonic sensor to GPIO 3 on Arduino
- Echo Pin on Ultrasonic sensor to GPIO 2 on Arduino
- Servo to GPIO 4 on Arduino
- Connect the Vcc of the Ultrasonic and Servo to 5V of Arduino
- Connect the Gnd of the Ultrasonic and Servo to Gnd of Arduino
- Upload the code to Arduino

## Result

Whenever any Object comes in 17cm Proximity of the Ultrasonic sensor the servo will set to the position 0 until the object leaves the ultrasonic sensor proximity.

## Required Circuit

You can see the required Circuit [here](https://github.com/arduino-libraries/Servo/blob/master/examples/ServoStop/ServoStop.png)
