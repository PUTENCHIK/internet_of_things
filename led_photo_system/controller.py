import paho.mqtt.client as mqtt_client
from models.Config import Config
from secrets import token_hex


broker = "broker.emqx.io"
con_id = Config.get_client_id() + token_hex(2)
controller_topic = Config.controller_mode_topic

client = mqtt_client.Client(
   mqtt_client.CallbackAPIVersion.VERSION2,
   con_id
)

print(con_id)


client.connect(broker)
client.loop_start()
print("Controller publishing starts")

try:
    while True:
        command = input("Input (<target> <mode>): ")

        if len(command.split()) == 2:
            print(f"Command: {command}")
            client.publish(controller_topic, command)
        else:
            print(f"Wrong command format")

except KeyboardInterrupt:
    pass
finally:
    client.disconnect()
    client.loop_stop()
