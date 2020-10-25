/*
   -- New project --
   
   This source code of graphical user interface 
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 2.4.3 or later version 
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                   
     - for ANDROID 4.5.1 or later version;
     - for iOS 1.4.1 or later version;
    
   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.    
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP8266WIFI_LIB_CLOUD
#include <ESP8266WiFi.h>
#include <RemoteXY.h>
 
#include <EMailSender.h> 
 //-------auto connect---------------------------
        #include <DNSServer.h>
        #include <ESP8266WebServer.h>
        #include <ESP8266WiFiMulti.h>
        #include <WiFiManager.h>
        ESP8266WiFiMulti WiFiMulti;
    
        char con_ssid[32]; //will hold the ssid of the wlan to which it is connectd
        char con_psk[64];  // ^^ private shared key for the wlan (password)
 //------- end auto connect-------------------------

        
// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID     "" // "dbSoft"
#define REMOTEXY_WIFI_PASSWORD "" // "xVf5nTEN"
#define REMOTEXY_CLOUD_SERVER "cloud.remotexy.com"
#define REMOTEXY_CLOUD_PORT 6376
#define REMOTEXY_CLOUD_TOKEN "b621822daa7fe9f0f5af8566ae85a629"


// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,1,0,1,1,156,0,10,13,0,
  67,4,1,11,57,6,2,26,51,67,
  4,1,19,57,6,2,26,51,67,4,
  1,27,57,6,2,26,51,129,0,32,
  1,34,6,17,206,163,207,133,206,189,
  206,177,206,179,206,181,207,129,206,188,
  207,140,207,130,0,67,5,63,27,35,
  6,2,26,51,67,5,1,46,97,6,
  2,26,51,65,6,90,12,9,9,2,
  0,64,11,22,11,2,26,8,31,67,
  108,111,115,101,0,79,112,101,110,0,
  129,0,27,57,50,3,17,119,119,119,
  46,100,98,115,111,102,116,46,103,114,
  32,206,156,207,133,207,129,206,175,207,
  131,206,186,206,191,207,130,32,206,153,
  207,137,206,172,206,189,206,189,206,183,
  207,130,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t switch_1; // =1 if switch ON and =0 if OFF 

    // output variables
  char text_1[51];  // string UTF8 end zero 
  char text_2[51];  // string UTF8 end zero 
  char text_3[51];  // string UTF8 end zero 
  char text_4[51];  // string UTF8 end zero 
  char text_5[51];  // string UTF8 end zero 
  uint8_t led_1_r; // =0..255 LED Red brightness 
  uint8_t led_1_g; // =0..255 LED Green brightness 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////
#include <EEPROM.h>
//#define PIN_SWITCH_1 D3
#include <espnow.h>

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
// 98:f4:ab:b3:8f:b8    SEND C
// 2c:f4:32:7a:5:18     SEND D
// a4:cf:12:df:48:fa    SEND E
// 84:f3:eb:1a:3a:88    SEND Z
// A4:CF:12:DF:48:AA    RECEIVER

const int buttonPin   = D3; // Button ON - OFF
int       buttonState = 0;

//ορισμος εξοδων ρελε
 const int Relay_01 = D1 ;
 const int Relay_02 = D2 ;

 unsigned long previousMillisDoor = 0; 
 const long intervalDoor = 1000;
 int ledStateDoor = LOW; 

 bool trap_A = false ;
 bool trap_B = false ;
 bool trap_C = false ;
 bool trap_D = false ;
 bool trap_Z = false ;
  
typedef struct {
    char a[32];
    int b;
    float c;
    String d;
    bool e;
} struct_message;
// Create a struct_message called myData
struct_message myData;

String mac_esp;

// Set your new MAC Address
uint8_t newMACAddress[] = {0xA4, 0xCF, 0x12, 0xDF, 0x48, 0xAA};
 

// Callback function that will be executed when data is received

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Char: ");
  Serial.println(myData.a);
  Serial.print("Int: ");
  Serial.println(myData.b);
  Serial.print("Float: ");
  Serial.println(myData.c);
  Serial.print("String: ");
  Serial.println(myData.d);
  Serial.print("Bool: ");
  Serial.println(myData.e);
 
  mac_esp = macToStr(mac) ;
  
  Serial.println( macToStr(mac) ); 
  
  Serial.println();
  
 //strcpy (str3,"copy successful");
  mac_esp.toCharArray( RemoteXY.text_1,18 );
  dtostrf( myData.c  , 0, 1, RemoteXY.text_2);
  strcpy (RemoteXY.text_3 , myData.a ) ;


   if  ( macToStr(mac) == "a4:cf:12:df:58:46"  && myData.e  ) {
       trap_A = true ;
   }
    if  ( macToStr(mac) == "84:f3:eb:1a:3a:88"  && myData.e  ) {
       trap_Z = true ;
   }
   if  ( macToStr(mac) == "98:f4:ab:b3:8f:b8"  && myData.e  ) {
       trap_C = true ;
   }
   
 
 if (buttonState) {
                            
    if ( trap_A ){
        strcpy (RemoteXY.text_5 , "Παραβίαση πορτας A" ) ;
        open_Door();
    }
    else if ( trap_B ) {
        strcpy (RemoteXY.text_5 , "Παραβίαση πορτας B" ) ;
        open_Door();
    }
    else if ( trap_C ) {
        strcpy (RemoteXY.text_5 , "Παραβίαση πορτας C" ) ;
        open_Door();
    }
     else if ( trap_Z ) {
        strcpy (RemoteXY.text_5 , "Παραβίαση πορτας Z" ) ;
        open_Door();
    }
    else {  
        strcpy (RemoteXY.text_5 , "Ειστε ασφαλείς  " ) ;
        digitalWrite(Relay_01,HIGH);
        digitalWrite(Relay_02,HIGH);
    }
 } 
 else {
     strcpy (RemoteXY.text_5 , "     " ) ;
     digitalWrite(Relay_01,HIGH);
     digitalWrite(Relay_02,HIGH);
     trap_A = false ;
     trap_B = false ;
     trap_C = false ;
     trap_D = false ;
     trap_Z = false ;
 }
}

//---------------------------------------
void setup(){
//---------------------------------------
  // Initialize Serial Monitor
  Serial.begin(115200);
 // pinMode( buttonPin, INPUT_PULLUP);
  /*ορισμος pin  εξοδων για ρελε */
  pinMode(Relay_01,OUTPUT);
  pinMode(Relay_02,OUTPUT);

  digitalWrite(Relay_01,HIGH);
  digitalWrite(Relay_02,HIGH);

 //  pinMode (PIN_SWITCH_1, OUTPUT);

   RemoteXY.led_1_r = 0 ;
   RemoteXY.led_1_g = 200 ; 
   RemoteXY.switch_1= 0 ;  

     EEPROM.begin(512);
     EEPROM.get(10,buttonState); 
    if ( buttonState > 0 ) {
       buttonState = 1 ;
       strcpy (RemoteXY.text_4 , "Ενεργος" ) ; 
       RemoteXY.led_1_r = 0 ;
       RemoteXY.led_1_g = 200 ;
       RemoteXY.switch_1 = 1;
     // 
     //  RemoteXY_sendInputVariables ();
    }  
    else {
      buttonState = 0 ;
      strcpy (RemoteXY.text_4 , "Μη Ενεργος" ) ;    
      RemoteXY.led_1_r = 200 ;
      RemoteXY.led_1_g = 0 ;   
      RemoteXY.switch_1= 0 ;
      EEPROM.begin(512);
      EEPROM.put( 10,buttonState);
      EEPROM.commit();
      strcpy (RemoteXY.text_5 ,con_ssid  ) ; 
      RemoteXY.switch_1 = 0;
    //
    //  RemoteXY_sendInputVariables();
    }
  
  // For Soft Access Point (AP) Mode
  //wifi_set_macaddr(SOFTAP_IF, &newMACAddress[0]);
  // For Station Mode
  wifi_set_macaddr(STATION_IF, &newMACAddress[0]);
  Serial.print("[NEW] ESP8266 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
    
 // RemoteXY_Init ();  
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);  

  //------------Start autoConnect-------------- 
    //--- settaggi di Wifimanager-------
    WiFiManager wifiManager;
    // wifiManager.resetSettings();
    wifiManager.setTimeout(180); 
 
    if (!wifiManager.autoConnect("WiFi-ESP", "12345678")) // credenziali per accedere alla modalità AP
    {
      delay(3000);
      ESP.reset();
      delay(5000);
    }
    WiFi.SSID().toCharArray(con_ssid, 32);
    WiFi.psk().toCharArray(con_psk, 64);
    remotexy  = new CRemoteXY (RemoteXY_CONF_PROGMEM, &RemoteXY, REMOTEXY_ACCESS_PASSWORD,  con_ssid , con_psk, REMOTEXY_CLOUD_SERVER, REMOTEXY_CLOUD_PORT, REMOTEXY_CLOUD_TOKEN );
    //------------end autoConnect--------------

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  Serial.println("initializing ESP-NOW");

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv); 
  
  // TODO you setup code
  
}

//----------------------------------
void loop() { 
//---------------------------------- 
  arxi :
 // goto arxi ;
  
  RemoteXY_Handler ();
 // digitalWrite(PIN_SWITCH_1, (RemoteXY.switch_1==0)?HIGH:LOW);
    (buttonState= (RemoteXY.switch_1==1)?HIGH:LOW);
   //buttonState = digitalRead(buttonPin);
  if (buttonState){
      strcpy (RemoteXY.text_4 , "Ενεργος" ) ; 
      RemoteXY.led_1_r = 0 ;
      RemoteXY.led_1_g = 200 ;
      RemoteXY.switch_1= 1 ;
   //   RemoteXY_sendInputVariables ();
      buttonState = 1 ;
      EEPROM.begin(512);
      EEPROM.put( 10,buttonState);
      EEPROM.commit();
  } 
  if (!buttonState){
      strcpy (RemoteXY.text_4 , "Μη Ενεργος" ) ;    
      RemoteXY.led_1_r = 200 ;
      RemoteXY.led_1_g = 0 ;   
      strcpy (RemoteXY.text_5 ,con_ssid  ) ; 
      RemoteXY.switch_1= 0 ;
    //  RemoteXY_sendInputVariables ();
      buttonState = 0 ;
      EEPROM.begin(512);
      EEPROM.put( 10,buttonState);
      EEPROM.commit();
  } 
  
}

//----------------------------------
String macToStr(const uint8_t* mac){
//----------------------------------  
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  
  return result;
}

//---------------------------------
void open_Door() {
//---------------------------------    
    unsigned long currentMillisDoor = millis();
    int  timer = 0 ;
    if (ledStateDoor){
       timer = 15 ; //RemoteXY.edit_10 ;  ο χρονος που χτυπάει
    }
    else{
       timer = 30 ; //RemoteXY.edit_11 ; ο χρονος που σταματάει
    }
  if (currentMillisDoor - previousMillisDoor >= (intervalDoor * timer )  ) {
      previousMillisDoor = currentMillisDoor;
    if (ledStateDoor == LOW) {
      ledStateDoor = HIGH;
      digitalWrite(Relay_01,LOW);
      digitalWrite(Relay_02,LOW);
     
    } else {
      ledStateDoor = LOW;
      digitalWrite(Relay_01,HIGH);
      digitalWrite(Relay_02,HIGH);
     
    }
  }
}
