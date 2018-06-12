# LoRa Node-to-Node Communication

- `LoRaSender`
- `LoRaReceiver`
- `LoRaAllInOne`: for testing both sender & receiver on the same Arduino board

_Currently only `LoRaAllInOne` is tested. `LoRaReceiver` not implemented._

## Notes on the Modules

- Hardware
	- `MD0`, `MD1`
		- For setting the modes
	- `AUX`
		- Indicates the working mode of the module
		When sending/receiving data, `AUX` goes low first, then goes high when transmission finishes
- Software
	- Transmission packet
		- The packets start with a 4-byte destination address, followed by data bytes
		- Broadcasting has a destination address of `0xFF`
	- Modes
		- Normal: can send & receive data
		- Wakeup: can send & receive data; can send data to modules in save mode
		- Save (power): cannot send data; can receive data from modules in wakeup mode
		- Sleep: for writing module settings

## Issues

###### Wakeup Mode

Normal mode behaves correctly, but wakeup mode troublesome.

1. Data not the same as in normal mode, which are all ASCII representations
	1. A -> 5, B -> 9, C -> 13, ...
	2. a -> 133, b -> 137, c -> 141, ...
	3. 0 -> 130, 1 -> 138, 2 -> 146, ...
2. Eats up the beginning 59 characters, as found by experiments
3. Transmission always ends with an extra 255 (letters) or 254 (digital numbers, punctuations)

_Normal mode, before sending:_
![](https://github.com/HanoiWaterbots/arduino/blob/master/LoRa/imgs/Normal_before_send.png?raw=true)
_Normal mode, after sending:_
![](https://github.com/HanoiWaterbots/arduino/blob/master/LoRa/imgs/Normal_after_send.png?raw=true)
_Wakeup mode, before sending:_
![](https://github.com/HanoiWaterbots/arduino/blob/master/LoRa/imgs/Wakeup_before_send.png?raw=true)
_Wakeup mode, after sending:_
![](https://github.com/HanoiWaterbots/arduino/blob/master/LoRa/imgs/Wakeup_after_send.png?raw=true)
