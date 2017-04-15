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
#define DISARMED 0
#define ARMING 1
#define ARMED 2
#define DISARMING 3
#define DETONATED 4

#define TWO_MINS_IN_MILLIS 120000
#define THIRTY_SECONDS_IN_MILLIS 30000

int armRemaining = 0;
int disarmRemaining = 0;
int current_state = DISARMED;

void setup() {
  tft.initR(INITR_BLACKTAB);

  tft.setCursor(0, 0);
  tft.setTextColor(ST7735_BLACK);
  tft.setTextWrap(true);
  tft.print("hello world");
}

void loop() {
  switch(current_state) {
    case DISARMED:
      break;
    case ARMING:
      break;
    case ARMED:
      break;
    case DISARMING:
      break;
    case DETONATED:
      break;
  }
}
