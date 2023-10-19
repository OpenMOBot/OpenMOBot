#define BLINK_INTERVAL 500

#include "OpenMOBot.h"

#include "FxTimer.h"

// Variables will change:
// StateStatusLED_g used to set the LED
int StateStatusLED_g = LOW;  

FxTimer *BlinkTimer_g;

void setup()
{
	// Setup the user LED pin.
	pinMode(PIN_USER_LED, OUTPUT);
	
	// Setup the blink timer.
	BlinkTimer_g = new FxTimer();
	BlinkTimer_g->setExpirationTime(BLINK_INTERVAL);
	BlinkTimer_g->updateLastTime();
}

void loop()
{
  BlinkTimer_g->update();
  if(BlinkTimer_g->expired())
  {
    BlinkTimer_g->updateLastTime();
    BlinkTimer_g->clear();

    // set the LED with the StateStatusLED_g of the variable:
    StateStatusLED_g = !StateStatusLED_g;
    digitalWrite(PIN_USER_LED, StateStatusLED_g);
  }
}

