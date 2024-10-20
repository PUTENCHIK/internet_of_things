import time
import paho.mqtt.client as mqtt_client
from models.Config import Config
from models.Photo import Photo


def on_message(client, userdata, message):
    if message.topic == Config.controller_mode_topic:
        data = str(message.payload.decode("utf-8"))
        target, mode = data.split()
        if target == Photo.name or Config.all:
            try:
                connection_photo.set_mode(mode)
            except ValueError:
                print(f"Wrong mode for {Photo.name}")
            

broker = "broker.emqx.io"
pub_id = Config.get_client_id()
delay = 0.5

client = mqtt_client.Client(
   mqtt_client.CallbackAPIVersion.VERSION2,
   pub_id
)

client.on_message = on_message

port_photo = "/dev/ttyUSB0"
connection_photo = Photo(port_photo)

client.connect(broker)
client.loop_start()
print("Publishing starts")

try:
    while True:
        if connection_photo.is_delay_passed():
            value: int = connection_photo.current_mode()
            connection_photo.update_track()
            print("Value:", value)

except KeyboardInterrupt:
    pass
finally:
    client.disconnect()
    client.loop_stop()
