# Serial Data to Database

## Usage

1. Install [`pySerial`](https://pythonhosted.org/pyserial/index.html): `$ pip3 install pyserial`
2. Find serial port
    1. MacOS: `$ ls /dev/tty.*`
3. Run with port name & baudrate: `$ python3 example.py <port-name> <baudrate>`
    1. e.g. `python3 example.py /dev/tty.usbmodem1411 9600`

## Example

![](https://github.com/HanoiWaterbots/arduino/blob/master/SerialDataToDB/example.png?raw=true)
