import time
import paho.mqtt.client as mqtt_client
from led_photo_system.models.Config import Config
from led_photo_system.models.Led import Led


config = Config()

broker = "broker.emqx.io"
sub_id = config.get_client_id()

port_led = "COM5"

connection_led = Led(port_led)
controller_mode_topic = config.controller_mode_topic

connection_led.current_data_topic = config.get_topic('instant')


def on_message(client, userdata, message):
    data = str(message.payload.decode("utf-8"))

    print("Топик:", message.topic)
    print("Сообщение:", data)

    if message.topic == config.controller_mode_topic:
        data = str(message.payload.decode("utf-8"))
        target, mode = data.split()

        if target == Led.name:
            if mode == 'threshold':
                client.subscribe(config.threshold_min_topic)
                client.subscribe(config.threshold_max_topic)
            elif mode == 'average':
                client.unsubscribe(config.threshold_min_topic)
                client.unsubscribe(config.threshold_max_topic)

            try:
                connection_led.set_mode(mode)
            except ValueError:
                print(f"Wrong mode for {Led.name}")

        elif target == config.all and connection_led.current_data_topic != mode:
            client.unsubscribe(connection_led.current_data_topic)

            connection_led.current_data_topic = config.get_topic(mode)

            client.subscribe(connection_led.current_data_topic)

    elif message.topic == config.threshold_min_topic:
        photo_val_resp = int(data)
        connection_led.threshold_min = photo_val_resp

    elif message.topic == config.threshold_max_topic:
        photo_val_resp = int(data)
        connection_led.threshold_max = photo_val_resp

    else:
        photo_val_resp = int(data)

        led_command: str = connection_led.current_mode(photo_val_resp)

        print("Состояние:", led_command)

        if led_command == 'up':
            connection_led.send_command(config.command_up_led.command, config.command_up_led.length)
        elif led_command == 'down':
            connection_led.send_command(config.command_down_led.command, config.command_down_led.length)
        elif led_command == 'wait':
            return


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
    client.subscribe(connection_led.current_data_topic)
    client.subscribe(controller_mode_topic)
    time.sleep(1800)
except KeyboardInterrupt:
    pass
finally:
    client.disconnect()
    client.loop_stop()
