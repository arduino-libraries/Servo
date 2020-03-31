##Example for sweeping a servo
##Connect Signal pin of the servo to pin D6


import pyb
import time

servo = pyb.Servo('D6')  #create a servo object attached at pin D6

while True:
    for pos in range(0, 180, 1): #pos changes from 0 to 179 degrees with step 1
        servo.angle(pos, 15) #moves servo to the value of pos in 15ms
#        print(pos)

    for pos in range(180, -1, -1): #pos changes from 180 to 0 degrees with step -1
        servo.angle(pos, 15) #moves servo to the value of pos in 15ms
#        print(pos)
