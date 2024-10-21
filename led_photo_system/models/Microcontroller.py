import serial


class Microcontroller:
    def __init__(self, port: str):
        self.MODES = {}

        self.port = port
        self.connection = serial.Serial(self.port, timeout=1)
        self.mode = None

    def send_command(self, command: str, length: int) -> int:
        int_resp: int = 0
        self.connection.write(command.encode())
        if length > 0:
            response: bytes = self.connection.read(length)
            int_resp = int.from_bytes(response, 'little')

        return int_resp
    
    def set_mode(self, mode: str):
        if mode in self.MODES.keys():
            self.mode = mode
        else:
            raise ValueError(f"No mode {mode}, Possible modes: {self.MODES.keys()}")
