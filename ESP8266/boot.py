import urequests
import ujson
import machine
from machine import UART
import os
import sys
import uos
import network
import machine
import gc
gc.collect()

#Connect to the WiFi Network
def do_connect():
    sta_if = network.WLAN(network.STA_IF)
    sta_if.active(True)
    if not sta_if.isconnected():
        sta_if.active(True)
        
        #Provide the wifi SSID and password here
        sta_if.connect('Backpakerf3', '25trungyen')
        
        while not sta_if.isconnected():
            pass

#Send the sensor data to the API Server
def sendData(x):
    urequests.post('http://ec2-18-205-17-12.compute-1.amazonaws.com:3000/dbhandler/data', json=x)

#Main function
def main():
    do_connect()

    uart=UART(0,115200)
    x = ""
    #Until the ESP runs, the serial input to the ESP will be processed to be sent to the server
    while(True):
        #Chunks of Serial data in bytes
        chunk = uart.readline()
        if chunk is not None :
            #Convert the binary data to string
            chunk = chunk.decode('utf-8')

            #Combine chunks of data together
            x = x + chunk
            #One data entry ends after the newline character
            if x.find('\n') >= 0:
                #Parse string to JSON format
                x = ujson.loads(x)
                sendData(x)
                #Reset for the next entry
                x = ""

main()
