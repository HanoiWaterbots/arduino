import sys
from motion_detector import MotionDetector
from sense_hat import SenseHat
from LED import LED 
import time

SERIAL_PORT = sys.argv[1] 
SERIAL_RATE = sys.argv[2]

MIN_L = 1600
MIN_R = 1600

STOP_R = 1550
STOP_L = 1550

MAX_L = 2000
MAX_R = 2000

LM = STOP_L
RM = STOP_R

MAX_ACCL = 5

sense = SenseHat()
led = LED()

def accelerationToMotors(acceleration, LM, RM):
    global MIN_L, MIN_R, MAX_L, MAX_R, MAX_ACCL
    #Detect tilt acceleration
    if acceleration > 0.25:
        LM = min(LM + acceleration*(MAX_ACCL), MAX_L)
        RM = min(RM + acceleration*(MAX_ACCL), MAX_R)
    elif acceleration< -0.3:
        LM = max(LM + acceleration*(MAX_ACCL)*10, MIN_L)
        RM = max(RM + acceleration*(MAX_ACCL)*10, MIN_R)

    return LM, RM

def orientationToMotors(orientation, LM, RM):
    if orientation >= 0.2 :
        #Turn rigt
        if LM <= MAX_L:
            LM = LM + (MAX_L - LM)*(orientation*orientation)
        
    elif orientation < -0.2:
        #Turn left
        if RM <= MAX_R:
            RM = RM + (MAX_R - RM)*((orientation*orientation))

    else:
        LM = min(LM, RM)
        RM = min(LM, RM)

    return LM, RM

def sendSignal(LM, RM):
    print(int(LM),int(RM))

def main():
    global LM, RM, led
    m = MotionDetector()
    t = time.time()
    while(True):
        #Read the tilt and orientation from the other program
        m.update_parameters()
        x, y, z, orientation, acceleration = m.get_parameters()

        #Change the orientation and acceleration to motor speeds
        LM, RM = accelerationToMotors(acceleration, LM, RM)
        LM, RM = orientationToMotors(orientation, LM, RM)
       
        for event in sense.stick.get_events():
            if event.direction == 'middle' and event.action=='pressed':
                LM,RM = STOP_L, STOP_R
  
        led.set_left_motor(LM)
        led.set_right_motor(RM)
        led.show()

        if time.time() - t > 1: 
            sendSignal(LM, RM)
            t = time.time()



if __name__=='__main__':
    main()
