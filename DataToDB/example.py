import sys
import requests
import serial


SERIAL_PORT = sys.argv[1] 
SERIAL_RATE = sys.argv[2]
DB_URL = 'http://ec2-18-205-17-12.compute-1.amazonaws.com:3000/dbhandler/data'


def send_db(data):
    print('Sending: ' + str(data))
    r = requests.post(DB_URL, json=data)
    print('Response: ' + str(r.json()))

def main():
    ser = serial.Serial(SERIAL_PORT, SERIAL_RATE)

    is_started = False
    serial_data = dict()
    while True:
        data = ser.readline().decode('utf-8').rstrip()
        if not is_started and data == 'start': # start a batch of data
            is_started = True
        elif is_started and data == 'end': # end a batch of data, send to db
            send_db(serial_data)
            is_started = False
            serial_data = dict()
        elif is_started: # collect data
            key, value = data.split(':')
            serial_data[key] = float(value) # assume only number data

if __name__ == "__main__":
    main()
