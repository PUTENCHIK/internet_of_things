#define l_photo A1
#define r_photo A0
#define l_led 7
#define r_led 8

const int LIMIT_EQUAL = 110;                                // Разница в значениях фоторезисторов, при которой считается, что разницы нет
const int DELAY_BLINK = 200;                                // Задержка мигания светодиодов
unsigned long timer = millis();
bool isHigh = true;
bool isBlinking = true;
bool isLeftLed = false;

/*
diff > 0: right
diff < 0: left
*/
int diff;
int l_value, r_value;

void setup() {
  Serial.begin(9600);
  pinMode(l_photo, INPUT);
  pinMode(r_photo, INPUT);
  pinMode(l_led, OUTPUT);
  pinMode(r_led, OUTPUT);
}

void printValues(int l, int r, int d) {
  Serial.print("\t\t");
  Serial.print(l);
  Serial.print("\t\t");
  Serial.print(r);
  Serial.print("\t\t");
  Serial.println(d);
}

void ledsOff() {
  digitalWrite(l_led, LOW);
  digitalWrite(r_led, LOW);
}

void ledsOn() {
  digitalWrite(l_led, HIGH);
  digitalWrite(r_led, HIGH);
}

void blinking(int diff) {
  if (isLeftLed && diff > 0) {
    isLeftLed = false;
    digitalWrite(l_led, LOW);
  } else if (!isLeftLed && diff < 0) {
    isLeftLed = true;
    digitalWrite(r_led, LOW);
  }

  if (millis() - timer > DELAY_BLINK) {
    isHigh = !isHigh;
    timer = millis();

    if (isLeftLed) {
      digitalWrite(l_led, isHigh);
    } else {
      digitalWrite(r_led, isHigh);
    }
  }
}

void loop() {
  r_value = analogRead(r_photo);
  l_value = analogRead(l_photo);
  diff = r_value - l_value;
  if (abs(diff) > LIMIT_EQUAL) {
    if (!isBlinking) {
      isBlinking = true;
      ledsOff();
    }
    
    blinking(diff);
  } else {
    if (isBlinking) {
      isBlinking = false;
      ledsOn();
    }
  }

  printValues(l_value, r_value, diff);
}