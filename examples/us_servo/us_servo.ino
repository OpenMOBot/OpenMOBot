#define BLINK_INTERVAL 5000

#include <Servo.h>

#include "OpenMOBot.h"
#include "FxTimer.h"

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

void setup()
{
  // attaches the servo on pin 11 to the servo object
  UsServo_g.attach(PIN_US_SERVO);

  pinMode(PIN_USER_LED, OUTPUT);

  BlinkTimer_g = new FxTimer();
  BlinkTimer_g->setExpirationTime(BLINK_INTERVAL);
  BlinkTimer_g->updateLastTime();

  ServoSweepTimer = new FxTimer();
  ServoSweepTimer->setExpirationTime(15);
  ServoSweepTimer->updateLastTime();
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

  ServoSweepTimer->update();
  if(ServoSweepTimer->expired())
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
  }
}
