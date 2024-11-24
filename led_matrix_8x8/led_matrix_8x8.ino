#define r1 0            // red right
#define r2 12           // gray right
#define r3 11           // puple right
#define r4 2            // black left
#define r5 10           // white left
#define r6 1            // yellow left
#define r7 A1           // brawn left
#define r8 A0           // orange left

#define c1 7            // gray right
#define c2 6            // white right
#define c3 8            // brawn right
#define c4 13           // yellow right short
#define c5 4            // yellow right long
#define c6 9            // brawn left
#define c7 3            // gray left
#define c8 5            // blue left

const int n = 8;

int rows[n] = {r1, r2, r3, r4, r5, r6, r7, r8}; 
int columns[n] = {c1, c2, c3, c4, c5, c6, c7, c8};


int heart[n][n] = {{0, 1, 1, 0, 0, 1, 1, 0},
                   {1, 1, 1, 1, 1, 1, 1, 1},
                   {1, 1, 1, 1, 1, 1, 1, 1},
                   {1, 1, 1, 1, 1, 1, 1, 1},
                   {0, 1, 1, 1, 1, 1, 1, 0},
                   {0, 0, 1, 1, 1, 1, 0, 0},
                   {0, 0, 1, 1, 1, 1, 0, 0},
                   {0, 0, 0, 1, 1, 0, 0, 0}};

int human[n][n] = {{0, 0, 0, 1, 1, 0, 0, 0},
                   {0, 0, 0, 1, 1, 0, 0, 0},
                   {1, 1, 1, 1, 1, 1, 1, 1},
                   {1, 0, 1, 1, 1, 1, 0, 1},
                   {1, 0, 1, 1, 1, 1, 0, 1},
                   {0, 0, 1, 0, 0, 1, 0, 0},
                   {0, 0, 1, 0, 0, 1, 0, 0},
                   {0, 0, 1, 0, 0, 1, 0, 0}};


void showFrame(int numberRow, int arr[8]) {
    digitalWrite(rows[numberRow], 1);
    for (int i = 0; i < n; i++) {
        digitalWrite(columns[i], !arr[0]);
    }
}


void reset() {
    for (int i = 0; i < n; i++) {
        digitalWrite(rows[i], 0);
    }
    for (int i = 0; i < n; i++) {
        digitalWrite(columns[i], 0);
    }
}


void setup() {
    for (int i = 0; i < n; i++) {
        pinMode(rows[i], OUTPUT);
    }
    for (int i = 0; i < n; i++) {
        pinMode(columns[i], OUTPUT);
    }
}

void loop() {
    for (int i = 0; i < n; i++) {
        showFrame(i, heart[i]);
//        showFrame(i, human[i]);
        delay(1);
        reset();
        delay(1);
    }
}
