#include <Arduino.h>

// DAS PINS
#define ARM_BUTTON 3
#define ARM_LED 5
#define DISARM_BUTTON_ONE 8

// STATES
#define DISARMED 0
#define ARMING 1
#define ARMED 2
#define DISARMING 3
#define DETONATED 4

#define TWO_MINS_IN_MILLIS 120000
#define THIRTY_SECONDS_IN_MILLIS 30000

unsigned long arm_target = 0;
unsigned long arm_time = 0;
unsigned long disarm_target = 0;
unsigned long disarm_time = 0;
int current_state = DISARMED;

void setup() {
}


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


/*

*/

void handle_arm_button_off(){
  if (current_state == ARMING)
  {
    current_state = ARMED;
    digitalWrite(ARM_LED, HIGH);
  }
}

void loop() {
  switch(current_state) {
    case DISARMED:
      /*
      Handle the disarmed state. The only state that this can transition to is
      ARMED, by flicking the arm switch, the transition of which is handled by
      interrupts
      */
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
