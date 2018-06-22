# Bot Control

## Motion Detector API

```python
from motion_detector import MotionDetector


motion_detector = MotionDetector()
motion_detector.update_parameters() # Constantly updating is suggested
x, y, z, orientation, acceleration = motion_detector.get_parameters()
```    

## LED API

```python
import time
from LED import LED


led = LED()

led.set_left_motor(1600)
led.set_right_motor(1700)

led.show()
time.sleep(1) # Shed for 1 second
led.clear()
```

## Axes

1. `x`: face right = `1`, face left = `-1` 
2. `y`: face inwards = `1`, face outwards = `-1` 
3. `z`: face up = `1`, face down = `-1`
