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


int currentNumber = 0;
int stage = 1;
int amount = 1;
byte value;
String inputString = "";

//                   .GFABCDE
// byte numbers[10] = {B11000000,   //0
//                     B11110011,   //1
//                     B10100100,   //2
//                     B10100001,   //3
//                     B10010011,   //4
//                     B10001001,   //5
//                     B10001000,   //6
//                     B11100011,   //7
//                     B10000000,   //8
//                     B10000001,   //9
// };
//                   .ABFGEDC
byte numbers[10] = {B10001000,   //0
                    B11110011,   //1
                    B10100100,   //2
                    B10100001,   //3
                    B10010011,   //4
                    B10001001,   //5
                    B10001000,   //6
                    B11100011,   //7
                    B10000000,   //8
                    B10000001,   //9
};

void setup() {
    DDRD = B00011100;
    Serial.begin(9600);
    value = numbers[currentNumber%10];
    timer = millis();

//     TIMER 2
    cli();
    TCCR2A = 0;
    TCCR2B = 0;
    // timer 2 with prescaler 8 = 128 микросекунд
    TCCR2B = TCCR2B | (1 << CS11);
    
    TIMSK2 = TIMSK2 | (1 << TOIE2); 
    sei();
}

void myDigitalWrite(int pin, int value) {
    if (value) {
        PORTD = PORTD ^ (1 << pin);
    }
    else {
        PORTD = PORTD ^ !(1 << pin);
    }
}

ISR(TIMER2_OVF_vect) {
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
    
    if (amount == 9) {
        amount = 1;
        digitalWrite(PIN_LETCH, 1);
        value = numbers[currentNumber];
    }
}

void loop() {
    if (Serial.available() > 0) {
        inputString = Serial.readString();

        Serial.println(inputString);


        if (inputString.length() == 2) {
            if (isDigit(inputString[0]) && isDigit(inputString[1])) {
                startNumber = inputString.toInt();
                currentNumber = startNumber;
            }
            else {
                Serial.println("Ошибка: Введено неверное число");
            }
        }
        else if (inputString.length() == 1) {
            if (isDigit(inputString[0])) {
                startNumber = inputString.toInt();
                currentNumber = startNumber;
            }
            else {

                Serial.println("Ошибка: Введено неверное число");
            }
        }
        else {
            Serial.println("Ошибка: Введены лишние символы");
        }
    }
    if (millis() > timer + duration) {
        currentNumber = currentNumber > 0 ? currentNumber-1 : startNumber;
        timer = millis();
    }
}
