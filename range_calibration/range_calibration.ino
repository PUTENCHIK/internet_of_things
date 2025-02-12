#define TRIG 3
#define ECHO 4
#define ULTRA A0

int distance, value;

const char GET = 'g';

void setup() {
    Serial.begin(9600);
    
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
}

int getDistance() {
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    int result = pulseIn(ECHO, HIGH) * 0.034/2;
    
    return result > 0 ? result : 0;
}

int getUltraValue() {
    return analogRead(ULTRA);
}

int countDigits(int num) {
    int digits = 1, buffer = num;
    while (buffer > 9) {
        digits++;
        buffer /= 10;
    }
    return digits;
}

void printValues() {
    for (int i = 4; i > countDigits(distance); i--)
        Serial.print('0');
    Serial.print(distance);
    Serial.print(' ');
    for (int i = 4; i > countDigits(value); i--)
        Serial.print('0');
    Serial.println(value);
}

void loop() {
    if (Serial.available() > 0) {
        char command = Serial.read();
        switch (command) {
            case GET:
                distance = getDistance();
                value = getUltraValue();
                printValues();
                break;
            default:
                break;
        }
    }
}
