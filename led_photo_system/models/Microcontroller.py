import serial


class Microcontroller:
    def __init__(self, port: str):
        self.port = port
        self.connection = serial.Serial(self.port, timeout=1)

    def send_command(self, command: str, length: int) -> int:
        int_resp: int = 0
        self.connection.write(command.encode())
        if length > 0:
            response: bytes = self.connection.read(length)
            int_resp = int.from_bytes(response, 'little')

        return int_resp
