#define LED_PIN 2
#define PHOTO_PIN A0

const char SUB_MODE = 's';
const char PUB_MODE = 'p';
char mode = SUB_MODE;

String ssidAP = "ESP_MAXY_GLEBBY"; // имя контроллера и точки доступа
String passwordAP = "test12345"; // пароль точки доступа

// Имя и пароль точки для доступа в интернет
char* ssidCLI = "Glebiks";
char* passwordCLI = "toster123";

char* mqtt_broker = "broker.emqx.io";
const int mqtt_port = 1883;

String topic_template = "system/photo/";
String topic = "negr";

int threshold = 1024 / 2;