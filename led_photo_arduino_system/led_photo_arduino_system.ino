#include "Config.h"
#include "WiFi.h"
#include "MQTT.h"


int value;
unsigned long timer = millis();
char tab2[1024];


void setup() {
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    WIFI_init(false);
    MQTT_init();
}


void loop() {
    mqtt_cli.loop();
//    mqtt_cli.publish("system/photo/message", "hello!");
//    delay(1900);

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
        mqtt_cli.subscribe("system/DANNY_IS_NEGRIK/message");
        delay(2000);
    }
    
}
