#include "Config.h"
#include <SoftwareSerial.h>

SoftwareSerial BTSerial(MCE_RX, MCE_TX);

int current_mode = 1;

void setup() {
    Serial.begin(9600);
    while (!Serial) { }
    Serial.println("Hello from PC!");

    BTSerial.begin(9600);
    BTSerial.println("Hello, from phone!");

    for (int i = 0; i < PINS_AMOUNT; i++) {
        pinMode(PINS[i], OUTPUT);
        digitalWrite(PINS[i], LOW);
    }
}

void changeMode(bool isNext) {
    if (isNext) {
        current_mode = current_mode+1 > MODES_AMOUNT 
            ? MODES_AMOUNT : current_mode+1;
    } else {
        current_mode = current_mode-1 < 1
            ? 1: current_mode-1;
    }
    Serial.print("New mode: ");
    Serial.println(current_mode);
}

void loop() {
    if (BTSerial.available()) {
        char command = BTSerial.read();
        Serial.println(command);
        switch (command) {
            case START_BUTTON:
                changeMode(false);
                break;
            case PAUSE_BUTTON:
                changeMode(true);
                break;
            default:
                Serial.println("Unknown command: " + command);
                break;
        }
    }
}
