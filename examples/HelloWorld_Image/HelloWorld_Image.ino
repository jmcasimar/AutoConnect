/*
  HelloWorld.ino, Example for the AutoConnect library.
  Copyright (c) 2019, Hieromon Ikasamo
  https://github.com/Hieromon/AutoConnect

  This software is released under the MIT License.
  https://opensource.org/licenses/MIT
*/
/*
  To experience this example, upload the JSON file which is style.json
  from the data folder. Its file contains the attributes for the Caption
  of AutoConnectText. You can change the elements for your realization.
*/

#if defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
typedef ESP8266WebServer WEBServer;
#elif defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>
typedef WebServer WEBServer;
#endif
#include <FS.h>
#include <AutoConnect.h>

#define HELLO_URI   "/hello"
#define PARAM_STYLE "/style.json"

// Declare AutoConnectText with only a value.
// Qualify the Caption by reading style attributes from the SPIFFS style.json file.
ACImage(Caption, "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");

//AutoConnectAux for the custom Web page.
AutoConnectAux helloPage(HELLO_URI, "Hello", true, { Caption });
AutoConnect portal;

// JSON document loading buffer
String ElementJson;

// Redirects from root to the hello page.
void onRoot() {
  WEBServer& webServer = portal.host();
  webServer.sendHeader("Location", String("http://") + webServer.client().localIP().toString() + String(HELLO_URI));
  webServer.send(302, "text/plain", "");
  webServer.client().flush();
  webServer.client().stop();
}

// Load the attribute of the element to modify at runtime from external.
String onHello(AutoConnectAux& aux, PageArgument& args) {
  aux.loadElement(ElementJson);
  return String();
}

// Load the element from specified file in SPIFFS.
void loadParam(const char* fileName) {
  SPIFFS.begin();
  File param = SPIFFS.open(fileName, "r");
  if (param) {
    ElementJson = param.readString();
    param.close();
  }
  SPIFFS.end();
}

void setup() {
  delay(1000);
  Serial.begin(115200);

  loadParam(PARAM_STYLE);     // Pre-load the element from JSON.
  helloPage.on(onHello);      // Register the attribute overwrite handler.
  portal.join(helloPage);     // Join the hello page.
  portal.begin();
  
  WEBServer& webServer = portal.host();
  webServer.on("/", onRoot);  // Register the root page redirector.
}

void loop() {
  portal.handleClient();
}
