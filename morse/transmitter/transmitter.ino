
struct Morze {
    char name;
    String value;
};


Morze symbols[] = {
    {'A', "._"},
    {'B', "_..."},
    {' ', " "},
};
int symbolsLength = 3;

String inputString;
String message = "";


String getValue(char key) {
    for (int i = 0; i < symbolsLength; i++) {
        if (symbols[i].name == key) {
            return symbols[i].value;
        }
    }
    return "=";
}



void translateToMorse(String inputString) {
    
    for (int i = 0; i < inputString.length(); i++) {
        message += getValue(inputString.charAt(i));
    }
}


void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
}


void loop() {
    if (Serial.available() > 0) {
        message = "";
        inputString = Serial.readString();

        Serial.println(inputString);

        translateToMorse(inputString);
        Serial.println(message);
    }

}
