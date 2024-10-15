import hashlib
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
