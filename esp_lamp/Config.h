// Состояния приложения и индикации
#define own_ap_state 1              // Поднята своя точка доступа
#define int_mqtt_state 2            // Есть подключение к интернету и брокеру
#define only_int_state 3            // Есть подключение к интернету, но не к брокеру

// Название и пароль собственной точки доступа
String ssidAP = "ESP_GLEBBY_MAXY";
String passwordAP = "qwerty1234";

char* ssidCLI = "Galaxy_M01";
char* passwordCLI = "qwerty1234";

long int timer = millis();
int LED_PIN = 2;
int led_stage = 1;
bool isStageChanged = false;
const int long_led_delay = 2000;
const int short_led_delay = 500;

int state = int_mqtt_state;
