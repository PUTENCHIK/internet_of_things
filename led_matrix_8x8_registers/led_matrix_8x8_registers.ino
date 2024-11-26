#define r1 0            // puple-yellow right 5
#define r2 12           // orange-red left 6
#define r3 11           // red-gray left 5
#define r4 2            // black-black right 2
#define r5 10           // black-puple left 3
#define r6 1            // white-white right 1
#define r7 A1           // green-brawn right 7
#define r8 A0           // red-orange right 8

#define c1 7            // brawn left 4
#define c2 6            // yellow short 7
#define c3 8            // white left 2
#define c4 13           // yellow long left 8
#define c5 4            // gray right 4
#define c6 9            // gray right 1
#define c7 3            // blue right 6
#define c8 5            // brawn right 1

const int n = 8;
int currentRow = 0;
bool isHigh = true;

int rows[n] = {r1, r2, r3, r4, r5, r6, r7, r8}; 
int columns[n] = {c1, c2, c3, c4, c5, c6, c7, c8};


int heart[n][n] = {{0, 1, 1, 0, 0, 1, 1, 0},
                   {1, 1, 1, 1, 1, 1, 1, 1},
                   {1, 1, 1, 1, 1, 1, 1, 1},
                   {1, 1, 1, 1, 1, 1, 1, 1},
                   {0, 1, 1, 1, 1, 1, 1, 0},
                   {0, 0, 1, 1, 1, 1, 0, 0},
                   {0, 0, 1, 1, 1, 1, 0, 0},
                   {0, 0, 0, 1, 1, 0, 0, 0}};

int human[n][n] = {{0, 0, 0, 1, 1, 0, 0, 0},
                   {0, 0, 0, 1, 1, 0, 0, 0},
                   {1, 1, 1, 1, 1, 1, 1, 1},
                   {1, 0, 1, 1, 1, 1, 0, 1},
                   {1, 0, 1, 1, 1, 1, 0, 1},
                   {0, 0, 1, 0, 0, 1, 0, 0},
                   {0, 0, 1, 0, 0, 1, 0, 0},
                   {0, 0, 1, 0, 0, 1, 0, 0}};


void myDigitalWrite(int pin, int value) {
    if (pin < 8) {
        if (value) {
            PORTD = PORTD ^ (1 << pin);
        }
        else {
            PORTD = PORTD ^ !(1 << pin);
        }
    }
    else if (pin == A0) {
        if (value) {
            PORTC = PORTC ^ (1 << 0);
        }
        else {
            PORTC = PORTC ^ !(1 << 0);
        }
    }
    else if (pin == A1) {
        if (value) {
            PORTC = PORTC ^ (1 << 1);
        }
        else {
            PORTC = PORTC ^ !(1 << 1);
        }
    }
    else if (pin >= 8) {
        if (value) {
            PORTB = PORTB ^ (1 << (pin - 8));
        }
        else {
            PORTB = PORTB ^ !(1 << (pin - 8));
        }
    }
}

void showFrame(int numberRow, int arr[8]) {
    for (int i = 0; i < n; i++) {
        myDigitalWrite(columns[i], !arr[i]);
    }
    
    myDigitalWrite(rows[numberRow], 1);
}

void setup() {
    DDRD = B11111111;
    DDRB = B00111111;
    DDRC = B00000011;

    cli(); // stop interrupts
    // 16мкс * 64 = 1024мкс ~ 1мс
    TCCR2A = 0;
    TCCR2B = 0;
    TCCR2B = TCCR2B| (1 << CS11) | (1 << CS10);

    TIMSK2 = TIMSK2 | (1 << TOIE2); 
    sei(); // allow interrupts
}

ISR(TIMER2_OVF_vect) {
    showFrame(currentRow, heart[currentRow]);
    if (!isHigh) {
        currentRow = currentRow == 7 ? 0 : currentRow+1;
    } 

    isHigh = !isHigh;
}


void loop() {}
