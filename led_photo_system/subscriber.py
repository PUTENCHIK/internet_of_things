import time
import paho.mqtt.client as mqtt_client
import serial
from common import get_id, get_topic, send_command, config
from collections import deque


def average(numbers):
    return sum(numbers) / len(numbers)


port_led = "COM5"

connection_led = serial.Serial(port_led, timeout=1)

config = config()

broker = "broker.emqx.io"
sub_id = get_id()

controller_topic = "led/controller"
threshold_min_topic = "lab/UNIQUE_ID/photo/min"
threshold_max_topic = "lab/UNIQUE_ID/photo/max"

current_sub_mode = "average"
current_pub_mode = "instant"

current_data_topic = "lab/UNIQUE_ID/photo/average"

len_average = 20
old_average = 1023
average_values = deque()
threshold_min = 0
threshold_max = 1023


def on_message(client, userdata, message):
    global current_sub_mode
    global current_pub_mode
    global current_data_topic
    global old_average
    global average_values
    global threshold_min
    global threshold_max

    data = str(message.payload.decode("utf-8"))
    # photo_val_resp = int(data)

    if message.topic == controller_topic:
        target, mode = data.split()

        if target == 'led' and current_sub_mode != mode:
            current_sub_mode = mode
            if current_sub_mode == "average":
                average_values = deque()
                old_average = 1023
            elif current_sub_mode == 'threshold':
                client.subscribe(threshold_min_topic)
                client.subscribe(threshold_max_topic)

        elif target == 'all' and current_pub_mode != mode:
            client.unsubscribe(current_data_topic)

            current_pub_mode = mode
            current_data_topic = "lab/UNIQUE_ID/photo/average"

            client.subscribe(current_data_topic)

    elif message.topic == threshold_min_topic:
        photo_val_resp = int(data)
        threshold_min = photo_val_resp

    elif message.topic == threshold_max_topic:
        photo_val_resp = int(data)
        threshold_max = photo_val_resp

    else:
        photo_val_resp = int(data)

        if current_sub_mode == "average":
            if average_values != len_average:
                average_values.append(photo_val_resp)
            else:
                average_values.popleft()
                average_values.append(photo_val_resp)

            new_average = average(average_values)
            led_command = 'up' if new_average < old_average else 'down'

            old_average = new_average

            send_command(config["responses"][led_command]['command'], config["responses"][led_command]['value'],
                         connection_led)

        elif current_sub_mode == "threshold":
            led_command = 'up' if photo_val_resp > average([threshold_min, threshold_max]) else 'down'

            send_command(config["responses"][led_command]['command'], config["responses"][led_command]['value'],
                         connection_led)


client = mqtt_client.Client(
    mqtt_client.CallbackAPIVersion.VERSION2,
    sub_id
)

client.on_message = on_message

try:
    print("Connecting to broker", broker)
    client.connect(broker)
    client.loop_start()
    print("Subcribing")
    client.subscribe(current_data_topic)
    client.subscribe(controller_topic)
    time.sleep(1800)
except KeyboardInterrupt:
    pass
finally:
    client.disconnect()
    client.loop_stop()
