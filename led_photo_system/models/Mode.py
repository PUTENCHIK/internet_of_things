from models.Config import Config


class Mode:
    def __init__(self, name: str, func, delay: float = 1):
        self.name = name
        self.func = func
        self.delay = delay

    def get_topic(self):
        return Config.common_topic + self.name