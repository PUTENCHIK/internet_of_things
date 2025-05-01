#include "Config.h"
#include <PID_v1_bc.h>
#define LEFT_DIRECTION 7
#define LEFT_SPEED 6
#define RIGHT_SPEED 5
#define RIGHT_DIRECTION 4

#define LEFT_SIDE_FORWARD HIGH
#define RIGHT_SIDE_FORWARD LOW

#define LEFT_SIDE_BACKWARD LOW
#define RIGHT_SIDE_BACKWARD HIGH

#define FORWARD_TRIG 8
#define FORWARD_ECHO 9

#define RIGHT_TRIG 12
#define RIGHT_ECHO 13

const int QUEUE_MAX_SIZE = 4;
int queue_values[QUEUE_MAX_SIZE];
int head = 0; // Индекс начала очереди
int tail = 0; // Индекс конца очереди
int queue_size = 0; // Текущий размер очереди


int forward_distance, right_distance, value;
int base_speed = 170;
int speed_left, speed_right;
double ratio = 0.5;
int average = 0;

double Kp = 0.01;  // Пропорциональный коэффициент
double Ki = 0; // Интегральный коэффициент
double Kd = 0; // Дифференциальный коэффициент


double setpoint = 14;
double input, output;


PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);


bool enqueue(int data) {
    if (queue_size == QUEUE_MAX_SIZE) {
        return false;
    }

    queue_values[tail] = data;
    tail = (tail + 1) % QUEUE_MAX_SIZE;
    queue_size++;
    return true;
}

bool dequeue() {
    if (queue_size == 0) {
        return false;
    }

    head = (head + 1) % QUEUE_MAX_SIZE;
    queue_size--;
    return true;
}

double queueAverage() {
    double summ = 0;
    int count = 0;

    if (QUEUE_MAX_SIZE < 2 || abs(queue_values[0] - queue_values[1]) < 50) {
        summ += queue_values[0];
        count++;
    }

    for (int i = 1; i < QUEUE_MAX_SIZE - 1; i++) {
        if (abs(queue_values[i] - queue_values[i - 1]) < 50 && abs(queue_values[i] - queue_values[i + 1]) < 50) {
            summ += queue_values[i];
            count++;
        }
    }

    if (QUEUE_MAX_SIZE < 2 || abs(queue_values[QUEUE_MAX_SIZE - 1] - queue_values[QUEUE_MAX_SIZE - 2]) < 50) {
        summ += queue_values[QUEUE_MAX_SIZE - 1];
        count++;
    }

    if (count == 0) {
        return 0.0;
    }

    return summ / count;
}

void setup() {
    Serial.begin(9600);
    for (int i=4; i<=7; i++) {
        pinMode(i, OUTPUT);
        digitalWrite(i, LOW);
    }

    pinMode(FORWARD_TRIG, OUTPUT);
    pinMode(FORWARD_ECHO, INPUT);
    pinMode(RIGHT_TRIG, OUTPUT);
    pinMode(RIGHT_ECHO, INPUT);

    // Инициализация PID
    myPID.SetMode(AUTOMATIC);
    myPID.SetOutputLimits(-50, 100); // Ограничиваем изменение скорости (важно для стабильности)
}

int getDistance(bool isForward) {
    long duration;
    int result;

    if (isForward) {
        digitalWrite(FORWARD_TRIG, LOW);
        delayMicroseconds(2);
        digitalWrite(FORWARD_TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(FORWARD_TRIG, LOW);
        duration = pulseIn(FORWARD_ECHO, HIGH);
    } else {
        digitalWrite(RIGHT_TRIG, LOW);
        delayMicroseconds(2);
        digitalWrite(RIGHT_TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(RIGHT_TRIG, LOW);
        duration = pulseIn(RIGHT_ECHO, HIGH);
    }

    if (duration == 0) {
        return 200;
    }

    result = duration * 0.034 / 2;

    return result > 0 ? result : 0;
}


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


void move_only_right() {
    move(true, 0, true, 200);
}


void move_only_left() {
    move(true, 200, true, 0);
}


void go_forward() {
    move(LEFT_SIDE_FORWARD, speed_left, RIGHT_SIDE_FORWARD, speed_right);
}


void go_backward() {
    move(LEFT_SIDE_BACKWARD, speed_left, RIGHT_SIDE_BACKWARD, speed_right);
}


void turn_left(float ratio) {
    /* ration is between 0 and 1 */
    move(LEFT_SIDE_FORWARD, int(base_speed * ratio), RIGHT_SIDE_FORWARD, base_speed);
}


void turn_right(float ratio) {
    /* ration is between 0 and 1 */
    move(LEFT_SIDE_FORWARD, base_speed, RIGHT_SIDE_FORWARD, int(base_speed * ratio));
}

void turn_left_onspot() {
    move(LEFT_SIDE_BACKWARD, base_speed, RIGHT_SIDE_FORWARD, base_speed);
}

void turn_right_onspot() {
    move(LEFT_SIDE_FORWARD, base_speed, RIGHT_SIDE_BACKWARD, base_speed);
}

void stop() {
    move(0, 0, 0, 0);
}

void loop() {
    forward_distance = getDistance(true);
    right_distance = getDistance(false);

    Serial.print(forward_distance);
    Serial.print(" ");
    Serial.print(right_distance);

    if (forward_distance < STOP_DISTANCE) {
        stop();
        while (forward_distance < 30) {
            forward_distance = getDistance(true);
            turn_left_onspot();
            delay(100);
            stop();
        }
    }

    right_distance = constrain(right_distance, 0, 150);
    enqueue(right_distance);
    if (queue_size < QUEUE_MAX_SIZE - 1) {
        return;
    }

    dequeue();

    average = queueAverage();

    if (average > 80) {

        speed_left = base_speed;
        speed_right = base_speed;

        
        while (right_distance > 40) {
            turn_right_o
            nspot();
            delay(150);
            go_forward();
            delay(100);
            right_distance = getDistance(false);
        }
        Serial.println();
        return;
    }

    input = average;
    myPID.Compute();

    speed_left = base_speed - output * 200;
    speed_right = base_speed + output * 0 ;

    speed_left = constrain(speed_left, 0, 255);
    speed_right = constrain(speed_right, 0, 255) - 20;

    Serial.print(" ");
    Serial.print(output * 200);

    Serial.print(" ");
    Serial.print(speed_left);

    Serial.print(" ");
    Serial.println(speed_right);
    go_forward();

    delay(50); 
}