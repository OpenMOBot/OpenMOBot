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

#define BLINK_INTERVAL_MS 5000

#pragma endregion

#pragma region Headers

#include <PID_v1.h>

#include "OpenMOBot.h"

#ifdef DEBUG_TEXT
#include "DebugPort.h"
#endif

#pragma endregion

#pragma region Functions Prototypes

/** @brief Interrupt Service Routine for handling left encoder.
 *  @return Void.
 */
void ISR_Left_Encoder();

/** @brief Interrupt Service Routine for handling right encoder.
 *  @return Void.
 */
void ISR_Right_Encoder();

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

/**
 * @brief Define Variables we'll be connecting to
 */
double Setpoint, Input, Output;

/**
 * @brief Define the aggressive and conservative Tuning Parameters.
 */
double consKp = 2, consKi = 0.025, consKd = 0.00;

/**
 * @brief Specify the links and initial tuning parameters.
 */
PID *myPID;

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

  Serial.begin(DEFAULT_BAUD);

  BlinkTimer_g = new FxTimer();
  BlinkTimer_g->setExpirationTime(BLINK_INTERVAL);
  BlinkTimer_g->updateLastTime();

  SendTimer_g = new FxTimer();
  SendTimer_g->setExpirationTime(100);
  SendTimer_g->updateLastTime();

  // Setup the motor driver.
  MotorModel_t MotorModelL = {
      PIN_L_F,
      PIN_L_B,
      PIN_L_PWM,
      PIN_R_F,
      PIN_R_B,
      PIN_R_PWM,
      WHEEL_DIAMETER,
      DISTANCE_BETWEEN_WHEELS,
      ENCODER_TRACKS};

  // Initialize the motor controller.
  MotorController.init(&ModelL);

  // Set the PID regulator.
  myPID = new PID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);
  myPID->SetMode(AUTOMATIC);
  myPID->SetSampleTime(50);
  // myPID->SetTunings(DeviceConfiguration.KP, DeviceConfiguration.KI, DeviceConfiguration.KD);
  myPID->SetOutputLimits(-PWM_MAX, PWM_MAX);
}

void loop()
{
  myPID->Compute();

  MotorController.update();

  BlinkTimer_g->update();
  if (BlinkTimer_g->expired())
  {
    BlinkTimer_g->updateLastTime();
    BlinkTimer_g->clear();

    // set the LED with the StateStatusLED_g of the variable:
    StateStatusLED_g = !StateStatusLED_g;
    digitalWrite(PIN_USER_LED, StateStatusLED_g);
  }

  SendTimer_g->update();
  if (SendTimer_g->expired())
  {
    SendTimer_g->updateLastTime();
    SendTimer_g->clear();

    // Set the speed.
    Setpoint = 100;

    // Set the input feedback.
    Input = MotorController.GetLeftMotorRPM() / 5.0;

    // Set the output from the regulator.
    MotorController.SetPWM(Output, 0);

    Serial.print("Setpoint:");
    Serial.print(Setpoint);
    Serial.print(", ");
    Serial.print("Input:");
    Serial.print(Input);
    Serial.print(", ");
    Serial.print("Output:");
    Serial.println(Output);
  }
}

#pragma region Functions

/** @brief Interrupt Service Routine for handling left encoder.
 *  @return Void.
 */
void ISR_Left_Encoder()
{
  MotorController.UpdateLeftEncoder();
}

/** @brief Interrupt Service Routine for handling right encoder.
 *  @return Void.
 */
void ISR_Right_Encoder()
{
  MotorController.UpdateRightEncoder();
}

#pragma endregion
