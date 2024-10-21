import time
from models.Microcontroller import Microcontroller
from models.Config import Config
from models.Mode import Mode
from collections import deque


class Photo(Microcontroller):
    name = "photo"
    command = Config.command_photo
    average_amount = 100
    default_mode = 'instant'

    default_min = 10**5
    default_max = 0

    def __init__(self, port: int):
        super().__init__(port)
        self.MODES = {
            'instant': Mode('instant', self.instant),
            'average': Mode('average', self.average),
            'stream': Mode('stream', self.stream, 0.2),
        }
        self.track = time.time()
        self.track_mim_max = time.time()
        self.deque = deque()

        self.mode = Photo.default_mode
        self.min = Photo.default_min
        self.max = Photo.default_max

    def get_mode(self):
        return self.MODES[self.mode]
    
    def set_mode(self, mode):
        super().set_mode(mode)
        self.min = Photo.default_min
        self.max = Photo.default_max
    
    def get_topic(self):
        return self.get_mode().get_topic()
    
    def get_value(self) -> int:
        return self.send_command(Photo.command.command,
                                 Photo.command.length)

    def is_delay_passed(self):
        return time.time() - self.track > self.get_mode().delay
    
    def update_track(self):
        self.track = time.time()

    def update_min_max(self, value: int):
        if value > self.max:
            self.max = value
        elif value < self.min:
            self.min = value

    def current_mode(self) -> tuple:
        value = self.get_mode().func()

        self.update_min_max(value)

        return self.min, self.max, value

    def instant(self) -> int:
        return self.get_value()

    def average(self) -> int:
        print(self.deque)
        if len(self.deque) == 0:
            while len(self.deque) < Photo.average_amount:
                self.deque += [self.get_value()]
        else:
            self.deque.popleft()
            self.deque += [self.get_value()]

        return int(sum(self.deque) / len(self.deque))

    def stream(self) -> int:
        return self.get_value()
