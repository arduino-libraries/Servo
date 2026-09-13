#This is a micropython example for controlling the servo with a potentiometer.
#Potentiometer is referred as pot here.
#Please connect the pot with pin 0
#Please connect the pot with pin 1

import machine
import pyb
import time

pot = machine.ADC(0)
servo = pyb.Servo(1)


#Map function similar to the one in arduino
def map(value, range1_lb, range1_ub, range2_lb, range2_ub):
  return ((value - range1_lb) * (range2_ub - range2_lb) / (range1_ub - range1_lb) + range2_lb )

while(True):
  value = pot.read()
  value = int(map(value, 0, 1024, -90, 90))
  servo.angle(value, 1000)
  time.sleep(1)