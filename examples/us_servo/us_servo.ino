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

#define BLINK_INTERVAL 2000

#pragma endregion

#pragma region Headers

#if defined(__AVR_ATmega328P__)
#include <Servo.h>
#elif defined(__AVR_ATmega2560__)
#include <Servo.h>
#elif defined(ESP8266)
#pragma message("Unsupported ESP8266")
#elif defined(ESP32)
#include <ESP32Servo.h>
#endif

#include "OpenMOBot.h"

#pragma endregion

#pragma region Variables

/**
 * @brief Create servo object to control a servo.
 */
Servo UsServo_g;

/**
 * @brief Variable to store the servo position.
 */
int ServoPosition_g = 0;

/**
 * @brief Variable to store the servo position.
 */
int ServoDirection_g = 0;

/**
 * @brief StateStatusLED_g used to set the LED.
 */
int StateStatusLED_g = LOW;

/**
 * @brief Blink timer instance.
 */
FxTimer *BlinkTimer_g;

/**
 * @brief Send timer instance.
 */
FxTimer *ServoSweepTimer;

#pragma endregion

void setup()
{
  Serial.begin(DEFAULT_BAUD);

  // attaches the servo on pin 11 to the servo object
  UsServo_g.attach(PIN_US_SERVO);

  pinMode(PIN_USER_LED, OUTPUT);

  BlinkTimer_g = new FxTimer();
  BlinkTimer_g->setExpirationTime(BLINK_INTERVAL);
  BlinkTimer_g->updateLastTime();

  ServoSweepTimer = new FxTimer();
  ServoSweepTimer->setExpirationTime(30);
  ServoSweepTimer->updateLastTime();
}

void loop()
{
  BlinkTimer_g->update();
  if (BlinkTimer_g->expired())
  {
    BlinkTimer_g->updateLastTime();
    BlinkTimer_g->clear();

    // set the LED with the StateStatusLED_g of the variable:
    StateStatusLED_g = !StateStatusLED_g;
    digitalWrite(PIN_USER_LED, StateStatusLED_g);
  }

  ServoSweepTimer->update();
  if (ServoSweepTimer->expired())
  {
    ServoSweepTimer->updateLastTime();
    ServoSweepTimer->clear();

    if (ServoDirection_g == 0)
    {
      // Increment position with one degree.
      ServoPosition_g++;

      if (ServoPosition_g == 180)
      {
        // Turn the other direction.
        ServoDirection_g = 1;
      }
    }

    if (ServoDirection_g == 1)
    {
      // Increment position with one degree.
      ServoPosition_g--;

      if (ServoPosition_g == 0)
      {
        // Turn the other direction.
        ServoDirection_g = 0;
      }
    }

    // tell servo to go to position in variable 'pos'
    UsServo_g.write(ServoPosition_g);

    Serial.print("ServoPosition:");
    Serial.println(ServoPosition_g);
  }
}
