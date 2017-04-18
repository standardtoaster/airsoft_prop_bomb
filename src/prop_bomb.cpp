#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_NeoPixel.h"

// DAS PINS
#define ARM_BUTTON 3
#define ARM_LED 5
#define DISARM_BUTTON_ONE 8
#define BIOHAZARD_PIN 0 // TODO: FIXME

// STATES
#define DISARMED 0
#define ARMING 1
#define ARMED 2
#define DISARMING 3
#define DETONATED 4

#define TWO_MINS_IN_MILLIS 120000
#define THIRTY_SECONDS_IN_MILLIS 30000
#define DISARM_GRACE_IN_MILLIS 250

// How fast the biohazard LED strip blinks. Basically if millis() % VAL is 0,
// this will change state. 1000 would be once a second, 250 would be 4 a second.
#define BIOHAZARD_BLINK_INTERVAL 250

// The number of pixels in the biohazard strip.
#define BIOHAZARD_PIXEL_COUNT 5

#define BIOHAZARD_GREEN

unsigned long arm_target = 0;
unsigned long disarm_target = 0;
unsigned long disarm_time = 0;
unsigned long last_disarm_button_up_millis = 0;
int current_state = DISARMED;

Adafruit_7segment arm_timer = Adafruit_7segment();
Adafruit_7segment disarm_timer = Adafruit_7segment();

Adafruit_NeoPixel biohazard_strip = Adafruit_NeoPixel(
  BIOHAZARD_PIXEL_COUNT,
  BIOHAZARD_PIN,
  NEO_RGB
);

uint32_t biohazard_off = biohazard_strip.Color(0, 0, 0);
uint32_t biohazard_red = biohazard_strip.Color(255, 0, 0);
uint32_t biohazard_green = biohazard_strip.Color(0, 255, 0);
uint32_t biohazard_blue = biohazard_strip.Color(0, 0, 255);

void handle_arm_button_on(){
  /*
    The arm button has been flicked on. Handle state transition.
    This is probably bad to do in an ISR.
  */
  if (current_state == DISARMED) {
    // change state to arming and turn on the LED
    current_state = ARMING;
  }
}

void handle_arm_button_off(){
  if (current_state == ARMING)
  {
    current_state = ARMED;
    digitalWrite(ARM_LED, HIGH);
    arm_target = millis() + TWO_MINS_IN_MILLIS;
  }
}

void handle_disarm_button_on() {
  if (current_state != DISARMING) {
    disarm_target = millis() + THIRTY_SECONDS_IN_MILLIS;
  }
  current_state = DISARMING;
}

void handle_disarm_button_off() {
  // give 250ms before we flip the state back to armed.
  if (last_disarm_button_up_millis + DISARM_GRACE_IN_MILLIS > millis()) {
    current_state = ARMED;
  }
}


unsigned int gen_countdown_time(unsigned long time) {
  // Generate the countdown time as a 4 digit integer for display on a 4 unit 7
  // segment display, which expects an int.

  unsigned int retval = 0;

  // Thanks to @mjonuschat for this nugget. Move the minutes over using simple
  // multiplication.

  // Minutes
  retval += time / 60 * 100;
  // Seconds
  retval += time % 60;

  return retval;
}

void blank_display(Adafruit_7segment matrix) {
  matrix.writeDigitRaw(0, 0);
  matrix.writeDigitRaw(1, 0);
  matrix.drawColon(false);
  matrix.writeDigitRaw(3, 0);
  matrix.writeDigitRaw(4, 0);
  matrix.writeDisplay();
}

void render_arm_countdown() {
  if (current_state == ARMED || current_state == DISARMING){
    arm_timer.drawColon(true);
    arm_timer.print(gen_countdown_time(arm_target - millis()));
    arm_timer.writeDisplay();
  } else {
    blank_display(arm_timer);
  }
}

void render_disarm_countdown() {
  if (current_state == DISARMING){
    disarm_timer.drawColon(true);
    disarm_timer.print(gen_countdown_time(disarm_target - millis()));
    disarm_timer.writeDisplay();
  } else {
    blank_display(disarm_timer);
  }
}

void render_biohazard_pixel(int pixel_id) {
  switch(current_state) {
    case ARMED:
      // Solid green.
    case DISARMING:
      // Solid green.
      biohazard_strip.setPixelColor(pixel_id, biohazard_green);
      // TODO: Code to make thing green.
      break;
    case DETONATED:
      // Flashing Red.
      if (millis() % BIOHAZARD_BLINK_INTERVAL == 0) {
        biohazard_strip.setPixelColor(pixel_id, biohazard_red);
      } else if (millis() % (BIOHAZARD_BLINK_INTERVAL * 2) == 0) {
        biohazard_strip.setPixelColor(pixel_id, biohazard_off);
      }
      break;
    default:
      //all LEDs off.
      biohazard_strip.setPixelColor(pixel_id, biohazard_off);
      break;
  }
}

void render_biohazard(){
  int i = 0;
  for( i = 0; i < BIOHAZARD_PIXEL_COUNT ; i += 1 ){
    render_biohazard_pixel(i);
  }
  biohazard_strip.show();
}

void setup() {
  arm_timer.begin(0x70);
  disarm_timer.begin(0x71);
  blank_display(arm_timer);
  blank_display(disarm_timer);
}

void loop() {
  switch(current_state) {
    case DISARMED:
      /*
      Handle the DISARMED state. The only state that this can transition to is
      ARMIGN, by flicking the arm switch, the transition of which is handled by
      interrupts.
      */
      //TODO: Lights should be off, displays should be off, reset counters
      break;
    case ARMING:
      /*
      Handle the ARMING state. The only state that this can transition to is
      ARMED, by flicking the arm switch, the transition of which is handled by
      interrupts.
      */
      break;
    case ARMED:
      // If the arm_target is now or later than now BOOM!
      if (arm_target >= millis()) {
        current_state = DETONATED;
      }
      break;
    case DISARMING:
      if (disarm_target >= millis())
      {
        current_state = DISARMED;
      }
      break;
    case DETONATED:
      //The only way to turn this off is to cycle the power
      break;
  }
  render_arm_countdown();
  render_disarm_countdown();
  render_biohazard();
}
