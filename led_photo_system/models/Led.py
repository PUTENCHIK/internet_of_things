from led_photo_system.models.Microcontroller import Microcontroller
from led_photo_system.models.Config import Config
from collections import deque
from led_photo_system.models.Mode import Mode


class Led(Microcontroller):
    name = "led"
    current_data_topic = "instant"
    command_up = Config.command_up_led
    command_down = Config.command_down_led

    # for threshold mode
    threshold_min = 1
    threshold_max = 1023

    # for average mode
    new_average = 1
    old_average = 1
    average_values = deque()
    len_average = 20

    def __init__(self, port: str):
        super().__init__(port)
        self.MODES = {
            'average': Mode('average', self.average),
            'threshold': Mode('threshold', self.threshold),
        }
        self.mode = 'average'

    def get_mode(self):
        return self.MODES[self.mode]

    def current_mode(self, photo_val_resp) -> str:
        return self.get_mode().func(photo_val_resp)

    def up(self) -> int:
        return self.send_command(Led.command_up.command,
                                 Led.command_up.length)
    
    def down(self) -> int:
        return self.send_command(Led.command_down.command,
                                 Led.command_down.length)

    def average(self, photo_val_resp) -> str:
        if len(self.average_values) != self.len_average:
            self.average_values.append(photo_val_resp)

            return 'wait'
        else:
            self.average_values.popleft()
            self.average_values.append(photo_val_resp)

            self.new_average = sum(self.average_values) / len(self.average_values)
            led_command = 'up' if self.new_average < self.old_average else 'down'

            self.old_average = self.new_average

            return led_command

    def threshold(self, photo_val_resp) -> str:
        buf = [self.threshold_min, self.threshold_max]

        return 'up' if photo_val_resp > sum(buf) / len(buf) else 'down'
    
