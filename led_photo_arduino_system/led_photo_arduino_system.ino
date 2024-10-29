#include "Config.h"
#include "WiFi.h"
#include "MQTT.h"


void setup() {
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    WIFI_init(false); // if true - micro is server
    MQTT_init();
}


void loop() {
    mqtt_cli.loop();

    if (mode == PUB_MODE) {
        mqtt_cli.publish("system/DANNY_IS_NEGRIK/message", "Hello!");
        delay(2000);    
    } else {
        mqtt_cli.subscribe("system/DANNY_IS_NEGRIK/message");
        delay(2000);
    }
    
}
