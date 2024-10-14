#define l_pin 13

const char UP = 'u';
const char DOWN = 'd';
bool isHigh = false;

void setup() {
    Serial.begin(9600);
    pinMode(l_pin, OUTPUT);
}

void updateLed() {
    if (isHigh) {
        digitalWrite(l_pin, HIGH);
        Serial.println("Led is high");
    } else {
        digitalWrite(l_pin, LOW);
        Serial.println("Led is low mmr");
    }
}

void loop() {
    if (Serial.available() > 0) {
        char command = Serial.read();
        switch(command) {
            case UP:
                if (!isHigh) {
                    isHigh = true;
                    updateLed();
                }
                break;
            case DOWN:
                if (isHigh) {
                    isHigh = false;
                    updateLed();
                }
                break;
            default:
                Serial.println("Unknown command");
        }
    }
}
