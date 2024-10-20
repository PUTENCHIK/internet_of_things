from Microcontroller import Microcontroller
from Command import Command


class Led(Microcontroller):
    command_up = Command('u', 6)
    command_down = Command('d', 7)

    def __init__(self, port: int):
        super().__init__(port)

    def up(self) -> int:
        return self.send_command(Led.command_up.command,
                                 Led.command_up.length)
    
    def down(self) -> int:
        return self.send_command(Led.command_down.command,
                                 Led.command_down.length)
    
    
