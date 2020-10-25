/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp8266-nodemcu-arduino-ide/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <ESP8266WiFi.h>
#include <espnow.h>



//-------------------EMailSender---------------------------
 #include "Arduino.h"
 #include <EMailSender.h>
 const char* ssid = "dbSoft";
 const char* password = "xVf5nTEN";

 uint8_t connection_state = 0;
 uint16_t reconnect_interval = 10000;

//EMailSender emailSend("myriskosarduino@gmail.com", "dbSoft1957");
//EMailSender emailSend("info.dbsoft@gmail.com", "dbSoft1957!@#");
EMailSender emailSend("myriskos@yahoo.gr", "cswmhhgzvrcxoftg");
//EMailSender emailSend("info@dbsoft.gr", "!!MyrJohn!@#");

uint8_t WiFiConnect(const char* nSSID = nullptr, const char* nPassword = nullptr)
{
    static uint16_t attempt = 0;
    Serial.print("Connecting to ");
    if(nSSID) {
        WiFi.begin(nSSID, nPassword);
        Serial.println(nSSID);
    }

    uint8_t i = 0;
    while(WiFi.status()!= WL_CONNECTED && i++ < 50)
    {
        delay(200);
        Serial.print(".");
    }
    ++attempt;
    Serial.println("");
    if(i == 51) {
        Serial.print("Connection: TIMEOUT on attempt: ");
        Serial.println(attempt);
        if(attempt % 2 == 0)
            Serial.println("Check if access point available or SSID and Password\r\n");
        return false;
    }
    Serial.println("Connection: ESTABLISHED");
    Serial.print("Got IP address: ");
    Serial.println(WiFi.localIP());
    return true;
}

void Awaits()
{
    uint32_t ts = millis();
    while(!connection_state)
    {
        delay(50);
        if(millis() > (ts + reconnect_interval) && !connection_state){
            connection_state = WiFiConnect();
            ts = millis();
        }
    }
}
  
void MailYahoo(){

    connection_state = WiFiConnect(ssid, password);
    if(!connection_state)  // if not connected to WIFI
     {
       Awaits();              // constantly trying to connect
     }
    EMailSender::EMailMessage message;
    message.subject = "Esp8266NodeMCU";
    message.message = "Παραβίαση πόρτας ορόφου<br>Arduino.<br>www.dbsoft.gr";

    EMailSender::Response resp = emailSend.send("info.dbsoft@gmail.com", message);
    EMailSender::Response resp2 = emailSend.send("myriskos@yahoo.gr", message);
    Serial.println("Sending status: ");

    Serial.println(resp.status);
    Serial.println(resp.code);
    Serial.println(resp.desc);

    Serial.println(resp2.status);
    Serial.println(resp2.code);
    Serial.println(resp2.desc); 
  
  
}

//---------------------end EMailSender--------------------------------------



// REPLACE WITH RECEIVER MAC Address
// {0xA4, 0xCF, 0x12, 0xDF, 0x58, 0x46} ;  // SEND A
// {0xA4, 0xCF, 0x12, 0xDF, 0x51, 0x1}  ;  // SEND B
// {0x98, 0xF4, 0xAB, 0xB3, 0x8F, 0x94} ;  // SEND C
// {0xA4, 0xCF, 0x32, 0x7A, 0x5, 0x18}  ;  // SEND D
// {0xA4, 0xCF, 0x12, 0xDF, 0x48, 0xFA} ;  // SEND E
// {0x84, 0xF3, 0xEB, 0x1A, 0x3A, 0x88} ;  // SEND Z
// {0xA4, 0xCF, 0x12, 0xDF, 0x48, 0xAA} ;  // RECEIVER

// a4:cf:12:df:58:46    SEND A
// a4:cf:12:df:51:1     SEND B
// 98:f4:ab:b3:8f:94    SEND C
// 2c:f4:32:7a:5:18     SEND D
// a4:cf:12:df:48:fa    SEND E
// 84:f3:eb:1a:3a:88    SEND Z
// A4:CF:12:DF:48:AA    RECEIVER

uint8_t broadcastAddress[] = {0xA4, 0xCF, 0x12, 0xDF, 0x48, 0xAA}; // RECEIVER

 const int buzzerPin = D2;
 const int buttonPin = D1; // 16
 int buttonState = 0;
 int counter = 0;
 bool lSend = false ;

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
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  pinMode( buttonPin,INPUT_PULLUP); 
  pinMode(buzzerPin, OUTPUT); 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

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
}
 
void loop() {

/*
  buttonState = digitalRead(buttonPin);
  if (!buttonState){
    myData.e = false;
    lSend    = false ;
    Serial.println("false");
  }
  if (buttonState){
    myData.e = true;
    beep(440, 500);
    beep(600, 500);
    beep(700, 500);
    Serial.println("true");
    if (!lSend)    {
        MailYahoo();
        lSend = true ;
    }    
  }

*/
  
  // Set values to send
  strcpy(myData.a, "THIS IS A CHAR from B");
  myData.b = random(1,20);
  myData.c = 1.2;
  myData.d = "Prodect1203456789";
  myData.e = false;
Serial.println(myData.a);
Serial.println(myData.b);
Serial.println(myData.c);
Serial.println(myData.d);
Serial.println(myData.e);
Serial.println(WiFi.macAddress());

/*
struct point{
    int x;
    int y;
    float z;
};
struct point my_point1;
struct point my_point2 = {2, 5, 3.7};
my_point2.x = 4;
*/
  // Send message via ESP-NOW
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  delay(2000);
}


//-----------------
void beep(int note, int duration) {
//-----------------  

 //Play tone on buzzerPin
 tone(buzzerPin, note, duration);
  delay(duration);
 
 //Stop tone on buzzerPin
 noTone(buzzerPin);
 delay(50);
 
 //Increment counter
 counter++;
}
