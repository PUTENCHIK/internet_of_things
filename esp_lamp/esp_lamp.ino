#include "Config.h"
#include "WiFi.h"
#include "Server.h"


void setup() {
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    WIFI_init(true); // if true - micro is server
    server_init();
}


void loop() {
    server.handleClient();
}
