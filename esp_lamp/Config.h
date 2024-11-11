// Состояния приложения и индикации
#define own_ap_state 1              // Поднята своя точка доступа
#define int_mqtt_state 2            // Есть подключение к интернету и брокеру
#define only_int_state 3            // Есть подключение к интернету, но не к брокеру

// Название и пароль собственной точки доступа
String ssidAP = "ESP_GLEBBY_MAXY";
String passwordAP = "qwerty1234";

const int max_ssid_name = 20;
const int max_password_name = 20;
char ssidCLI[max_ssid_name];
char passwordCLI[max_password_name];


long int timer = millis();
int LED_PIN = 2;
int led_stage = 1;
bool isStageChanged = false;

const int long_led_delay = 1000;
const int short_led_delay = 250;

int state = own_ap_state;
bool isStateChanged = false;
