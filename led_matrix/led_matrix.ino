#define r1 12         // red right
#define r2 11         // gray right
#define r3 10         // puple right
#define r4 2          // black left
#define r5 1          // white left
#define r6 0          // yellow left
#define r7 A1         // brawn left
#define r8 A0         // orange left

#define c1 9          // gray right
#define c2 8          // white right
#define c3 7          // brawn right
#define c4 6          // yellow right short
#define c5 13         // yellow right long
#define c6 5          // brawn left
#define c7 4          // gray left
#define c8 3          // blue left



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
  digitalWrite(r2, HIGH);
  digitalWrite(r3, HIGH);
  digitalWrite(r4, HIGH);
  digitalWrite(r5, HIGH);
  digitalWrite(r6, HIGH);
  analogWrite(r7, 1023);
  analogWrite(r8, 1023);

  digitalWrite(c1, LOW);
  digitalWrite(c2, LOW);
  digitalWrite(c3, LOW);
  digitalWrite(c4, LOW);
  digitalWrite(c5, LOW);
  digitalWrite(c6, LOW);
  digitalWrite(c7, LOW);
  digitalWrite(c8, LOW);

}
