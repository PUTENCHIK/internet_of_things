#include <Wire.h>
int ADXL345 = 0x53;
float X_out, Y_out, Z_out;
float X1, Y1, Z1;

void setup() {
    Serial.begin(9600);
    Wire.begin();
    Wire.beginTransmission(ADXL345);
    Wire.write(0x2D);
    Wire.write(8);
    Wire.endTransmission();
}

void loop() {
    Wire.beginTransmission(ADXL345);
    Wire.write(0x32);
    Wire.endTransmission(false);
    Wire.requestFrom(ADXL345, 6, true);
    X_out = ( Wire.read() | Wire.read() << 8);
    Y1 = X_out / 256;
    Y_out = ( Wire.read() | Wire.read() << 8);
    X1 = Y_out / 256;
    Z_out = ( Wire.read() | Wire.read() << 8);
    Z1 = Z_out / 256;

    Serial.print(X1);
    Serial.print("    ");
    Serial.print(Y1);
    Serial.print("    ");
    Serial.println(Z1);
    delay(100);
}
