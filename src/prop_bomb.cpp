#include <Arduino.h>
#include <SPI.h>

#include <Adafruit_ST7735.h>
#include <Adafruit_GFX.h>
#include <gfxfont.h>

// DAS PINS
#define ARM_BUTTON 3;
#define ARM_LED 5;
#define DISARM_BUTTON_ONE 8;
#define DISARM_BUTTON_TWO 9;

// TFT setup
#define TFT_CS     21
#define TFT_RST    0
#define TFT_DC     20
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

// STATES
#define DISARMED = 0
#define ARMING 1
#define ARMED 2
#define DISARMING 2




void setup() {
  tft.initR(INITR_BLACKTAB);

  tft.setCursor(0, 0);
  tft.setTextColor(ST7735_BLACK);
  tft.setTextWrap(true);
  tft.print("hello world");
}

void loop() {
  // put your main code here, to run repeatedly:

}
