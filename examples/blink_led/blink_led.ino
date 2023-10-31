/*

MIT License

Copyright (c) [2023] [OpenMOBot]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#pragma region Definitions

// #define DEBUG_TEXT

#define DEBUG_OSC

#define BLINK_INTERVAL 500

#pragma endregion

#pragma region Headers

#include "OpenMOBot.h"
#include "FxTimer.h"

#ifdef DEBUG_TEXT
#include "DebugPort.h"
#endif

#pragma endregion

#pragma region Variables

/**
  * @brief StateStatusLED_g used to set the LED.
  */
int StateStatusLED_g = LOW;

/** 
 * @brief Blink timer instance.
 */
FxTimer *BlinkTimer_g;

#pragma endregion

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
