#include <Arduino.h>
#include <U8g2lib.h>
#include <clib\u8g2.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup()
{
    // put your setup code here, to run once:
    u8g2.begin();
    u8g2.setFontPosTop();
}

void loop()
{
   u8g2.clearBuffer();
  //u8g2_font_10x20_t_greek
  u8g2.setFont(u8g2_font_unifont_t_greek);
  u8g2.setCursor( 0,0);
  u8g2.print("MYRISKOS");
  delay(5000);


    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_fub35_tn); // 791 bytes
    u8g2.setCursor( 0, 0);
    u8g2.print("23.45");
    u8g2.setFont(u8g2_font_fub20_tr); // 2520 bytes
    u8g2.setCursor( 0, 40);
    u8g2.print("fub35_tn");
    u8g2.sendBuffer();
    delay(5000);
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_fub30_tn); // 669 bytes
    u8g2.setCursor( 0, 0);
    u8g2.print("-34.56");
    u8g2.setFont(u8g2_font_fub20_tr); // 2520 bytes
    u8g2.setCursor( 0, 40);
    u8g2.print("fub30_tn");
    u8g2.sendBuffer();
    delay(5000);
}
