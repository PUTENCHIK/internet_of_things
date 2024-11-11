#include <ESP8266WebServer.h>

ESP8266WebServer server(5000);    

void handleRoot() {
  server.send(200, 
              "text/html", 
              "<form action='/config' method='POST'><input name='name' type='text' placeholder='Enter WiFi name'> <br> <input name='password' type='password' placeholder='Enter WiFi password'> <br> <input type='submit' value='Send'></form>");
}

void handleConfig() {                          
  String name = server.arg("name");
  String password = server.arg("password");
  Serial.println("New name: " + name);
  Serial.println("New password: " + password);

  name.toCharArray(ssidCLI, sizeof(ssidCLI));
  password.toCharArray(passwordCLI, sizeof(passwordCLI));
  EEPROM.put(0, ssidCLI);
  EEPROM.put(max_password_name, passwordCLI);
  EEPROM.commit();

  state = only_int_state;
  
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); 
}

void server_init() {
  server.on("/", HTTP_GET, handleRoot);     
  server.on("/config", HTTP_POST, handleConfig);  
  server.onNotFound(handleNotFound);        

  server.begin();                          
  Serial.println("HTTP server started");    
}
