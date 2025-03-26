#include <Wire.h>

#define LED_PIN 13

int ADXL345 = 0x53;
const float threshold = 0.208;
float X_out, Y_out, Z_out;
float X1, Y1, Z1;

void setup() {
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    Wire.begin();
    Wire.beginTransmission(ADXL345);
    Wire.write(0x2D);
    Wire.write(8);                    // (8)_10 = (0000 1000)_2
    Wire.endTransmission();
}

void checkIsWeightlessness(float x, float y, float z) {
    if (abs(x)+ abs(y) + abs(z) < threshold) {
        Serial.println("\tWEIGHTLESSNESS");
        digitalWrite(LED_PIN, HIGH);
    } else {
        digitalWrite(LED_PIN, LOW);
    }
}

void loop() {
    Wire.beginTransmission(ADXL345);
    Wire.write(0x32);
    Wire.endTransmission(false);
    Wire.requestFrom(ADXL345, 6, true);
    X_out = (Wire.read() | Wire.read() << 8);
    X1 = X_out / 256;
    Y_out = (Wire.read() | Wire.read() << 8);
    Y1 = Y_out / 256;
    Z_out = (Wire.read() | Wire.read() << 8);
    Z1 = Z_out / 256;
    Serial.print(X1);
    Serial.print("\t");
    Serial.print(Y1);
    Serial.print("\t");
    Serial.println(Z1);
    checkIsWeightlessness(X1, Y1, Z1);
    delay(10);
}
