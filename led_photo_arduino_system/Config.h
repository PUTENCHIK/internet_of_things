#define LED_PIN 2
#define PHOTO_PIN A0

const char SUB_MODE = 's';
const char PUB_MODE = 'p';
char mode = PUB_MODE;

// Имя и пароль точки для доступа в интернет
char* ssidAP = "Galaxy_M01";
char* passwordAP = "qwerty1234";

char* mqtt_broker = "broker.emqx.io";
const int mqtt_port = 1883;
