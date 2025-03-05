#include <SoftwareSerial.h>

#define LEFT_DIRECTION 7
#define LEFT_SPEED 6
#define RIGHT_SPEED 5
#define RIGHT_DIRECTION 4

#define LEFT_SIDE_FORWARD HIGH
#define RIGHT_SIDE_FORWARD LOW

#define LEFT_SIDE_BACKWARD LOW
#define RIGHT_SIDE_BACKWARD HIGH

SoftwareSerial BTSerial(10, 11);
String text = "";
char c;

void setup() {
    Serial.begin(9600);
    while (!Serial) { }
    Serial.println("Hello from Maximka's phone!");

    BTSerial.begin(9600);
    BTSerial.println("Hello, from Maximka's PC!");

    for (int i=4; i<=7; i++) {
        pinMode(i, OUTPUT);
        digitalWrite(i, LOW);
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


void move_only_right() {
    move(true, 200, true, 200);
}


void move_only_left() {
    move(false, 200, false, 200);
}


void go_forward(int speed) {
    move(LEFT_SIDE_FORWARD, speed, RIGHT_SIDE_FORWARD, speed);
}


void go_backward(int speed) {
    move(LEFT_SIDE_BACKWARD, speed, RIGHT_SIDE_BACKWARD, speed);
}


void turn_left(int speed, float ratio) {
    /* ration is between 0 and 1 */
    move(LEFT_SIDE_FORWARD, int(speed * ratio), RIGHT_SIDE_FORWARD, speed);
}


void turn_right(int speed, float ratio) {
    /* ration is between 0 and 1 */
    move(LEFT_SIDE_FORWARD, speed, RIGHT_SIDE_FORWARD, int(speed * ratio));
}

void turn_left_onspot(int speed) {
    move(LEFT_SIDE_BACKWARD, speed, RIGHT_SIDE_FORWARD, speed);
}

void turn_right_onspot(int speed) {
    move(LEFT_SIDE_FORWARD, speed, RIGHT_SIDE_BACKWARD, speed);
}

void stop() {
    move(0, 0, 0, 0);
}

void loop() {
//    if (Serial.available()) {
//        BTSerial.write(Serial.read());
//    }
    if (BTSerial.available()) {
        char c = BTSerial.read();
        switch (c) {
            case 'F':
                go_forward(200);
                break;
            case 'B':
                go_backward(200);
                break;
            case 'R':
                move_only_right();
                break;
            case 'L':
                move_only_left();
                break;
            case '0':
                stop();
                break;
            default:
                break;
        }
        Serial.write(c);
    }
}
