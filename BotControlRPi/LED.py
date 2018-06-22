import sys
import time
import math
from sense_hat import SenseHat


class LED:
    L = [255, 0, 0]
    R = [0, 255, 0]
    O = [0, 0, 0]
    STOP_MOTOR = 1550
    MIN_MOTOR = 1600
    MAX_MOTOR = 2000
    MATRIX_ROWS = 8
    MATRIX_COLS = 8
    LEFT = 0
    RIGHT = 1

    def __init__(self):
        self.sense = SenseHat()
        self.led_matrix = None
        self.reset()

    def reset(self):
        self.led_matrix = [[self.O for _ in range(self.MATRIX_COLS)] for _ in range(self.MATRIX_ROWS)]

    def reset_half(self, orientation):
        O = self.O
        half_cols = math.floor(self.MATRIX_COLS / 2)
        if orientation == self.LEFT:
            for i in range(self.MATRIX_ROWS):
                for j in range(half_cols):
                    self.led_matrix[i][j] = O
        elif orientation == self.RIGHT:
            for i in range(self.MATRIX_ROWS):
                for j in range(half_cols, self.MATRIX_COLS):
                    self.led_matrix[i][j] = O

    def get_lightup_info(self, value):
        MIN_MOTOR, MAX_MOTOR, STOP_MOTOR, MATRIX_ROWS, MATRIX_COLS = self.MIN_MOTOR, self.MAX_MOTOR, self.STOP_MOTOR, self.MATRIX_ROWS, math.floor(self.MATRIX_COLS / 2)
        MOTOR_RANGE = MAX_MOTOR - MIN_MOTOR

        value = STOP_MOTOR if value < STOP_MOTOR else (MAX_MOTOR if value > MAX_MOTOR else value)
        amount = value - MIN_MOTOR

        # No lights when at STOP_MOTOR value
        if value >= STOP_MOTOR and value < MIN_MOTOR:
            return 0, 0 # No rows/cols lighting up

        # Ensure at least one light is on for MIN_MOTOR value
        if amount == 0:
            amount = math.ceil((MAX_MOTOR - MIN_MOTOR) / (MATRIX_ROWS * MATRIX_COLS))

        lightup_rows = math.floor(amount / (MOTOR_RANGE / MATRIX_ROWS))
        lightup_cols = math.floor((amount % (MOTOR_RANGE / MATRIX_ROWS)) / ((MOTOR_RANGE / MATRIX_ROWS) / MATRIX_COLS))

        return lightup_rows, lightup_cols

    def set_left_motor(self, value):
        lightup_rows, lightup_cols = self.get_lightup_info(value)

        self.reset_half(self.LEFT)

        # All lights up except the last few rows
        half_cols = math.floor(self.MATRIX_COLS / 2)
        for ti in range(lightup_rows):
            i = self.MATRIX_ROWS - 1 - ti
            for j in range(half_cols):
                self.led_matrix[i][j] = self.L

        # Some lights up in the last row
        for j in range(lightup_cols):
            i = self.MATRIX_ROWS - 1 - lightup_rows
            self.led_matrix[i][j] = self.L

    def set_right_motor(self, value):
        lightup_rows, lightup_cols = self.get_lightup_info(value)

        self.reset_half(self.RIGHT)

        # All lights up except the last few rows
        half_cols = math.floor(self.MATRIX_COLS / 2)
        for ti in range(lightup_rows):
            i = self.MATRIX_ROWS - 1 - ti
            for j in range(half_cols, self.MATRIX_COLS):
                self.led_matrix[i][j] = self.R

        # Some lights up in the last row
        for j in range(half_cols, half_cols + lightup_cols):
            i = self.MATRIX_ROWS - 1 - lightup_rows
            self.led_matrix[i][j] = self.R

    def show(self):
        matrix = [l for row in self.led_matrix for l in row]
        self.sense.set_pixels(matrix)

    def clear(self):
        self.sense.clear()

if __name__ == '__main__':
    left_motor, right_motor = int(sys.argv[1]), int(sys.argv[2])

    led = LED()

    led.set_left_motor(left_motor)
    led.set_right_motor(right_motor)

    led.show()
    time.sleep(1)
    led.clear()
