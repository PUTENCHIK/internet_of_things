#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 11);

void setup() {
    Serial.begin(9600);
    Serial.println("Hello, phone!");

    BTSerial.begin(9600);
    BTSerial.println("Hello, PC!");
}

void loop() {
    if (Serial.available()) {
        BTSerial.println(Serial.read());
    }
    if (BTSerial.available()) {
        Serial.println(BTSerial.read());
    }
}
