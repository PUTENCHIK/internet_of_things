#include "Config.h"
#include "WiFi.h"
#include "MQTT.h"

void setup() {
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    WIFI_init(false); // if true - micro is server
    MQTT_init();

    Serial.println("I work!");
}


void loop() {
    mqtt_cli.loop();

    if (mode == PUB_MODE) {
        if (Serial.available()) {
            topic = Serial.readString();
            Serial.println(topic_template + topic);
        }
    } else {
        if (Serial.available()) {
            String cmd = Serial.readString();
            if (topic != cmd) {

                Serial.println(topic_template + cmd);
                mqtt_cli.unsubscribe((topic_template + topic).c_str());

                topic = cmd;
                mqtt_cli.subscribe((topic_template + topic).c_str());
            }
            
        }
    }
    
}
