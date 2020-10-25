//  
 // Arduino digital clock  
 // Datum en tijd op LCD display  
 // a.d.h. van DS1307 RTC via I2C  
 //  
 // arduinows.blogspot.com  
   
 #include <Wire.h>  
 #include "RTClib.h"  
 #include <LiquidCrystal.h>  
   
 // Set up LCD display met 4 bits - R/W aan GND  
 LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // RS, E, D4-D7  
 //  
 RTC_DS1307 RTC;   
   
 void setup ()   
   {  
   lcd.begin(16,2);  
   Wire.begin();  
   RTC.begin();  
   }  
   
 void loop ()   
   {  
   DateTime now = RTC.now();  
   lcd.clear();  
   lcd.setCursor(0, 0);  
   lcd.print(now.day(), DEC);  
   lcd.print('/');   
   lcd.print(now.month(), DEC);  
   lcd.print('/');   
   lcd.print(now.year(), DEC);  
   lcd.setCursor(12,0);  
   lcd.print("tik");  
   lcd.setCursor(0, 1);  
   lcd.print(now.hour(), DEC);  
   lcd.print(':');  
   if (now.minute()<10) {   
   lcd.print('0'); }        // voeg "leading zero" toe als de minutenwaarde kleiner is dan 10  
   lcd.print(now.minute(), DEC);  
   lcd.print(':');   
   if (now.second()<10) {  
   lcd.print('0'); }        // voeg "leading zero" toe als de secondenwaarde kleiner is dan 10  
   lcd.print(now.second(), DEC);  
   if ((now.second() % 2) != 0)   // check of secondenwaarde oneven of even is.  
     {  
     lcd.setCursor(12,0);  
     lcd.print("tik");       // toon "tik" op oneven seconden  
     }  
   else   
     {  
     lcd.setCursor(12,0);  
     lcd.print("  ");  
     lcd.setCursor(12,1);  
     lcd.print("tak");       // toon "tak" op even seconden  
     }  
   delay(1000);  
 }