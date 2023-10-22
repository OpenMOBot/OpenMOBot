/*

MIT License

Copyright (c) [2019] [Orlin Dimitrov]

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

#define BLINK_INTERVAL 5000

#pragma endregion

#pragma region Headers

#include "OpenMOBot.h"
#include "MotorController.h"
#include "FxTimer.h"

#ifdef DEBUG_TEXT
#include "DebugPort.h"
#endif

#pragma endregion

#pragma region Functions Prototypes

/** @brief Interrupt Service Routine for handleng left encoder.
 *  @return Void.
 */
void ISR_Left_Encoder();

/** @brief Interrupt Service Routine for handleng right encoder.
 *  @return Void.
 */
void ISR_Right_Encoder();

/** @brief Read motor speed.
 *  @return Motor speed.
 */
double read_motor_speed();

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

#pragma endregion

void setup()
{
  pinMode(PIN_USER_LED, OUTPUT);

	// Attach the Interrupts to their ISR's
	pinMode(PIN_LEFT_ENCODER, INPUT_PULLUP);
  pinMode(PIN_RIGHT_ENCODER, INPUT_PULLUP);
  
  // Increase counter 1 when speed sensor pin goes High.
	attachInterrupt(digitalPinToInterrupt(PIN_LEFT_ENCODER), ISR_Left_Encoder, RISING);
	// Increase counter 2 when speed sensor pin goes High.
	attachInterrupt(digitalPinToInterrupt(PIN_RIGHT_ENCODER), ISR_Right_Encoder, RISING);

  Serial.begin(9600);
  Serial.println("Leftencoder,RightEncoder");

  BlinkTimer_g = new FxTimer();
  BlinkTimer_g->setExpirationTime(BLINK_INTERVAL);
  BlinkTimer_g->updateLastTime();

  SendTimer_g = new FxTimer();
  SendTimer_g->setExpirationTime(100);
  SendTimer_g->updateLastTime();

	// Setup the motor driver.
	MotorModel_t model = {
		PIN_L_F,
		PIN_L_B,
    PIN_L_PWM,
		PIN_R_F,
		PIN_R_B,
    PIN_R_PWM,
		WHEEL_DIAMETER,
		DISTANCE_BETWEEN_WHEELS,
		ENCODER_TRACKS
	};

	// Initialize the motor controller.
	MotorController.init(&model);
}

void loop()
{
  MotorController.update();

  BlinkTimer_g->update();
  if(BlinkTimer_g->expired())
  {
    BlinkTimer_g->updateLastTime();
    BlinkTimer_g->clear();

    // if the LED is off turn it on and vice-versa:
    if (StateStatusLED_g == LOW)
    {
    	MotorController.SetPWM(-120, 120);
    }
    else
    {
    	MotorController.SetPWM(120, -120);
    }

    // set the LED with the StateStatusLED_g of the variable:
    StateStatusLED_g = !StateStatusLED_g;
    digitalWrite(PIN_USER_LED, StateStatusLED_g);
  }

  SendTimer_g->update();
  if(SendTimer_g->expired())
  {
    SendTimer_g->updateLastTime();
    SendTimer_g->clear();

    
    Serial.print(MotorController.GetLeftMotorRPM());
    Serial.print(",");
    Serial.println(MotorController.GetRightMotorRPM());
  }
}

/** @brief Interrupt Service Routine for handleng left encoder.
 *  @return Void.
 */
void ISR_Left_Encoder()
{
	MotorController.UpdateLeftEncoder();
}

/** @brief Interrupt Service Routine for handleng right encoder.
 *  @return Void.
 */
void ISR_Right_Encoder()
{
	MotorController.UpdateRightEncoder();
}
