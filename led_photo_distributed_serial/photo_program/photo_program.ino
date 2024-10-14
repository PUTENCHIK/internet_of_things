#define p_pin A0                // аналоговый пин для фоторезистора

const char PHOTO = 'p';         // символ для команды, записывающей значение фоторезистора

void setup() {
    Serial.begin(9600);
    pinMode(p_pin, INPUT);
}

void printValue() {
    int value = analogRead(p_pin);
    value /= 4;                         // приведение значения фоторезистора к диапазону (0 -> 255)
    Serial.write(value);                // отправка значение в виде единичного байта
}

void loop() {
    if (Serial.available() > 0) {
        char command = Serial.read();
        switch (command) {
            case PHOTO:
                printValue();
                break;
            default:
                break;
        }
    }
}
