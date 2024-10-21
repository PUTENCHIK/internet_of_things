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
            start = time.time()
            mmin, mmax, value = connection_photo.current_mode()
            print("time:", time.time() - start)
            connection_photo.update_track()
            print("Value:", value)
            client.publish(connection_photo.get_topic(), value)

            print(f"(min, max) = ({mmin}, {mmax})")
            client.publish(Config.threshold_min_topic, mmin)
            client.publish(Config.threshold_max_topic, mmax)
            print()

except KeyboardInterrupt:
    pass
finally:
    client.disconnect()
    client.loop_stop()
