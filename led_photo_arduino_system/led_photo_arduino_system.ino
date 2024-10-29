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
            
            mqtt_cli.publish(const_cast<char*>((topic_template + topic).c_str()),
                             const_cast<char*>(String(value).c_str()));
            // mqtt_cli.publish("system/photo/message", "550");

            timer = millis();
        }
    } else {
        mqtt_cli.subscribe("system/DANNY_IS_NEGRIK/message");
        delay(2000);
    }
    
}
