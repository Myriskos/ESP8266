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

// βιβλιοθηκη για αισθητηρας θερμοκρασιας DS18B20
#include <DallasTemperature.h>
#include <OneWire.h>
//αισθητηρας θερμοκρασιας DS18B20
 #define ONE_WIRE_BUS 2 //δηλωση pin δεδομενων στο 10
 OneWire oneWire(ONE_WIRE_BUS);
 DallasTemperature sensors(&oneWire); //Σχετίζουμε την Onewire στον Dallas Temperature


// REPLACE WITH RECEIVER MAC Address
// {0xA4, 0xCF, 0x12, 0xDF, 0x58, 0x46} ;  // SEND A
// {0xA4, 0xCF, 0x12, 0xDF, 0x51, 0x1}  ;  // SEND B
// {0x98, 0xF4, 0xAB, 0xB3, 0x8F, 0x94} ;  // SEND C
// {0xA4, 0xCF, 0x32, 0x7A, 0x5, 0x18}  ;  // SEND D
// {0xA4, 0xCF, 0x12, 0xDF, 0x48, 0xFA} ;  // SEND E
// {0xA4, 0xCF, 0x12, 0xDF, 0x48, 0xAA} ;  // RECEIVER

// a4:cf:12:df:58:46    SEND A
// a4:cf:12:df:51:1     SEND B
// 98:f4:ab:b3:8f:94    SEND C
// 2c:f4:32:7a:5:18     SEND D
// a4:cf:12:df:48:fa    SEND E
// A4:CF:12:DF:48:AA    RECEIVER

uint8_t broadcastAddress[] = {0xA4, 0xCF, 0x12, 0xDF, 0x48, 0xAA}; // RECEIVER
float therm_1 ;
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
    sensors.requestTemperatures ();
    therm_1=(sensors.getTempCByIndex(0));  


    
  }
  else{
    Serial.println("Delivery fail");
  }
}
 
void setup() {

  sensors.begin(); /* Εκκίνηση της βιβλιοθήκης για DS18B20 */
  // Init Serial Monitor
  Serial.begin(115200);
 
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
  // Set values to send
  strcpy(myData.a, "THIS IS a CHAR from D");
  myData.b = random(1,20);
  myData.c = therm_1;
  myData.d = "send D";
  myData.e = false;

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
