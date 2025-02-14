#define ROW1 2
#define ROW2 3
#define ROW3 4

#define COL1 5
#define COL2 6
#define COL3 7

#define NROWS 3
#define NCOLS 3

// delay for timer between comparing rows of buttons
const int N_DELAY = 1;
// coefficient for value to compare with
const float K_DELAY = 62.496;
// delay between checkings of switch statuses
const int MAIN_DELAY = 50;

const int row_pins[NROWS] = {ROW1, ROW2, ROW3};
const int col_pins[NCOLS] = {COL1, COL2, COL3};
bool btn_pressed[NCOLS] = {false, false, false};

bool btnStatuses[NROWS][NCOLS] = {{false, false, false},
								  {false, false, false},
								  {false, false, false}};

long int btnTimers[NROWS][NCOLS] = {{0, 0, 0},
								    {0, 0, 0},
								    {0, 0, 0}};

bool isChecking = true;
int iRow = 0;
long int timer = millis();

void setup()
{
  	Serial.begin(9600);
  	DDRD = B00000000;
  	// set row pins to OUTPUT, other to INPUT
  	for (int i = 0; i < NROWS; i++) {
      	DDRD |= (1 << row_pins[i]);
    }	
  	// enable PULLUP for column pins
  	for (int i = 0; i < NCOLS; i++) {
      	PORTD |= (1 << col_pins[i]);
    }
  
    cli();
  	// timer 1 with prescaler 256
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    OCR1A = K_DELAY * N_DELAY;
    TCCR1B |= (1 << WGM12);
    TCCR1B = TCCR1B | (1 << CS12);
    TIMSK1 |= (1 << OCIE1A);
    sei();
}

void loop() {}

ISR(TIMER1_COMPA_vect) {
  	if (!isChecking && millis() > timer + MAIN_DELAY) {
    	isChecking = true;
    }
    
    if (isChecking) {
		for (int i = 0; i < NROWS; i++)
          	myDigitalWrite(row_pins[i], 1);
        myDigitalWrite(row_pins[iRow], 0);
      
        for (int icol = 0; icol < NCOLS; icol++)
          	btn_pressed[icol] = myDigitalRead(col_pins[icol]);
        check_states(iRow);
      	iRow += 1;
    }
  
    if (iRow >= NROWS) {
      	iRow = 0;
		isChecking = false;
      	timer = millis();       
    }
}

void myDigitalWrite(int pin, int value) {
    if (value)
        PORTD = PORTD | (1 << pin);
    else PORTD = PORTD & ~(1 << pin);
}

int myDigitalRead(int pin) {
  	return !(PIND & (1 << pin));
}

void check_states(int row_number) {
    for (int icol = 0; icol < NCOLS; icol++) {
        if (btn_pressed[icol] != btnStatuses[row_number][icol]) {
          	btnStatuses[row_number][icol] = btn_pressed[icol];
          	printPressedButtons();
            if (btn_pressed[icol]) {
              	btnTimers[row_number][icol] = millis();
            } else {
             	 printBtnTime(row_number, icol);
            }
        }
    }
}

int btnNumber(int r, int c) {
	return NCOLS*r + c+1;
}

void printPressedButtons() {
  	Serial.print("\nButtons pressed: ");
    for (int i = 0; i < NROWS; i++) {
        for (int j = 0; j < NCOLS; j++) {
          	if (btnStatuses[i][j]) {
              	Serial.print(btnNumber(i, j));
          		Serial.print(", ");
          	}
        }
    }
  	Serial.println();
}

void printBtnTime(int r, int c) {
	Serial.print("Duration btn ");
  	Serial.print(btnNumber(r, c));
  	Serial.print(": ");
    Serial.print(millis() - btnTimers[r][c]);
    Serial.print("ms; started at: ");
    Serial.print(btnTimers[r][c]);
  	Serial.println("ms");
}