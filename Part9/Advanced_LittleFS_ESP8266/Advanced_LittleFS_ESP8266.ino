/* ESP8266 Advanced LittleFS
 *  
 * Virgool.io/@farhad3113
 * github.com/farhad3113
 * 
 */
#include <FS.h>
#include <LittleFS.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

const char* ssid = "YOUR SSID";
const char* password = "YOUR PASSWORD";

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

    if (!LittleFS.begin()) {
      Serial.println("LittleFS mount failed");
      return;
    }

    server.on("/login", HTTP_GET, [](AsyncWebServerRequest *request){
      if(!request->authenticate("user", "pass"))
          return request->requestAuthentication();
      request->redirect("/");
    });
    
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        if(!request->authenticate("user", "pass"))
            request->redirect("/login");
        request->send(LittleFS, "index.html", "text/html");
    });
    
    server.on("/img", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "img.png", "image/png", true);
    });
    server.on("/on", HTTP_GET, led_on);
    server.on("/off", HTTP_GET, led_off);
    server.onNotFound(handleNotFound);
    
    server.begin();
}

void loop() {}
