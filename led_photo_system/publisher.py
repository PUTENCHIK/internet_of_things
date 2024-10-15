import serial
import paho.mqtt.client as mqtt_client
import time
from common import get_id, get_topic


broker = "broker.emqx.io"
pub_id = get_id()
topic = get_topic()

client = mqtt_client.Client(
   mqtt_client.CallbackAPIVersion.VERSION2,
   pub_id
)

print("Connecting to broker", broker)
print(client.connect(broker))
client.loop_start()
print("Publishing")

try:
    while True:
        state = "Danny Shelby: "
        inp = input()
        state += inp
        print(f"State is {state}")
        client.publish(f"lab/{pub_id}/led/state", state)
except KeyboardInterrupt:
    pass
finally:
    client.disconnect()
    client.loop_stop()
