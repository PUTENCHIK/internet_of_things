#define PIN_DATA 4
#define PIN_LETCH 3
#define PIN_CLOCK 2

// state: 
// int state = 1;
bool first = true;

void setup() {
    pinMode(PIN_DATA, OUTPUT);
    pinMode(PIN_LETCH, OUTPUT);
    pinMode(PIN_CLOCK, OUTPUT);
    // DDRD = B00011100;

    // cli();
    // TCCR2A = 0;
    // TCCR2B = 0;
    // // timer 2 with prescaler 8 = 128 микросекунд
    // TCCR2B = TCCR2B | (1 << CS11);

    // TIMSK2 = TIMSK2 | (1 << TOIE2); 
    // sei();
}

// ISR(TIMER2_OVF_vect) {
    
// }

void loop() {
    byte value = B11111000;
    digitalWrite(PIN_LETCH, LOW);
    shiftOut(PIN_DATA, PIN_CLOCK, LSBFIRST, value);
    digitalWrite(PIN_LETCH, HIGH);

    delay(100);
    
    value = B00000111;
    digitalWrite(PIN_LETCH, LOW);
    delay(2000);
    
    shiftOut(PIN_DATA, PIN_CLOCK, LSBFIRST, value);
    digitalWrite(PIN_LETCH, HIGH);
    
    delay(100);
}
