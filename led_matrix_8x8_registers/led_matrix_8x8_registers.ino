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
}

void showFrame(int numberRow, int arr[8]) {


    myDigitalWrite(c1, !arr[0]);
    myDigitalWrite(c2, !arr[1]);
    myDigitalWrite(c3, !arr[2]);
    myDigitalWrite(c4, !arr[3]);
    myDigitalWrite(c5, !arr[4]);
    myDigitalWrite(c6, !arr[5]);
    myDigitalWrite(c7, !arr[6]);
    myDigitalWrite(c8, !arr[7]);
    
    if (numberRow >= 6) {
        analogWrite(rows[numberRow], 1023);
    }
    else {
        digitalWrite(rows[numberRow], 1);
    }
}


void reset() {
    digitalWrite(r1, 0);
    digitalWrite(r2, 0);
    digitalWrite(r3, 0);
    digitalWrite(r4, 0);
    digitalWrite(r5, 0);
    digitalWrite(r6, 0);
    analogWrite(r7, 0);
    analogWrite(r8, 0);
  
    digitalWrite(c1, 0);
    digitalWrite(c2, 0);
    digitalWrite(c3, 0);
    digitalWrite(c4, 0);
    digitalWrite(c5, 0);
    digitalWrite(c6, 0);
    digitalWrite(c7, 0);
    digitalWrite(c8, 0);
}


void setup() {
    DDRD = DDRD | (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0);
    DDRB = DDRB | (1 << 5) | (1 << 4) | (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0);
    DDRC = DDRC | (1 << 1) | (1 << 0);

    cli(); // stop interrupts
    // 16мкс * 64 = 1024мкс ~ 1мс
    TCCR2A = 0;
    TCCR2B = 0;
    TCCR2B = TCCR2B | (1 << CS11)| (1 << CS10);

    TIMSK2 = TIMSK2 | (1 << TOIE2); 
    sei(); // allow interrupts
}

ISR(TIMER2_OVF_vect) {
  if (isHigh) {
    showFrame(currentRow, heart[currentRow]);
    currentRow = currentRow == 7 ? 0 : currentRow+1;
  } else {
    reset();
  }
  isHigh = !isHigh;
}


void loop() {}
