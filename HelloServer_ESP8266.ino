/* ESP8266 Simple Server and LED Control
 *  
 * Virgool.io/@farhad3113
 * github.com/farhad3113
 * 
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#ifndef STASSID
#define STASSID "SSID"      // Enter your SSID
#define STAPSK  "PASS"      // Enter your Password
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);    // Server Port

void handleNotFound() {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    server.send(404, "text/plain", message);
}
void led_on(){
  digitalWrite(LED_BUILTIN, LOW);
  server.send(200, "text/plain", "LED is ON!");
}
void led_off(){
  digitalWrite(LED_BUILTIN, HIGH);
  server.send(200, "text/plain", "LED is OFF!");
}

void setup(void) {
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", []() {
    server.send(200, "text/plain", "Hello My Little Server");
  });
  server.on("/on", led_on);
  server.on("/off", led_off); 
  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
