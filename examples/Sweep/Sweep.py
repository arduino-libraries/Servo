#Sweep of Servo
# Using the Servo
# Make sure you have the Servo checkbox marked!

import machine
import pyb
import time

# The pyboard has four simple servo connections
servo = pyb.Servo(1)
while(1):
    for x in range(-90,91):
        servo.angle(x, 5000)
        time.sleep_ms(15)
    for x in range(90,-91, -1):
        servo.angle(x, 5000)
        time.sleep_ms(15)