import time
from models.Microcontroller import Microcontroller
from models.Config import Config
from models.Mode import Mode


class Photo(Microcontroller):
    name = "photo"
    command = Config.command_photo

    def __init__(self, port: int):
        super().__init__(port)
        self.MODES = {
            'instant': Mode('instant', self.instant),
            'average': Mode('average', self.stream),
            'stream': Mode('stream', self.stream, 0.5),
        }
        self.track = time.time()
        self.mode = 'instant'

    def get_mode(self):
        return self.MODES[self.mode]
    
    def get_value(self) -> int:
        return self.send_command(Photo.command.command,
                                 Photo.command.length)

    def is_delay_passed(self):
        return time.time() - self.track > self.get_mode().delay
    
    def update_track(self):
        self.track = time.time()

    def current_mode(self) -> int:
        return self.get_mode().func()

    def instant(self):
        value: int = self.send_command(self.command.command,
                                       self.command.length)
        return value

    def average(self):
        pass

    def stream(self):
        pass
