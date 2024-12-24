#include "Config.h"


bool isTransmitting = false;

int timerStage = 0;
int timerStageLimit = 16;

String message = "";
String morse = "";
int morseIndex = -1;
char currentChar = 0;

bool isVoid = false;
int stage = 0;
int stageLimit = 0;

void setup() {
    Serial.begin(9600);

    // TIMER 2
    cli();
    TCCR2A = 0;
    TCCR2B = 0;
    // timer 2 with prescaler 8 (11) = 128 микросекунд; 64 (11 and 10) = 1024 микросекунд
    TCCR2B = TCCR2B | (1 << CS11 << CS10);
    
    TIMSK2 = TIMSK2 | (1 << TOIE2); 
    sei();
}

ISR(TIMER2_OVF_vect) {
    if (timerStage == timerStageLimit) {
        timerStage = 0;
        if (isTransmitting) {
            if (stage >= stageLimit) {
                morseIndex++;
                stage = 0;
                currentChar = morse[morseIndex];
                switch (currentChar) {
                    case point:
                        isVoid = false;
                        stageLimit = 1;
                        break;
                    case dash:
                        isVoid = false;
                        stageLimit = 3;
                        break;
                    case symbolDelimiter:
                        isVoid = true;
                        stageLimit = 1;
                        break;
                    case charDelimiter:
                        isVoid = true;
                        stageLimit = 3;
                        break;
                    case wordDelimiter:
                        isVoid = true;
                        stageLimit = 7;
                        break;
                }
            }
            if (morseIndex == morse.length()) {
                isTransmitting = false;
                stageLimit = 0;
                stage = 0;
            } else {
                Serial.write(isVoid);
                stage++;
            }
        } else {
            Serial.write(1);
        }
    }
    timerStage++;
}

char toLower(char s) {
    return s - 'A' + 'a';
}

String addSymbolDelimeters(String morse) {
    String result = "";
    int l = morse.length();
    for (int i = 0; i < l; i++) {
        char symbol = morse[i];
        char delimiter = i == 0 ? 0 : symbolDelimiter;
        result += String(delimiter) + symbol;
    }
    return result;
}

String messageToMorse(String message) {
    String morse = "";
    int l = message.length();
    bool previousSpace = false;
    if (l) {
        for (int i = 0; i < l; i++) {
            char s = message[i];
            if (s == 32) {
                // space
                morse += wordDelimiter;
                previousSpace = true;
            }
            else {
                char delimiter = (i == 0 or previousSpace ? 0 : charDelimiter);
                previousSpace = false;
                if (s >= 48 and s <= 57) {
                    // 0-9
                    morse += delimiter + addSymbolDelimeters(numbers[s - '0']);
                } else {
                    // any other symbol
                    if (s >= 65 and s <= 90) {
                        // A-Z -> a-z
                        s = toLower(s);
                    }
                    if (s >= 97 and s <= 122) {
                        // a-z
                        morse += delimiter + addSymbolDelimeters(letters[s - 'a']);
                    }
                }
            }
        }
    }
    return morse;
}

void loop() {
    if (Serial.available()) {
        message = Serial.readString();
        morse = messageToMorse(message);
        if (debug) {
            Serial.println("\nMorse: " + morse);
            Serial.println("Morse length: " + String(morse.length()));
        }
        isTransmitting = true;
        morseIndex = -1;
    }
}
