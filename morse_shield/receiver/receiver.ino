#define RECEIVE_PIN 0
#define TRANSMIT_PIN 1
#define LED_PIN 6


void setup() {
    Serial.begin(9600);
//    pinMode(RECEIVE_PIN, INPUT);
//    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    if (Serial.available()) {
        int s = Serial.read();
        Serial.println(s);
    }
}
