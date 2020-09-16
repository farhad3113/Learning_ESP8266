/* ESP8266 Simple Auth
 *  
 * Virgool.io/@farhad3113
 * github.com/farhad3113
 * 
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

const char* ssid = "YOUR SSID";
const char* password = "YOUR PASSWORD";

String getLedStatus(){
  if(digitalRead(LED_BUILTIN)) return "LED is OFF!";
  else return "LED is ON!";
}
void handleNotFound(AsyncWebServerRequest *request) {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += request->url();  
    request->send(404, "text/plain", message);
}
void led_on(AsyncWebServerRequest *request){
    if(!request->authenticate("user", "pass")){
        request->redirect("/login");
    } else {
        digitalWrite(LED_BUILTIN, LOW);
        request->redirect("/");
    }   
}
void led_off(AsyncWebServerRequest *request){
    if(!request->authenticate("user", "pass")){
        request->redirect("/login");
    } else {
        digitalWrite(LED_BUILTIN, HIGH);
        request->redirect("/");
    } 
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    
    Serial.begin(115200);
    Serial.println("");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.print("\nIP Address: ");
    Serial.println(WiFi.localIP());

    server.on("/login", HTTP_GET, [](AsyncWebServerRequest *request){
      if(!request->authenticate("user", "pass"))
          return request->requestAuthentication();
      request->redirect("/");
    });
    
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        if(!request->authenticate("user", "pass"))
            request->redirect("/login");
        String index = "<html><head><title>My Async WebServer</title></head>";
        index += "<body><center>Hello, World</center></br>";
        index += "<center>" + getLedStatus() + "<center></br>";
        index += "<button><a href=\"/on\">ON</a></button>";
        index += "<button><a href=\"/off\">OFF</a></button>";
        index += "</body></html>";
        request->send(200, "text/html", index);
    });

    server.on("/on", HTTP_GET, led_on);
    server.on("/off", HTTP_GET, led_off);
    server.onNotFound(handleNotFound);
    
    server.begin();
}

void loop() {}
