// 10 9 8 7 6
//  A(7)
//|------|
//|      |
//|F(9)  |B(6)
//|      |
//|------|G(10)
//|      |
//|E(1)  |C(4)
//|      |
//|------|
//  D(2)
// 1 2 3 4 5

// 1 2 3 4 5 6 7 8 9 10
// E D   C . B A   F G

#define PIN_DATA 6
#define PIN_LETCH 4
#define PIN_CLOCK 3

int stage = 1;
int amount = 1;
byte value;

//                   .GFABCDE
byte numbers[10] = {B10000100,   //0
                    B10101111,   //1
                    B11001000,   //2
                    B10001010,   //3
                    B10100011,   //4
                    B10010010,   //5
                    B10010000,   //6
                    B10001111,   //7
                    B10000000,   //8
                    B10000010,   //9
};

void setup() {
    pinMode(PIN_DATA, OUTPUT);
    pinMode(PIN_LETCH, OUTPUT);
    pinMode(PIN_CLOCK, OUTPUT);
    Serial.begin(9600);
    value = numbers[0];
}

void display() {
    digitalWrite(PIN_LETCH, 0);
    switch (stage) {
        case 1:
            stage++;
            break;
        case 2:
            digitalWrite(PIN_DATA, value%2);
            stage++;
            value >>= 1;
            break;
        case 3:
            digitalWrite(PIN_CLOCK, 1);
            stage++;
            break;
        case 4:
            digitalWrite(PIN_CLOCK, 0);
            stage++;
            break;
        case 5:
            digitalWrite(PIN_DATA, 0);
            amount++;
            stage = 1;
            break;
    }
}

void loop() {
    if (amount == 8) {
        int index = analogRead(A0) / (1024 / 10 + 1);
        Serial.print(index);
        value = numbers[index];
        Serial.print("\t");
        Serial.println(value);
        digitalWrite(PIN_LETCH, 1);
        amount = 1;
    }
    display();
}
