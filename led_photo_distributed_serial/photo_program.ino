#define p_pin A0

const char PHOTO = 'p';

void setup() {
    Serial.begin(9600);
    pinMode(p_pin, INPUT);
}

void printValue() {
    int value = analogRead(p_pin);
    value /= 4;
    Serial.write(value);
}

void loop() {
    if (Serial.available() > 0) {
        char command = Serial.read();
        switch (command) {
            case PHOTO:
                printValue();
                break;
            default:
                break;
        }
    }
}
