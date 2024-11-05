#include "Config.h"
#include "WiFi.h"
#include "Server.h"


void setup() {
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    WIFI_init(true);
    server_init();
}

// Равномерная индикация (при включенной собственной точке доступа или при доступе только в интернет)
void ownApIndication(int ddelay) {
  if (millis() > timer + ddelay) {
    led_stage = led_stage == 2 ? 1 : 2;
    isStageChanged = true;
    timer = millis();
  }
  if (isStageChanged) {
    isStageChanged = false;
    if (led_stage == 1) {
      digitalWrite(LED_PIN, LOW);       // on led
    } else {
      digitalWrite(LED_PIN, HIGH);      // off led
    }
  }
}

// Индикация при доступе в интернет и поднятом MQTT
void intMqttIndication() {
  int ddelay = led_stage < 4 ? short_led_delay : long_led_delay;
  if (millis() > timer + ddelay) {
    led_stage = led_stage == 4 ? 1 : led_stage+1;
    isStageChanged = true;
    timer = millis();
  }
  if (isStageChanged) {
    isStageChanged = false;
    if (led_stage%2 == 1) {
      digitalWrite(LED_PIN, LOW);       // on led
    } else {
      digitalWrite(LED_PIN, HIGH);      // off led
    }
  }
}

// Функция индикации светодиода
void led_indication() {
  switch (state) {
    case own_ap_state:
      ownApIndication(long_led_delay);
      break;
    case int_mqtt_state:
      intMqttIndication();
      break;
    case only_int_state:
      ownApIndication(short_led_delay);
      break;
  }
}

void loop() {
    server.handleClient();
    led_indication();
}
