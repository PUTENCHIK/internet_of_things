import time
import datetime
import paho.mqtt.client as mqtt_client
import random
import hashlib
from uuid import getnode as get_mac
import questionary

def our_hash(string):
    h = hashlib.new('sha256')
    h.update(str(string).encode())

    return h.hexdigest()[:20]

def get_client_id():
    mac = get_mac()
    id = our_hash(mac)

    return id


topic = questionary.text("What's topic").ask()

broker = "broker.emqx.io"

client = mqtt_client.Client(
   mqtt_client.CallbackAPIVersion.VERSION1,
   get_client_id()
)

print("Connecting to broker", broker)
print(client.connect(broker))
client.loop_start()
print("Publishing")

last_execution_time = time.monotonic()
state = ""
high_second_threshold = 40
low_second_threshold = 20
update_threshold = 25
last_change = "LOW"

while True:
    now = datetime.datetime.now()

    if now.second == low_second_threshold and state != "HIGH":
        client.publish(topic, 'HIGH')
        print("HIGH")
        state = "HIGH"
    elif now.second == high_second_threshold and state != "LOW":
        client.publish(topic, 'LOW')
        print("LOW")
        state = "LOW"

        if last_change == "LOW":
            high_second_threshold -= 1
            last_change = "HIGH"
        elif last_change == "HIGH":
            low_second_threshold += 1

            if low_second_threshold == 25:
                low_second_threshold = 20
                high_second_threshold = 40

            last_change = "LOW"

    else:
        continue

client.disconnect()
client.loop_stop()