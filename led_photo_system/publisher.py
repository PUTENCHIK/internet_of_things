import serial
import paho.mqtt.client as mqtt_client
import time

broker = "broker.emqx.io"

# client = mqtt_client.Client('isu100123234235')
# FOR new version change ABOVE line to
client = mqtt_client.Client(
   mqtt_client.CallbackAPIVersion.VERSION1,
   'DANNY_DOT_COM_1001230034'
)

pub_id = "bc27bab771"

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
