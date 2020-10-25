#include <Arduino.h>
#include "SSD1306Wire.h"


SSD1306Wire  display(0x3c, D4, D5);
//SSD1306Wire  display(0x7A, D3, D5);
     


void setup() {

  Serial.begin(115200);

  display.init();
  display.setContrast(255);
  
  display.clear();

  display.drawString(0, 0, "ESP32/OLED");
  display.display();

 
}

//for(i=0; i<100; i++){
//    analogWrite(ledB, i);
//    delay(10);
//}


void loop() {
  int i;
  display.clear();
  for(i=0; i<60; i++) {
    display.drawString(i, 10, "CONNECTED");
    display.display();
  } 
   // display.display();
  
  Serial.println("DONE");
  delay(1000);

}  
