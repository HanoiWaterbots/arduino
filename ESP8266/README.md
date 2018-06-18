#ESP8266 for Real Time Data Transmission
1. Firstly, install all the requirements in the virtual environment
   `pip install -r requirements.txt`

2. Now, to flash the ESP8266 with Micropython, firstly erase the flash and then flash the new one.
   `esptool.py --port /dev/ttyUSB0 erase_flash`
   `esptool.py --port /dev/ttyUSB0 --baud 460800 write_flash --flash_size=detect 0 esp8266-20180511-v1.9.4.bin`

3. Now use ampy to put boot.py and main.py to the ESP8266
   `ampy -p /dev/ttyUSB0 put boot.py main.py`

Now when we press the reset button, it must start the program in boot.py