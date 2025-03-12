#include "Config.h"
#include <SoftwareSerial.h>

SoftwareSerial BTSerial(MCE_RX, MCE_TX);


int current_side = 1;
int current_mode = 1;
int speed_left = DEFAULT_SPEED;
int speed_right = DEFAULT_SPEED;
int current_direction_side_index = DEFAULT_DIRECTION_SIDE_INDEX;
int current_speed_object = 1;
int current_turning_object = 1;
long int turning_time[TURNING_SETTINGS_AMOUNT] {0, 0, 0, 0};
long int start_timer = 0;
long int end_timer = 0;

int segmentNumber = 5;
int segmentStage = 1;
int segmentAmount = 1;
byte currentValue = NUMBERS[current_mode];
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

    segmentValue = currentValue;
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
        segmentValue = currentValue;
    }
}

void move(bool dir_left, int speed_left, bool dir_right, int speed_right) {
    digitalWrite(LEFT_DIRECTION, dir_left);
    analogWrite(LEFT_SPEED, speed_left);
    digitalWrite(RIGHT_DIRECTION, dir_right);
    analogWrite(RIGHT_SPEED, speed_right);
}

void stop() {
    move(0, 0, 0, 0);
}

void go_forward() {
    move(DIRECTION_SIDE_COEF[current_direction_side_index][2], 
         speed_left, 
         DIRECTION_SIDE_COEF[current_direction_side_index][0], 
         speed_right);
}


void go_backward() {
    move(DIRECTION_SIDE_COEF[current_direction_side_index][3], 
         speed_left, 
         DIRECTION_SIDE_COEF[current_direction_side_index][1], 
         speed_right);
}


void turn_left_onspot() {
    move(DIRECTION_SIDE_COEF[current_direction_side_index][3], 
         speed_left, 
         DIRECTION_SIDE_COEF[current_direction_side_index][0], 
         speed_right);
}


void turn_right_onspot() {
    move(DIRECTION_SIDE_COEF[current_direction_side_index][2], 
         speed_left, 
         DIRECTION_SIDE_COEF[current_direction_side_index][1], 
         speed_right);
}

void turn_right_onspot_calibration(int speed = 100) {
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
            ? 1 : current_mode-1;
    }
    Serial.print("New mode: ");
    Serial.println(current_mode);
    currentValue = NUMBERS[current_mode];
}

void changeCurrentTurningObject(bool isNext) {
    if (isNext) {
        current_turning_object = current_turning_object+1 > TURNING_SETTINGS_AMOUNT 
            ? TURNING_SETTINGS_AMOUNT : current_turning_object+1;
    } else {
        current_turning_object = current_turning_object-1 < 1
            ? 1 : current_turning_object-1;
    }

    Serial.print("New turning object: ");
    Serial.println(current_turning_object);
}

void changeCurrentSpeedObject(bool isNext) {
    if (isNext) {
        current_speed_object = current_speed_object+1 > SPEED_SETTINGS_AMOUNT 
            ? SPEED_SETTINGS_AMOUNT : current_speed_object+1;
    } else {
        current_speed_object = current_speed_object-1 < 1
            ? 1 : current_speed_object-1;
    }

    Serial.print("New speed object: ");
    Serial.println(current_speed_object);
}

void changeDirectionSide(bool isNext) {
    if (isNext) {
        current_direction_side_index = current_direction_side_index + 1 > DIRECTION_SIDE_AMOUNT - 1
            ? DIRECTION_SIDE_AMOUNT - 1 : current_direction_side_index+1;
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
        Serial.print("Command: ");
        Serial.println(command);
        switch (command) {
            case START_BUTTON:
                changeMode(false);
                break;
            case PAUSE_BUTTON:
                changeMode(true);
                break;
            case T_BUTTON:
                if (current_mode == 1) {
                    changeDirectionSide(true);
                }
                else if (current_mode == 2) {
                    if (current_speed_object == 1) {
                        speed_left += 1;
                    }
                    else if (current_speed_object == 2) {
                        speed_right += 1;
                    }
                    else if (current_speed_object == 3) {
                        speed_left += 10;
                        speed_right += 10;
                    }

                    if (speed_left > 255) {
                        speed_left = 255;
                    }
                    if (speed_right > 255) {
                        speed_right = 255;
                    }

                    Serial.print("Speed left wheel: ");
                    Serial.println(speed_left);

                    Serial.print("Speed right wheel: ");
                    Serial.println(speed_right);
                }
                else if (current_mode == 3) {
                    turn_right_onspot_calibration();
                    delay(turning_time[current_turning_object-1]);
                }
                break;
            case X_BUTTON:
                if (current_mode == 1) {
                    changeDirectionSide(false);
                }
                else if (current_mode == 2) {
                    if (current_speed_object == 1) {
                        speed_left -= 1;
                    }
                    else if (current_speed_object == 2) {
                        speed_right -= 1;
                    }
                    else if (current_speed_object == 3) {
                        speed_left -= 10;
                        speed_right -= 10;
                    }

                    Serial.print("Текущая скорость левого колеса: ");
                    Serial.println(speed_left);

                    Serial.print("Текущая скорость правого колеса: ");
                    Serial.println(speed_right);
                }
                else if (current_mode == 3) {
                    if (start_timer == 0) {
                        start_timer = millis();
                        turn_right_onspot_calibration();
                    }
                    else {
                        stop();
                        end_timer = millis();
                        turning_time[current_turning_object-1] = end_timer - start_timer;
                        start_timer = 0;
                        end_timer = 0;
                    }
                }
                break;

            case S_BUTTON:
                if (current_mode == 2) {
                    changeCurrentSpeedObject(false);
                }
                else if (current_mode == 3) {
                    changeCurrentTurningObject(false);
                }
                break;
            case O_BUTTON:
                if (current_mode == 2) {
                    changeCurrentSpeedObject(true);
                }
                else if (current_mode == 3) {
                    changeCurrentTurningObject(true);
                }
                break;
            case FORWARD_BUTTON:
                go_forward();
                break;
            case BACKWARD_BUTTON:
                go_backward();
                break;
            case RIGHT_BUTTON:
                turn_right_onspot();
                break;
            case LEFT_BUTTON:
                turn_left_onspot();
                break;
            case PULLUP:
                stop();
                break;
            default:
                Serial.println("Unknown command: " + command);
                break;
        }
    }
}
