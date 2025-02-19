#include <Servo.h>

#define SERVO_PIN 6

Servo servo1;

void setup() {
   servo1.attach(SERVO_PIN);
}

void loop() {
   servo1.write(0);
   delay(2000);

   servo1.write(90);
   delay(2000);

   servo1.write(180);
   delay(2000);
}