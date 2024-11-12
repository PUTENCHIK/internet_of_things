#define r1 0         // red right
#define r2 12         // gray right
#define r3 11         // puple right
#define r4 2          // black left
#define r5 10          // white left
#define r6 1          // yellow left
#define r7 A1         // brawn left
#define r8 A0         // orange left

#define c1 7          // gray right
#define c2 6          // white right
#define c3 8          // brawn right
#define c4 13          // yellow right short
#define c5 4         // yellow right long
#define c6 9          // brawn left
#define c7 3          // gray left
#define c8 5          // blue left


int arr[8][8] = {{0, 1, 0, 0, 0, 1, 0},};
int rows[8] = {r1, r2, r3, r4, r5, r6, r7, r8}; 


void showFrame(int numberRow, int arr[8]) {
    
    if (numberRow == 6 || numberRow == 7) {
        analogWrite(rows[numberRow], 1023);
    }
    else {
        digitalWrite(rows[numberRow], HIGH);
    }

    digitalWrite(c1, arr[0]);
    digitalWrite(c2, arr[1]);
    digitalWrite(c3, arr[2]);
    digitalWrite(c4, arr[3]);
    digitalWrite(c5, arr[4]);
    digitalWrite(c6, arr[5]);
    digitalWrite(c7, arr[6]);
    digitalWrite(c8, arr[7]);
}


int heart[8][8] = {{0, 1, 1, 0, 0, 1, 1, 0},
                  {1, 1, 1, 1, 1, 1, 1, 1},
                  {1, 1, 1, 1, 1, 1, 1, 1},
                  {1, 1, 1, 1, 1, 1, 1, 1},
                  {0, 1, 1, 1, 1, 1, 1, 0},
                  {0, 0, 1, 1, 1, 1, 0, 0},
                  {0, 0, 1, 1, 1, 1, 0, 0},
                  {0, 0, 0, 1, 1, 0, 0, 0}};


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
}

void loop() {
    digitalWrite(r1, HIGH);
    digitalWrite(r2, LOW);
    digitalWrite(r3, LOW);
    digitalWrite(r4, LOW);
    digitalWrite(r5, LOW);
    digitalWrite(r6, LOW);
    analogWrite(r7, 0);
    analogWrite(r8, 0);

    digitalWrite(c1, HIGH);
    digitalWrite(c2, LOW);
    digitalWrite(c3, HIGH);
    digitalWrite(c4, HIGH);
    digitalWrite(c5, HIGH);
    digitalWrite(c6, HIGH);
    digitalWrite(c7, LOW);
    digitalWrite(c8, HIGH);

    delay(7);

    digitalWrite(r1, LOW);
    digitalWrite(r2, HIGH);
    digitalWrite(r3, LOW);
    digitalWrite(r4, LOW);
    digitalWrite(r5, LOW);
    digitalWrite(r6, LOW);
    analogWrite(r7, 0);
    analogWrite(r8, 0);

    digitalWrite(c1, LOW);
    digitalWrite(c2, LOW);
    digitalWrite(c3, LOW);
    digitalWrite(c4, LOW);
    digitalWrite(c5, LOW);
    digitalWrite(c6, LOW);
    digitalWrite(c7, LOW);
    digitalWrite(c8, LOW);

    delay(7);
}
