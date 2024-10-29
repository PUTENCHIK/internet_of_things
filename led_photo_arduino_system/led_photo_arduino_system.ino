#include "Config.h"
#include "WiFi.h"
#include "MQTT.h"


int value;
unsigned long timer = millis();


void setup() {
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    WIFI_init(false);
    MQTT_init();

    Serial.println("I work!");
}


void loop() {
    mqtt_cli.loop();

    if (mode == PUB_MODE) {
        if (Serial.available()) {
            topic = Serial.readString();
            Serial.println("New topic: " + topic_template + topic);
            timer -= PUB_DELAY;
        }

        if (millis() - timer > PUB_DELAY) {
            value = analogRead(PHOTO_PIN);
            Serial.println("Value: " + String(value));

            String full_topic = topic_template + topic;
            mqtt_cli.publish(full_topic.c_str(), String(value).c_str());

            timer = millis();
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
