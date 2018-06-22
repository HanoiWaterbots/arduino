import serial
import sys
import pygame
import time

SERIAL_PORT = sys.argv[1] 
SERIAL_RATE = sys.argv[2]

MIN_L = 1600
MIN_R = 1600

STOP_R = 1550
STOP_L = 1550

MAX_L = 2000
MAX_R = 2000

LM = MIN_L
RM = MIN_R
ACC = 1        #Accelerate Unit

W_KEY = False
A_KEY = False
S_KEY = False
D_KEY = False
Q_KEY = False

TIME_INTERVAL = 1   #In seconds
print("Done")


s = serial.Serial(SERIAL_PORT, SERIAL_RATE)
pygame.init()
pygame.font.init()
myfont = pygame.font.SysFont('Comic Sans MS', 30)

pygame.display.set_caption('Remote Control')
size = [640, 480]
screen = pygame.display.set_mode(size)
white = (255, 255, 255)
screen.fill(white)

#Arrow Image
arrow = pygame.image.load('arrow.png')


def moveLeft():
    global MIN_L, MIN_R, MAX_L, MAX_R, LM, RM, ACC
    
    if RM + ACC >= MAX_R:
        if(LM >= MIN_L):
            LM = LM - ACC
    else:
        RM = RM + ACC


def moveRight():
    global MIN_L, MIN_R, MAX_L, MAX_R, LM, RM, ACC
    
    if LM + ACC >= MAX_L:
        if RM>=MIN_R:
            RM = RM - ACC
    else:
        LM = LM + ACC

def speedUp():
    global MIN_L, MIN_R, MAX_L, MAX_R, LM, RM, ACC
    
    if LM + ACC <=MAX_L:
        LM = LM + ACC
    if RM + ACC <= MAX_R:
        RM = RM + ACC

def slowDown():
    global MIN_L, MIN_R, MAX_L, MAX_R, LM, RM, ACC

    if LM - ACC >= MIN_L:
        LM = LM - ACC
    if RM - ACC >= MIN_R:
        RM = RM - ACC

def keyPressed(key, status):
    global W_KEY, A_KEY, S_KEY, D_KEY, Q_KEY, STOP_L, STOP_R, LM, RM

    if key == 119: #W
        W_KEY = status
    elif key == 97:
        A_KEY = status
    elif key == 115:
        S_KEY = status
    elif key == 100:
        D_KEY = status
    if key == 113:
        if LM >= STOP_L or RM >= STOP_R:
            LM = STOP_L
            RM = STOP_R

def updateDisplay():
    #Center of old arrow
    arrow_rect_center = screen.get_rect().center

    #Calculate Angle
    angle = ((RM - LM )/(MAX_R - MIN_L))*90

    #New arrow
    new_arrow = pygame.transform.rotate(arrow, angle)
    new_rect = new_arrow.get_rect(center=arrow_rect_center)

    return new_arrow, new_rect

def doCommands():
    global W_KEY, A_KEY, S_KEY, D_KEY
    if(W_KEY):
        speedUp()
    if(A_KEY):
        moveLeft()
    if(S_KEY):
        slowDown()
    if(D_KEY):
        moveRight()

def main():
    leftMotorSpeed = myfont.render('Left Motor' + str(LM), False, (0, 0, 0))
    rightMotorSpeed = myfont.render('Right Motor' + str(RM), False, (0, 0, 0))

    screen.blit(arrow, arrow.get_rect(center=screen.get_rect().center))
    screen.blit(leftMotorSpeed, (0,0))
    screen.blit(rightMotorSpeed, (30,0))
    
    pygame.display.update()

    #Start Input
    t = time.time()
    while(True):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                exit()
            elif event.type == pygame.KEYDOWN:
                keyPressed(event.key, True)
            
            elif event.type == pygame.KEYUP:
                keyPressed(event.key, False)

        #Do the commands after the events above
        doCommands()
        a,b = updateDisplay()
        screen.fill(white)
        screen.blit(a, b)
        leftMotorSpeed = myfont.render('Left Motor' + str(LM), False, (0, 0, 0))
        rightMotorSpeed = myfont.render('Right Motor' + str(RM), False, (0, 0, 0))
        screen.blit(leftMotorSpeed, (50,50))
        screen.blit(rightMotorSpeed, (450,50))
        
        pygame.display.update()
        if(time.time() - t >= TIME_INTERVAL):
            s.write(('b' + str(LM) + ";" +  str(RM)).encode())
            t = time.time()

if __name__ == '__main__':
    main()