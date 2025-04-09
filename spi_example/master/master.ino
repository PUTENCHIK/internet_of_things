#include<SPI.h>

#define LED A0           
#define ipbutton 2

const int DELAY = 100;
long int timer;
int buttonvalue;
int x;
void setup (void) {
    Serial.begin(115200); 
    
    pinMode(ipbutton, INPUT); 
    pinMode(LED, OUTPUT); 
    
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV8);
    digitalWrite(SS, HIGH);
    timer = millis();
}

void loop(void) {

    if (millis() > timer + DELAY) {
        byte Mastereceive;          
        buttonvalue = digitalRead(ipbutton); 
        Serial.println(buttonvalue);
        digitalWrite(SS, LOW);
        Mastereceive = SPI.transfer(buttonvalue);
        digitalWrite(SS, HIGH);

        if (buttonvalue) {
            Serial.print("Received: ");
            Serial.println(Mastereceive);
            analogWrite(LED, Mastereceive);
        }
        else {
            analogWrite(LED, 0);
        }

        timer = millis();
    }

    
    // delay(1000);
}