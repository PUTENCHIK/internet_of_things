#include "Config.h"

#define PIN_DATA 6
#define PIN_LETCH 4
#define PIN_CLOCK 3
#define PIN_RES A0

int timerStage = 0;
int timerStageLimit = timerStagesDefault;
int timerStageLimitOld = timerStagesDefault;
long int timerStageTimer;

bool transferStarted = false;
int sameSignalAmount = 0;
int lastSignal = 1;

String message = "";
int messageIndex = -1;
String currentCharCode = "";

long int timer;
int displayStage = 1;
int displayAmount = 8;
byte displayValue = B11111111;

void setup() {
    DDRD = B01011000;         // pins 6, 4, 3
    Serial.begin(9600);

    // TIMER 2
    cli();
    TCCR2A = 0;
    TCCR2B = 0;
    // timer 2 with prescaler 8 (11) = 128 микросекунд; 64 (11 and 10) = 1024 микросекунд
    TCCR2B = TCCR2B | (1 << CS11 << CS10);
    
    TIMSK2 = TIMSK2 | (1 << TOIE2); 
    sei();

    timer = millis();
    timerStageTimer = millis();
}

String charFromCode(String code) {
    for (int i = 0; i < 10; i++) {
        if (code == numbers[i]) {
            char a = i + '0';
            return String(a);
        }
    }
    for (int i = 0; i < 26; i++) {
        if (code == letters[i]) {
            char a = i + 'a';
            return String(a);
        }
    }
    return String('?');
}

ISR(TIMER2_OVF_vect) {
    if (timerStage == timerStageLimit) {
        timerStage = 0;

        int value = Serial.read();
        if (!transferStarted and value == 0) {
            transferStarted = true;
        }
        
        if (transferStarted) {
            if (value == 0 or value == 1) {
                if (value != lastSignal) {
                    if (lastSignal == 0) {
                        switch (sameSignalAmount) {
                            case 1:
                                currentCharCode += point;
                                break;
                            case 3:
                                currentCharCode += dash;
                                break;
                        }
                    } else {
                        switch (sameSignalAmount) {
                            case 3:
                                message += charFromCode(currentCharCode);
                                currentCharCode = "";
                                break;
                            case 7:
                                message += charFromCode(currentCharCode) + " ";
                                currentCharCode = "";
                                break;
                        }
                    }
                }

                sameSignalAmount = value == lastSignal ? sameSignalAmount+1 : 1;
                lastSignal = value;

                if (value == 1 and sameSignalAmount >= 8) {
                    transferStarted = false;
                    message += charFromCode(currentCharCode);
                    Serial.println("Final message: " + message);
                    message += " ";
                    currentCharCode = "";
                }
            }
        }
    }
    timerStage++;
}

byte getDisplayValue(char s) {
    if (s == ' ') {
        // space
        return displaySpace;
    } else if (s >= 48 and s <= 57) {
        // 0-9
        return displayNumbers[s - '0'];
    } else {
        // a-z
        int index = s - 'a';
        return displayLetters[index < 26 ? index : 25];
    }
}

void display() {
    digitalWrite(PIN_LETCH, 0);
    switch (displayStage) {
        case 1:
            displayStage++;
            break;
        case 2:
            digitalWrite(PIN_DATA, displayValue%2);
            displayStage++;
            displayValue >>= 1;
            break;
        case 3:
            digitalWrite(PIN_CLOCK, 1);
            displayStage++;
            break;
        case 4:
            digitalWrite(PIN_CLOCK, 0);
            displayStage++;
            break;
        case 5:
            digitalWrite(PIN_DATA, 0);
            displayAmount++;
            displayStage = 1;
            break;
    }
}

void loop() {
     int resValue = analogRead(PIN_RES);
     resValue = resValue > 1010 ? 1023 : resValue;
     
     int step = resValue / (1024 / (((timerStagesMax - timerStagesMin) / timerStagesDelta) + 1));
     int resValueProcessed = timerStagesMin + (timerStagesMax - timerStagesMin) * step / 3;
     resValueProcessed = resValueProcessed > timerStagesMax ? timerStagesMax : resValueProcessed;
     if (resValueProcessed != timerStageLimitOld and millis() > timerStageTimer + delayTimerStageChange) {
        Serial.print("timerStageLimit updated: ");
        Serial.println(resValueProcessed);
        timerStageLimit = resValueProcessed;
        timerStageLimitOld = resValueProcessed;
        timerStageTimer = millis();
     }

    if (message.length()) {
        if (millis() > timer + delayBetweenChars) {
            timer = millis();
            messageIndex++;
            if (messageIndex >= message.length()) {
                message = "";
                messageIndex = -1;
            }
        }

        if (messageIndex < message.length()) {
            // Serial.print(messageIndex);
            // Serial.println(") " + n);
            if (displayAmount == 8) {
                displayValue = getDisplayValue((char)message[messageIndex]);
                digitalWrite(PIN_LETCH, 1);
                displayAmount = 1;
            }
            display();
        }
    }
}
