import hashlib
from uuid import getnode as get_mac

from models.Command import Command


class Config:
    @staticmethod
    def our_hash(string):
        h = hashlib.new('sha256')
        h.update(str(string).encode())

        return h.hexdigest()[:20]

    @staticmethod
    def get_client_id():
        mac = get_mac()
        id = Config.our_hash(mac)

        return id

    @staticmethod
    def get_topic(mode):
        return Config.common_topic + mode


    all = "all"
    command_photo = Command('p', 1)
    command_up_led = Command('u', 6)
    command_down_led = Command('d', 7)

    LIMIT_1023 = 500
    LIMIT_255 = LIMIT_1023 // 4

    isHardcode = True
    unique_id = our_hash("DANNY IS AMOGUS") if isHardcode else None
    common_topic = f"led/{unique_id}/photo/"

    # Topic for setting mode to Led or Photo from controller.py
    controller_mode_topic = f"led/{unique_id}/controller/mode"
    threshold_min_topic = f"led/{unique_id}/photo/min"
    threshold_max_topic = f"led/{unique_id}/photo/max"
