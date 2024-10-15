import serial
import paho.mqtt.client as mqtt_client
from common import (
    get_id,
    get_topic,
    send_command,
    config
)


broker = "broker.emqx.io"
pub_id = get_id()
topic = get_topic()

client = mqtt_client.Client(
   mqtt_client.CallbackAPIVersion.VERSION2,
   pub_id
)

port_photo = "COM4"
responses = config()['responses']
command = responses['photo']['command']
command_value = responses['photo']['value']
connection_photo = serial.Serial(port_photo, timeout=1)

print("Connecting to broker:", broker)
print(client.connect(broker))
client.loop_start()
print("Publishing starts")

try:
    while True:
        photo_val_resp: int = send_command(command,
                                           command_value,
                                           connection_photo)

        # print(f"Value: {photo_val_resp}")
        client.publish(topic, photo_val_resp)
except KeyboardInterrupt:
    pass
finally:
    client.disconnect()
    client.loop_stop()
