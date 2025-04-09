#include<SPI.h>

#define RES A0

volatile boolean received;
volatile byte slaveReceived, slaveSend;
const int DELAY = 100;
long int timer;

void setup() {
    Serial.begin(115200);
    pinMode(MISO, OUTPUT);
    SPCR |= _BV(SPE);                       // SPI into Slave Mode
    received = false;
    SPI.attachInterrupt();                  // SPI interrupt ON
    timer = millis();
}

ISR (SPI_STC_vect) {
  slaveReceived = SPDR;
  received = true;
}

void loop() {
    if (millis() > timer + DELAY) {
        Serial.print("Recevied value: ");
        Serial.println(slaveReceived);
        if (received && slaveReceived) {
            int value = analogRead(RES);
            Serial.print("PHOTO value: ");
            Serial.println(value);
            SPDR = value;
            received = false;
        }
        timer = millis();
    }
}
