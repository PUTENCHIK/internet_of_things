from Microcontroller import Microcontroller
from models.Config import Config


class Led(Microcontroller):
    command_up = Config.command_up_led
    command_down = Config.command_down_led

    def __init__(self, port: int):
        super().__init__(port)

    def up(self) -> int:
        return self.send_command(Led.command_up.command,
                                 Led.command_up.length)
    
    def down(self) -> int:
        return self.send_command(Led.command_down.command,
                                 Led.command_down.length)
    
    
