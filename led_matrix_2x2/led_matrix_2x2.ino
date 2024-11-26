#define D3 3
#define D4 4
#define D5 5
#define D6 6


const int DURATION = 1000;
int pins[4] = {D6, D5, D4, D3};

struct MyDict {
    String name;
    byte value;
};

const int statesLength = 16; 

MyDict states[] = {
    {"off", B00000000},
    {"on", B11000000},
    {"1", B10010000},
    {"2", B10100000},
    {"3", B01010000},
    {"4", B01100000},
    {"12", B10000000},
    {"24", B11100000},
    {"34", B01000000},
    {"13", B11010000},
    {"14", B10010110},
    {"23", B10100101},
    {"124", B10000110},
    {"234", B10100100},
    {"123", B10000101},
    {"134", B10010100},
};

bool isSet = false;
byte fullCurrentState;
byte currentState;


byte getValue(String key) {
    for (int i = 0; i < statesLength; i++) {
        if (states[i].name == key) {
            return states[i].value;
        }
    }
    return -1;
}


void setState(byte state) {
    for (int i = 0; i < 4; i++) {
        digitalWrite(pins[i], state & 0b00000001);
        state >>= 1;
    }
    delay(1);
}


void reset() {
    for (int i = 0; i < 4; i++) {
        digitalWrite(pins[i], LOW);
    }
}


void setup() {
    Serial.begin(9600);
    pinMode(D3, OUTPUT);
    pinMode(D4, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
}
void loop() {
    if (isSet) {
        if (currentState == 0) {
            currentState = fullCurrentState;
        }
        setState(currentState);
        currentState >>= 4;
    }
  
    if (Serial.available()) {
        reset();
        String cmd = Serial.readString();
        byte state = getValue(cmd);
        

        if (state != byte(-1) && (!isSet || state != fullCurrentState)) {
            Serial.print(cmd + "\t");
            Serial.println(state);
            fullCurrentState = state;
            currentState = state;
            isSet = fullCurrentState != 0;
        } else {
            Serial.println("Bad command: " + cmd);
        }
    }
}
