import hashlib
import serial
from uuid import getnode as get_mac

def get_broker_id():
    return "4586429059345734534763946934"


def our_hash(string):
    h = hashlib.new('sha256')
    h.update(str(string).encode())

    return h.hexdigest()[:20]


def get_id():
    mac = get_mac()
    id = our_hash(mac)

    return id


def get_topic():
    keyword = "DANNY IS AMOGUS"
    return our_hash(keyword)


def send_command(cmd: str,
                 response_len: int,
                 connection: serial.Serial
    ) -> int:
    str_resp: int = 0
    connection.write(cmd.encode())
    if response_len > 0:
        resp: bytes = connection.read(response_len)
        str_resp = int.from_bytes(resp, 'little')

    return str_resp


def config():
    obj = {
        "responses": {'d': 7,
                      'u': 6,
                      'p': 1,},
        "LIMIT_1023": 500,
    }
    obj["LIMIT_255"] = obj['LIMIT_1023'] // 4

    return obj