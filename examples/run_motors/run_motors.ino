#define BLINK_INTERVAL 2000

#include "OpenMOBot.h"
#include "MotorController.h"
#include "FxTimer.h"

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

/** @brief Interrupt Service Routine for handleng left encoder.
 *  @return Void.
 */
void ISR_Left_Encoder();

/** @brief Interrupt Service Routine for handleng right encoder.
 *  @return Void.
 */
void ISR_Right_Encoder();

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

  Serial.begin(115200);
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

    Serial.print(MotorController.GetLeftEncoder());
    Serial.print(",");
    Serial.println(MotorController.GetRightEncoder());
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
