#define D3 3
#define D4 4
#define D5 5
#define D6 6
#define DURATION 1000


int pins[4] = {D6, D5, D4, D3};

struct MyDict {
    String name;
    byte value;
};

const int statesLength = 2; 

MyDict states[] = {
    {"1", B10010000},
    {"2", B10100000}
};

bool isDiagonal = false;
long int start = millis();


byte getValue(String key) {
    for (int i = 0; i < statesLength; i++) {
        if (states[0].name == key) {
            return states[0].value;
        }
    }
    return 0;
}


void setState(byte state) {
    for (int i = 0; i < 4; i++) {
        digitalWrite(pins[i], state & 0b00000001);
        state >>= 1;
    }
    delay(1);
    for (int i = 4; i < 8; i++) {
        digitalWrite(pins[i], state & 0b00000001);
        state >>= 1;
    }
    delay(1);
}


void setup() {
    Serial.begin(9600);
    pinMode(D3, OUTPUT);
    pinMode(D4, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
}
void loop() {
    if (Serial.available()) {
        String cmd = Serial.readString();
        byte state = getValue(cmd);
        Serial.println(cmd);
        Serial.println(state);
        setState(state);
    }
    // if (millis() >= start + DURATION) {
    //     start = millis();
    //     isDiagonal = isDiagonal ? false : true;
    // }
}
