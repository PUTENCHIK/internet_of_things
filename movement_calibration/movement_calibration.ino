#include "Config.h"
#include <SoftwareSerial.h>

SoftwareSerial BTSerial(MCE_RX, MCE_TX);

int current_mode = 1;
int speed_left = DEFAULT_SPEED;
int speed_right = DEFAULT_SPEED;
int current_direction_side_index = DEFAULT_DIRECTION_SIDE_INDEX;
CalibrationObject current_calibration_object = {CalibrationObject::MoveForward}

int segmentNumber = 5;
int segmentStage = 1;
int segmentAmount = 1;
byte segmentValue;
long int segmentTimer;

void setup() {
    Serial.begin(9600);
    while (!Serial) { }
    Serial.println("Hello from PC!");

    BTSerial.begin(9600);
    BTSerial.println("Hello, from phone!");

    for (int i = 0; i < PINS_AMOUNT; i++) {
        pinMode(PINS[i], OUTPUT);
        digitalWrite(PINS[i], LOW);
    }

    segmentValue = NUMBERS[segmentNumber%10];
    segmentTimer = millis();

    cli();
    TCCR2A = 0;
    TCCR2B = 0;
    TCCR2B = TCCR2B | (1 << CS11);
    TIMSK2 = TIMSK2 | (1 << TOIE2); 
    sei();
}

ISR(TIMER2_OVF_vect) {
    digitalWrite(PIN_LETCH, 0);
    switch (segmentStage) {
        case 1:
            segmentStage++;
            break;
        case 2:
            digitalWrite(PIN_DATA, segmentValue%2);
            segmentStage++;
            segmentValue >>= 1;
            break;
        case 3:
            digitalWrite(PIN_CLOCK, 1);
            segmentStage++;
            break;
        case 4:
            digitalWrite(PIN_CLOCK, 0);
            segmentStage++;
            break;
        case 5:
            digitalWrite(PIN_DATA, 0);
            segmentAmount++;
            segmentStage = 1;
            break;
    }
    
    if (segmentAmount == 8) {
        segmentAmount = 1;
        digitalWrite(PIN_LETCH, 1);
        segmentValue = NUMBERS[segmentNumber%10];
    }
}

void move(bool dir_left, int speed_left, bool dir_right, int speed_right) {

    if (speed_left > 255) {
        speed_left = 255;
    }
    if (speed_right > 255) {
        speed_right = 255;
    }

    digitalWrite(LEFT_DIRECTION, dir_left);
    analogWrite(LEFT_SPEED, speed_left);
    digitalWrite(RIGHT_DIRECTION, dir_right);
    analogWrite(RIGHT_SPEED, speed_right);
}


void go_forward(int speed) {
    move(DIRECTION_SIDE_COEF[current_direction_side_index][2], 
         speed, 
         DIRECTION_SIDE_COEF[current_direction_side_index][0], 
         speed);
}


void go_backward(int speed) {
    move(DIRECTION_SIDE_COEF[current_direction_side_index][3], 
         speed, 
         DIRECTION_SIDE_COEF[current_direction_side_index][1], 
         speed);
}


void turn_left_onspot(int speed) {
    move(DIRECTION_SIDE_COEF[current_direction_side_index][3], 
         speed, 
         DIRECTION_SIDE_COEF[current_direction_side_index][0], 
         speed);
}


void turn_right_onspot(int speed) {
    move(DIRECTION_SIDE_COEF[current_direction_side_index][2], 
         speed, 
         DIRECTION_SIDE_COEF[current_direction_side_index][1], 
         speed);
}

void changeMode(bool isNext) {
    if (isNext) {
        current_mode = current_mode+1 > MODES_AMOUNT 
            ? MODES_AMOUNT : current_mode+1;
    } else {
        current_mode = current_mode-1 < 1
            ? 1: current_mode-1;
    }
    Serial.print("New mode: ");
    Serial.println(current_mode);
}

void changeDirectionSide(bool isNext) {
    if (isNext) {
        current_direction_side_index = current_direction_side_index + 1 > DIRECTION_SIDE_AMOUNT 
            ? DIRECTION_SIDE_AMOUNT : current_direction_side_index+1;
    } else {
        current_direction_side_index = current_direction_side_index - 1 < 0
            ? 0 : current_direction_side_index-1;
    }
    Serial.print("New direction side index: ");
    Serial.println(current_direction_side_index);
}


void loop() {
    if (BTSerial.available()) {
        char command = BTSerial.read();
        Serial.println(command);
        switch (command) {
            case START_BUTTON:
                changeMode(false);
                break;
            case PAUSE_BUTTON:
                changeMode(true);
                break;
            case T_BUTTON:
                changeDirectionSide(true);
                break;
            case X_BUTTON:
                changeDirectionSide(false);
                break;
            case FORWARD_BUTTON:
                go_forward(DEFAULT_SPEED);
                break;
            case BACKWARD_BUTTON:
                go_forward(DEFAULT_SPEED);
                break;
            case RIGHT_BUTTON:
                turn_right_onspot(DEFAULT_SPEED);
                break;
            case LEFT_BUTTON:
                turn_left_onspot(DEFAULT_SPEED);
                break;
            default:
                Serial.println("Unknown command: " + command);
                break;
        }
    }
    if (millis() > segmentTimer + SEGMENT_DELAY) {
        segmentNumber = segmentNumber > 0 ? segmentNumber-1 : 9;
        segmentTimer = millis();
    }
}
