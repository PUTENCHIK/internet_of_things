#define led_r 8
#define led_y 9
#define led_g 10


const int red_delay = 7000;
const int yellow_delay = 1000;
const int yellow_manual_delay = 3000;
const int green_delay = 10000;
const int green_blink_delay = 2000;
const int delays[7] = {green_delay,
                        green_blink_delay/3,
                        green_blink_delay/3, 
                        green_blink_delay/3, 
                        yellow_delay, 
                        red_delay, 
                        yellow_delay};
const int MANUAL_MODE_LIMIT = 15000;

const char CHANGE_MODE = 'c';
const char HIGH_RED = 'r';
const char HIGH_GREEN = 'g';

bool isAuto = true;                                           // автоматический ли режим
bool isRed = true;                                            // красный ли сигнал в ручном режиме
bool isChanged = false;                                       // менялся ли в ручном режиме сигнал

/* Этапы автоматического режима
1 - зелёный
2 - зелёный тухнет
3 - зелёный загорается
4 - зелёный тухнет
5 - жёлтый
6 - красный
7 - жёлтый
*/
int stage = 0;
unsigned long timer = millis();
unsigned long manual_mode_timer = 0;

void setup() {
  Serial.begin(9600);
  pinMode(led_r, OUTPUT);
  pinMode(led_y, OUTPUT);
  pinMode(led_g, OUTPUT);
}

void lowLeds() {
  digitalWrite(led_r, LOW);
  digitalWrite(led_y, LOW);
  digitalWrite(led_g, LOW);
}

void autoMode() {
  if (!stage or (millis() - timer > delays[stage-1])) {
    // Serial.print("Diff: ");
    // Serial.println(millis() - timer);
    stage = stage+1 == 8 ? 1 : stage+1;
    lowLeds();
    switch(stage) {
      case 1:
        // Serial.println("Stage 1!");
        digitalWrite(led_g, HIGH);
        break;
      case 2:
      case 4:
        // Serial.println("Stage 2!");
        digitalWrite(led_g, LOW);
        break;
      case 3:
        // Serial.println("Stage 3!");
        digitalWrite(led_g, HIGH);
        break;
      case 5:
        // Serial.println("Stage 5!");
        digitalWrite(led_y, HIGH);
        break;
      case 6:
        // Serial.println("Stage 6!");
        digitalWrite(led_r, HIGH);
        break;
      case 7:
        // Serial.println("Stage 7!");
        digitalWrite(led_y, HIGH);
        break;
    }
    timer = millis();
  }
}

void highRedOrGreen() {
  if (isRed) {
    digitalWrite(led_r, HIGH);
  } else {
    digitalWrite(led_g, HIGH);
  }
}

void manualMode() {
  if (isChanged) {
    if (millis() - timer > yellow_manual_delay) {
      isChanged = false;
      digitalWrite(led_y, LOW);
      highRedOrGreen();
    }
  }
}

void loop() {
  if (isAuto) {
    autoMode();
  } else {
    if (millis() - manual_mode_timer > MANUAL_MODE_LIMIT) {
      isAuto = true;
      timer = millis();
      lowLeds();
      stage = 0;
      Serial.println("Force to auto mode");
    } else {
      manualMode();
    }
  }

  if (Serial.available() > 0) {
    char symbol = Serial.read();
    Serial.print("Command: ");
    Serial.println(symbol);
    switch (symbol) {
      case CHANGE_MODE:
        isAuto = !isAuto;
        if (isAuto) {
          stage = 0;
          timer = millis();
        } else {
          lowLeds();
          manual_mode_timer = millis();
          highRedOrGreen();
        }
        break;
      case HIGH_RED:
        if (!isAuto and !isRed) {
          lowLeds();
          isRed = true;
          isChanged = true;
          digitalWrite(led_y, HIGH);
          timer = millis();
        }
        break;
      case HIGH_GREEN:
        if (!isAuto and isRed) {
          lowLeds();
          isRed = false;
          isChanged = true;
          digitalWrite(led_y, HIGH);
          timer = millis();
        }
        break;
    }
  }
}
