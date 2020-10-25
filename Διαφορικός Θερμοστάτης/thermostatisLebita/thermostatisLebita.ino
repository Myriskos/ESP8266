/*
   -- Διαφορικός_Θερμοστάτης --
   
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

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID "" //"dbSoft"
#define REMOTEXY_WIFI_PASSWORD "" //"xVf5nTEN"
#define REMOTEXY_CLOUD_SERVER "cloud.remotexy.com"
#define REMOTEXY_CLOUD_PORT 6376
#define REMOTEXY_CLOUD_TOKEN "078a2fb13ad44666194d3dbd124c4332"


// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,1,0,24,0,145,0,10,13,0,
  66,132,18,23,13,10,2,24,129,0,
  9,4,87,7,17,206,148,206,185,206,
  177,207,134,206,191,207,129,206,185,206,
  186,207,140,207,130,32,206,152,206,181,
  207,129,206,188,206,191,207,131,207,132,
  206,172,207,132,206,183,207,130,0,67,
  4,8,37,33,11,2,26,11,66,132,
  68,24,13,10,2,24,67,4,59,37,
  33,11,2,26,11,129,0,13,15,23,
  6,17,206,155,206,173,206,178,206,183,
  207,132,206,177,207,130,0,129,0,66,
  16,17,6,17,66,111,105,108,101,114,
  0,2,0,37,51,28,9,2,26,31,
  31,79,112,101,110,0,67,108,111,115,
  101,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t switch_1; // =1 if switch ON and =0 if OFF 

    // output variables
  int8_t level_1; // =0..100 level position 
  char text_1[11];  // string UTF8 end zero 
  int8_t level_2; // =0..100 level position 
  char text_2[11];  // string UTF8 end zero 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

//-------auto connect---------------------------
        #include <DNSServer.h>
        #include <ESP8266WebServer.h>
        #include <ESP8266WiFiMulti.h>
        #include <WiFiManager.h>
        ESP8266WiFiMulti WiFiMulti;
       
        char con_ssid[32]; //will hold the ssid of the wlan to which it is connectd
        char con_psk[64];  // ^^ private shared key for the wlan (password)
 //------- end auto connect-------------------------

// βιβλιοθηκη για αισθητηρας θερμοκρασιας DS18B20
#include <DallasTemperature.h>
#include <OneWire.h>
//αισθητηρας θερμοκρασιας DS18B20
 #define ONE_WIRE_BUS D3 //δηλωση pin 
 OneWire oneWire(ONE_WIRE_BUS);
 DallasTemperature sensors(&oneWire); //Σχετίζουμε την Onewire στον Dallas Temperature

#include <Arduino.h>
#include <U8x8lib.h>
//#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

// Please UNCOMMENT one of the contructor lines below
// U8x8 Contructor List 
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // OLEDs without Reset of the Display
//το flag ειναι για να αναβοσβηνη το LED 
int flag=0 ;
float therm_1 ;
float therm_2 ;
//ορισμος εξοδων ρελε
const int Relay_01 = D4 ;
bool Relay_on = false ;
bool buttonState = true ;


void setup() 
{

 //RemoteXY_Init (); 
  
// Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

//------------Start autoConnect-------------- 
    //--- settaggi di Wifimanager-------
    WiFiManager wifiManager;
    // wifiManager.resetSettings();
    wifiManager.setTimeout(180); 
 
    if (!wifiManager.autoConnect("WiFi-ESP_A", "12345678")) // credenziali per accedere alla modalità AP
    {
      delay(3000);
      ESP.reset();
      delay(5000);
    }
    WiFi.SSID().toCharArray(con_ssid, 32);
    WiFi.psk().toCharArray(con_psk, 64);
    
    remotexy = new CRemoteXY (RemoteXY_CONF_PROGMEM, &RemoteXY, REMOTEXY_ACCESS_PASSWORD,  con_ssid , con_psk, REMOTEXY_CLOUD_SERVER, REMOTEXY_CLOUD_PORT, REMOTEXY_CLOUD_TOKEN );
     //------------end autoConnect--------------

 
 pinMode( Relay_01 ,OUTPUT );  
 digitalWrite(Relay_01,HIGH); 
  
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.clear();
  
}

void loop() { 
  arxh:
  RemoteXY_Handler ();
  buttonState = ((RemoteXY.switch_1==0)?HIGH:LOW);
    
  sensors.requestTemperatures ();
  therm_1=(sensors.getTempCByIndex(0));  
  therm_2=(sensors.getTempCByIndex(1));
   delay(100);
  if (therm_1 < 0 || therm_2 < 0) {
     goto arxh ;
  }
   
  dtostrf(therm_1, 0, 1, RemoteXY.text_1);
  dtostrf(therm_2, 0, 1, RemoteXY.text_2);
 
  RemoteXY.level_1 = therm_1 ;
  RemoteXY.level_2 = therm_2 ;

  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(01,0,"www.dbSoft.gr");
  u8x8.drawString(01,55,"info@dbSoft.gr");
  u8x8.setFont(u8x8_font_px437wyse700b_2x2_r);
  
   //u8x8.setFont(u8x8_font_pxplustandynewtv_r);
   char text_01[11];
   dtostrf(therm_1, 1, 0, text_01);
   u8x8.drawString(01, 11, text_01);
   char text_02[11];
   dtostrf(therm_2, 1, 0, text_02);
   u8x8.drawString(10, 11, text_02);

   if (!buttonState) {
      Relay_on = true ;
      digitalWrite(Relay_01,LOW);
      goto arxh ;
   }
   
/*
        char oled_msg[20];
        String tString=String(therm_1);
        String hString=String(therm_2);
        u8x8.clearLine(4);
        sprintf(oled_msg, "%s %s", tString.c_str(), hString.c_str()); 
        u8x8.drawString(0, 4, oled_msg); 
*/
if (therm_1 > 0 && therm_2 > 0) {   
  if (buttonState) {
   if (therm_1 >  therm_2 ) {
     Relay_on = true ;
     digitalWrite(Relay_01,LOW);
   }
   else if  (therm_1 <  therm_2 )  {
       Relay_on = false ;
       digitalWrite(Relay_01,HIGH);
   }  
  }
}

  delay(1000);
}



//----------------------------------------------------------
void ledAlarm(  int pinled  ) {
    if ( flag == 0){
      digitalWrite( pinled , HIGH);
      flag=1; 
    }
    else if ( flag == 1){
      digitalWrite( pinled , LOW);
      flag=0;  
    }    
return ;
}
