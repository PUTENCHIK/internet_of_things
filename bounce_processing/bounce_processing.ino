#define PIN_BTN 2

const int bounce_delay = 50;

volatile int press_count = 0;
volatile int bounce_count = 0;
volatile long int press_time;
volatile long int fall_time;

volatile bool press_candidate = false;
volatile bool true_press = false;
volatile bool is_falling = false;

void setup() {
    Serial.begin(9600);
    pinMode(PIN_BTN, INPUT);
    attachInterrupt(0, button_rising, RISING);
}

void button_rising() {
    if (!press_candidate and !true_press) {
        press_candidate = true;
        press_time = millis();
    }
    bounce_count++;
}

void loop() {
    if (press_candidate and !true_press and millis() > press_time + bounce_delay) {
        true_press = true;
        press_candidate = false;
        press_count++;
    }
    
    else if (true_press and !digitalRead(PIN_BTN)) {
        long int buffer = millis();
      
        Serial.print("Hold time: ");
        Serial.println(buffer - press_time);
        Serial.print("Total clicks: ");
        Serial.println(press_count);
        Serial.print("Bounce count: ");
        Serial.println(bounce_count);
        Serial.println();

        press_candidate = false;
        true_press = false;
        bounce_count = 0;
//        is_falling = true;
//        fall_time = millis();
    }

//    if (is_falling and millis() > fall_time + bounce_delay) {
//        is_falling = false;
//        bounce_count = 0;
//    }
}
