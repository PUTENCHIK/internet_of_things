import machine
import time

class RobotController:
    def __init__(self, uart_id=1, baudrate=9600):
        self.uart = machine.UART(uart_id, baudrate)
        print("RobotController initialized")

    def forward(self):
        self.uart.write('f')
        print("Forward command sent")

    def backward(self):
        self.uart.write('b')
        print("Backward command sent")

    def turn_left(self):
        self.uart.write('l')
        print("Turn Left command sent")

    def turn_right(self):
        self.uart.write('r')
        print("Turn Right command sent")

    def rotate_left(self):
        self.uart.write('q')
        print("Rotate Left command sent")

    def rotate_right(self):
        self.uart.write('e')
        print("Rotate Right command sent")

    def stop(self):
        self.uart.write('s')
        print("Stop command sent")

