#define MCE_RX 10
#define MCE_TX 11

#define PIN_DATA 8
#define PIN_LETCH 9
#define PIN_CLOCK 12

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
#define O_BUTTON 'C'        // C (right)

#define START_BUTTON 'A'
#define PAUSE_BUTTON 'P'
#define PULLUP '0'          // button released

const int DEFAULT_SPEED = 200;
const int PINS_AMOUNT = 7;
const int PINS[PINS_AMOUNT] = {RIGHT_DIRECTION,
                               RIGHT_SPEED,
                               LEFT_SPEED,
                               LEFT_DIRECTION,
                               PIN_DATA,
                               PIN_LETCH,
                               PIN_CLOCK};

enum class CalibrationObject {MoveForward, MoveBackward, MoveRight, MoveLeft};

const int DEFAULT_DIRECTION_SIDE_INDEX = 0;
const int DIRECTION_SIDE_AMOUNT = 4;
const int DIRECTION_SIDE_COEF[4][4] = {
    {LOW, HIGH, LOW, HIGH},  // right_forward, right_backward, left_forward, left_backward
    {HIGH, LOW, LOW, HIGH},
    {HIGH, LOW, HIGH, LOW},
    {LOW, HIGH, HIGH, LOW}
};

const int MODES_AMOUNT = 3;
const int SPEED_SETTINGS_AMOUNT = 3;
const int TURNING_SETTINGS_AMOUNT = 4;
const int SEGMENT_DELAY = 1000;
//                         .GEDCBAF
const byte NUMBERS[10] = {B11000000,   // 0
                          B11110011,   // 1
                          B10001001,   // 2
                          B10100001,   // 3
                          B10110010,   // 4
                          B10100100,   // 5
                          B10000100,   // 6
                          B11110001,   // 7
                          B10000000,   // 8
                          B10100000,   // 9
};
