import time
from sense_hat import SenseHat


PRINT_INTERVAL = 500


class MotionDetector:
    MAX_DEGREE = 1
    MIN_DEGREE = -1

    def __init__(self):
        self.sense = SenseHat()
        self.x = 0
        self.y = 0
        self.z = 0
        self.orientation = 0
        self.acceleration = 0

    def update_parameters(self):
        acc = self.sense.get_accelerometer_raw()
        self.x = acc['x']
        self.y = acc['y']
        self.z = acc['z']
        
        x, y, z = self.x, self.y, self.z
        if z >= 0 and z <= 1 and x <= 1 and x >= -1 and y <= 1 and y >= -1:
            self.orientation = x 
            self.acceleration = -y 

    def get_parameters(self):
        return self.x, self.y, self.z, self.orientation, self.acceleration


if __name__ == '__main__':
    motion_detector = MotionDetector()
    t = time.time()

    while True:
        motion_detector.update_parameters()

        if (time.time() - t) * 1000 > PRINT_INTERVAL:
            x, y, z, orientation, acceleration = motion_detector.get_parameters()
            print('x = {}, y = {}, z = {}; orientation = {}, acceleration = {}'.format(x, y, z, orientation, acceleration))
            t = time.time()
