#define led_r 8
#define led_y 9
#define led_g 10


void setup() {
  Serian.begin(9600);
  pinMode(led_r, OUTPUT);
  pinMode(led_y, OUTPUT);
  pinMode(led_g, OUTPUT);
}

void loop() {
  digitalWrite(led_r, HIGH);
  digitalWrite(led_y, HIGH);
  digitalWrite(led_g, HIGH);
}
