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

/**
 * @brief Time interval for update blink cycle.
 *
 */
#define BLINK_INTERVAL_MS 5000

#pragma endregion

#pragma region Headers

#include "OpenMOBot.h"
#include "FxTimer.h"
#include "HCSR04.h"

#ifdef DEBUG_TEXT
#include "DebugPort.h"
#endif

#pragma endregion

#pragma region Constants

#pragma endregion

#pragma region Variables

/**
 * @brief Blink led last state flag.
 */
int StateStatusLED_g = LOW;

/**
 * @brief Blink timer instance.
 */
FxTimer *BlinkTimer_g;

/**
 * @brief Send timer instance.
 */
FxTimer *SendTimer_g;

/* @brief Ultrasonic sensor. */
HCSR04 HCSR04_g;

/* @brief Ultra sonic sensor distance value. */
float USDistance_g = 0;

#pragma endregion

#pragma region Prototypes Functions

#pragma endregion

/**
 * @brief The setup function runs once when you press reset or power the board.
 *
 */
void setup()
{
	// Set user interaction.
	pinMode(PIN_USER_LED, OUTPUT);
	digitalWrite(PIN_USER_LED, LOW);

	// Initialize the ultrasonic.
	HCSR04_g.init(PIN_US_TRIG, PIN_US_ECHO);

  Serial.begin(DEFAULT_BAUD);

  BlinkTimer_g = new FxTimer();
  BlinkTimer_g->setExpirationTime(BLINK_INTERVAL_MS);
  BlinkTimer_g->updateLastTime();

  SendTimer_g = new FxTimer();
  SendTimer_g->setExpirationTime(100);
  SendTimer_g->updateLastTime();

}

/**
 * @brief Main loop.
 *
 */
void loop()
{
  static long microsec = 0;

  BlinkTimer_g->update();
  if (BlinkTimer_g->expired())
  {
    BlinkTimer_g->updateLastTime();
    BlinkTimer_g->clear();

    // // if the LED is off turn it on and vice-versa:
    // if (StateStatusLED_g == LOW)
    // {
    //   MotorController.SetPWM(-150, 150);
    // }
    // else
    // {
    //   MotorController.SetPWM(150, -150);
    // }

    // set the LED with the StateStatusLED_g of the variable:
    StateStatusLED_g = !StateStatusLED_g;
    digitalWrite(PIN_USER_LED, StateStatusLED_g);
  }

  SendTimer_g->update();
  if (SendTimer_g->expired())
  {
    SendTimer_g->updateLastTime();
    SendTimer_g->clear();

    microsec = HCSR04_g.timing();
    USDistance_g = HCSR04_g.convert(microsec, HCSR04::CM);
    Serial.print("millis:");
    Serial.print(millis());
    Serial.print(",");
    Serial.print("USDistance_g:");
    Serial.println(USDistance_g);
  }
}

#pragma region Functions

#pragma endregion
