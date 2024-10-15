import time
import paho.mqtt.client as mqtt_client
import serial
from common import get_id, get_topic, send_command, config

port_led = "COM5"

connection_led = serial.Serial(port_led, timeout=1)

config = config()


broker= "broker.emqx.io"
sub_id = get_id()
topic = get_topic()


def on_message(client, userdata, message):
    data = str(message.payload.decode("utf-8"))
    photo_val_resp = int(data)

    print(photo_val_resp)

    led_command = 'up' if photo_val_resp > config["LIMIT_255"] else 'down'
    send_command(config["responses"][led_command]['command'], config["responses"][led_command]['value'], connection_led)



client = mqtt_client.Client(
   mqtt_client.CallbackAPIVersion.VERSION1,
   sub_id
)


client.on_message=on_message


try:
    print("Connecting to broker", broker)
    client.connect(broker)
    client.loop_start()
    print("Subcribing")
    client.subscribe(topic)
    time.sleep(1800)
except KeyboardInterrupt:
    pass
finally:
    client.disconnect()
    client.loop_stop()
