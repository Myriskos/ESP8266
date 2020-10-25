
// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <espnow.h>
uint8_t broadcastAddress[] = {0xA4, 0xCF, 0x12, 0xDF, 0x48, 0xAA}; // RECEIVER
// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  String d ;
  bool e;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
    Serial.print("Last Packet Send Status: ");
    if (sendStatus == 0){
     Serial.println("Delivery success");
    }
    else{
    Serial.println("Delivery fail");
   }
}



//--------------------------
// DHT settings starts
#include "DHT.h"
#define DHTPIN 13    // what digital pin we're connected to
// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
float tValue;// 
// ****** DHT settings end (Robojax.com )
//-------------------------
#ifndef STASSID
#define STASSID "dbSoft" // Your WiFi SSID
#define STAPSK  "xVf5nTEN" //Your WiFi password
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
 
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
 
AsyncWebSocketClient * globalClient = NULL;
 
void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
 
  if(type == WS_EVT_CONNECT){
 
     Serial.println("Websocket client connection received");
     globalClient = client;
     
  } else if(type == WS_EVT_DISCONNECT){
     
     Serial.println("Websocket client connection finished");
     globalClient = NULL;
 
  }
}

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
// Variable to store the HTTP request
String header;
// Auxiliar variables to store the current output state
String output5State = "off";
String output4State = "off";

// Assign output variables to GPIO pins
const int output5 = 12;
const int output4 = 14;
float ctherm ;


void setup(){
  Serial.begin(115200);
 // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  
// config static IP
  WiFi.setAutoConnect(false);   // Not working by its own
  WiFi.disconnect();  //Prevent connecting to wifi based on previous configuration
  IPAddress ip(192, 168, 1, 65);
  IPAddress gateway(192, 168, 1, 1);
  Serial.print(F("Setting static ip to : ")); // Adding these lines to see the IP that the device is getting
  Serial.println(ip);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress dns(8, 8, 8, 8);
  WiFi.config(ip, gateway, subnet, dns);
//  WiFi.config(ip, subnet, gateway, dns); 
//------------------  Init ESP-NOW   --------
 
//---------------------------------------- 
 
 // Initialize the output variables as outputs
  pinMode(output5, OUTPUT);
  pinMode(output4, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output4, LOW);
  digitalWrite(output5, LOW);

  
 dht.begin();
  if(!SPIFFS.begin()){
     Serial.print("An Error has occurred while mounting SPIFFS");
     return;
  }


 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println(WiFi.localIP());
 
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);



   
  server.on("/R0", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", on_on().c_str()  );
  });
  server.on("/R1", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", on_off().c_str()  );
  });

   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/ws.html", "text/html");
  });
   
   server.on("/on_led", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(output5, HIGH);
    request->send(SPIFFS, "/ws.html", "text/html");
  });

   server.on("/off_led", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(output5, LOW);
    request->send(SPIFFS, "/ws.html", "text/html");
  });

   server.on("/blue_on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(output4, HIGH);
    request->send(SPIFFS, "/ws.html", "text/html");
  });

   server.on("/Blue_off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(output4, LOW);
    request->send(SPIFFS, "/ws.html", "text/html");
  });

 server.on("/ws", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/ws.html", "text/html");
  });
server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });  
  server.on("/bg1.jpg", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/bg1.jpg", "text/jpg");
  });  
  server.on("/main.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/main.js", "text/js");
  });  
  server.begin();


//--------------------  Init ESP-NOW  ---------------------
 // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

//----------------------------------------------------
  
}
 
void loop(){

   float ctherm = dht.readTemperature();// Read temperature as Celsius (the default)
   float h = dht.readHumidity();// Reading humidity 
   float f = dht.readTemperature(true);// Read temperature as Fahrenheit (isFahrenheit = true)
   Serial.println(ctherm);
  // tValue =ctherm;
   delay(1000);// change this to larger value (1000 or more) if you don't need very often reading
  
  
   if(globalClient != NULL && globalClient->status() == WS_CONNECTED){
      //String randomNumber = String(random(0,20));
      String randomNumber = String(ctherm) + "  Υγρασία:" + String(h) ;
      globalClient->text(randomNumber);
   }

   //-------------------------------------------
    // Set values to send
  strcpy(myData.a, "THIS IS ATemperature");
  myData.b = h ; //random(1,20);
  myData.c = ctherm;
  myData.d = "6977996927";
  myData.e = false;
  // Send message via ESP-NOW
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

   //------------------------------------------=
   
   
   delay(1000);
}


String on_on(){
 
}
String on_off(){
 
 }
