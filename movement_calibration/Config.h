#define MCE_RX 10
#define MCE_TX 11

#define LEFT_DIRECTION 7
#define LEFT_SPEED 6
#define RIGHT_SPEED 5
#define RIGHT_DIRECTION 4

#define LEFT_SIDE_FORWARD HIGH
#define RIGHT_SIDE_FORWARD LOW
#define LEFT_SIDE_BACKWARD LOW
#define RIGHT_SIDE_BACKWARD HIGH

#define FORWARD_BUTTON 'F'
#define RIGHT_BUTTON 'R'
#define BACKWARD_BUTTON 'B'
#define LEFT_BUTTON 'L'

#define T_BUTTON 'T'        // triangle (up)
#define S_BUTTON 'S'        // square (left)
#define X_BUTTON 'X'        // X (down)
#define O_BUTTON 'O'        // O (right)

#define START_BUTTON 'A'
#define PAUSE_BUTTON 'P'
#define PULLUP '0'          // button released

const int PINS_AMOUNT = 4;
const int PINS[PINS_AMOUNT] = {RIGHT_DIRECTION,
                               RIGHT_SPEED,
                               LEFT_SPEED,
                               LEFT_DIRECTION};

const int MODES_AMOUNT = 3;
