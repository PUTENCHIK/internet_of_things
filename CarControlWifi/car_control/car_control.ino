#define LEFT_DIRECTION 7
#define LEFT_SPEED 6
#define RIGHT_SPEED 5
#define RIGHT_DIRECTION 4
#include <SoftwareSerial.h>

const int txPin = 11; // Arduino RX
const int rxPin = 10; // Arduino TX
double ratio = 0.5;

SoftwareSerial espSerial(rxPin, txPin);

void move(bool dir_left, int speed_left, bool dir_right, int speed_right) {

    if (speed_left > 255) {
        speed_left = 255;
    }
    if (speed_right > 255) {
        speed_right = 255;
    }
    if (speed_left < 0) {
        speed_left = 0;
    }
    if (speed_right < 0) {
        speed_right = 0;
    }

    digitalWrite(LEFT_DIRECTION, dir_left);
    analogWrite(LEFT_SPEED, speed_left);
    digitalWrite(RIGHT_DIRECTION, dir_right);
    analogWrite(RIGHT_SPEED, speed_right);
}

void go_forward() {
    move(LEFT_SIDE_FORWARD, 200, RIGHT_SIDE_FORWARD, 200);
}


void go_backward() {
    move(LEFT_SIDE_BACKWARD, 200, RIGHT_SIDE_BACKWARD, 200);
}

void turn_left_onspot() {
    move(LEFT_SIDE_BACKWARD, 200, RIGHT_SIDE_FORWARD, 200);
}

void turn_right_onspot() {
    move(LEFT_SIDE_FORWARD, 200, RIGHT_SIDE_BACKWARD, 200);
}

void stop() {
    move(0, 0, 0, 0);
}

void setup() {
    Serial.begin(9600);
    espSerial.begin(9600);

    for (int i=4; i<=7; i++) {
        pinMode(i, OUTPUT);
        digitalWrite(i, LOW);
    }

}

void loop() {
    if (espSerial.available() > 0) {
        char command = espSerial.read();
        Serial.print("Received command: ");
        Serial.println(command);

        if (command == "f") {
            go_forward();
        }
        else if (command == "b") {
            go_backward();
        }
        else if (command == "l") {
            turn_left_onspot();
        }
        else if (command == "r") {
            turn_right_onspot();
        }
        else {
            Serial.println("Unknown command");
        }
    }
}
