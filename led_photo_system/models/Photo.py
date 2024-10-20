from Microcontroller import Microcontroller
from Command import Command


class Photo(Microcontroller):
    command = Command('p', 1)

    def __init__(self, port: int):
        super().__init__(port)

    def get_value(self) -> int:
        return self.send_command(Photo.command.command,
                                 Photo.command.length)
