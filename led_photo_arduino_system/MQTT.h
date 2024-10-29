#include <PubSubClient.h>

PubSubClient mqtt_cli(wifiClient);

long long convertToNumber(byte *payload, int length) {
    long long result = 0;
    for (int i = 0; i < length; ++i) {
        result = result * 10 + (payload[i] - '0');
    }
    return result;
}


void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");

    long long value = convertToNumber(payload, length);

    if (value > threshold) {
        digitalWrite(LED_PIN, LOW);
        Serial.println("Led is high");
    } else {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("Led is low mmr");
    }

    Serial.println();
    Serial.println("-----------------------");
}

void MQTT_init(){
    mqtt_cli.setServer(mqtt_broker, mqtt_port);
    mqtt_cli.setCallback(callback);
    while (!mqtt_cli.connected()) {
        String client_id = "esp8266-" + String(WiFi.macAddress());
//        Serial.print("The client " + client_id);
//        Serial.println(" connects to the public mqtt broker\n");
        if (mqtt_cli.connect(client_id.c_str())){
            Serial.println("MQTT Connected");
            delay(1000);
        } else {
            Serial.print("failed with state ");
            Serial.println(mqtt_cli.state());
            delay(2000);
        }
    }  
}
