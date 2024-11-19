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


void showFrame(int numberRow, int arr[8]) {

    digitalWrite(c1, !arr[0]);
    digitalWrite(c2, !arr[1]);
    digitalWrite(c3, !arr[2]);
    digitalWrite(c4, !arr[3]);
    digitalWrite(c5, !arr[4]);
    digitalWrite(c6, !arr[5]);
    digitalWrite(c7, !arr[6]);
    digitalWrite(c8, !arr[7]);
    
    if (numberRow >= 6) {
        analogWrite(rows[numberRow], 1023);
    }
    else {
        digitalWrite(rows[numberRow], HIGH);
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
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(r3, OUTPUT);
  pinMode(r4, OUTPUT);
  pinMode(r5, OUTPUT);
  pinMode(r6, OUTPUT);
  pinMode(r7, OUTPUT);
  pinMode(r8, OUTPUT);

  pinMode(c1, OUTPUT);
  pinMode(c2, OUTPUT);
  pinMode(c3, OUTPUT);
  pinMode(c4, OUTPUT);
  pinMode(c5, OUTPUT);
  pinMode(c6, OUTPUT);
  pinMode(c7, OUTPUT);
  pinMode(c8, OUTPUT);

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
