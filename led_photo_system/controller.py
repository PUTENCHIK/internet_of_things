import paho.mqtt.client as mqtt_client
from common import (
    get_id,
)


broker = "broker.emqx.io"
pub_id = get_id()
controller_topic = "led/controller"

client = mqtt_client.Client(
   mqtt_client.CallbackAPIVersion.VERSION2,
   pub_id
)


print(client.connect(broker))
client.loop_start()
print("Controller publishing starts")

try:
    while True:
        command = input()

        print(f"Value: {command}")
        client.publish(controller_topic, command)

except KeyboardInterrupt:
    pass
finally:
    client.disconnect()
    client.loop_stop()
