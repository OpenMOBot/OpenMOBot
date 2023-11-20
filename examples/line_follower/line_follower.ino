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

/** @brief Safety distance in CM.. */
#define SAFETY_DISTANCE 20.0

/** @brief Dead zone of the joystick. */
#define DEAD_ZONE 10

/** @brief Throttle input. */
#define PIN_THROTTLE A3

#pragma endregion

#pragma region Headers

#include "AppplicationState.h"

#include "OpenMOBot.h"
#include "DebugPort.h"
#include "LineSensor.h"
// #include "HCSR04.h"
#include "LRData.h"
#include "XYData.h"
#include "MotorController.h"
#include "utils.h"

// #include <Servo.h>

#pragma endregion

#pragma region Constants

/**
  * @brief Line sensor pins definitions.
  */
const uint8_t PinsLineSensor_g[LINE_SENSORS_COUNT] = 
{
  PIN_LS_1,
  PIN_LS_2,
  PIN_LS_3,
  PIN_LS_4,
  PIN_LS_5,
  PIN_LS_6
};

#pragma endregion

#pragma region Variables

/* @brief Application state flag. */
uint8_t AppStateFlag_g = ApplicationState::WaitForCalibration;

/* @brief Ultrasonic sensor. */
// HCSR04 HCSR04_g;

/* @brief Ultrasonic servo axis. */
// Servo USServo_g;

/* @brief User button state. */
int UserButtonState_g;

/* @brief Ultra sonic sensor distance value. */
float USDistance_g = 200;

/* @brief Line position value. */
float LinePosition_g = 0;

/* @brief Throttle value. */
int Throttle_g = 512;

/* @brief Safety flag. */
bool SafetyFlag_g = false;

/* @brief XY data value. */
XYData_t XYData_g;

/* @brief LR data value. */
LRData_t LRData_g;

#pragma endregion

#pragma region Prototypes Functions

/** @brief Read analog line sensor callback function.
 * 
 *  @param index int, Sensor index it exists in [0 to Sensor count -1].
 *  @return uint16_t Read sensor data.
 */
uint16_t readSensor(int index);

/** @brief Interrupt Service Routine for handling left encoder.
 *  @return Void.
 */
void ISR_Left_Encoder();

/** @brief Interrupt Service Routine for handling right encoder.
 *  @return Void.
 */
void ISR_Right_Encoder();

#pragma endregion

/**
 * @brief The setup function runs once when you press reset or power the board.
 * 
 */
void setup()
{
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
	// Attach the Interrupts to their ISR's
	// Increase counter 1 when speed sensor pin goes High.
	attachInterrupt(digitalPinToInterrupt(PIN_LEFT_ENCODER), ISR_Left_Encoder, RISING);
	// Increase counter 2 when speed sensor pin goes High.
	attachInterrupt(digitalPinToInterrupt(PIN_RIGHT_ENCODER), ISR_Right_Encoder, RISING);
	// play the test animation.
	//play_animation();

	// Initialize the line sensor.
	LineSensor.init(LINE_SENSORS_COUNT);
	LineSensor.setCbReadSensor(readSensor);
	LineSensor.setInvertedReadings(false);

	// Initialize the ultrasonic servo.
	// USServo_g.attach(PIN_US_SERVO);
	// USServo_g.write(90);

	// Initialize the ultrasonic.
	// HCSR04_g.init(PIN_US_TRIG, PIN_US_ECHO);

	// Set user interaction.
	pinMode(PIN_USER_LED, OUTPUT);
	digitalWrite(PIN_USER_LED, LOW);
}

/**
 * @brief Main loop.
 * 
 */
void loop()
{
	static int CalibrationL = 0;

	LineSensor.update();

	// long microsec = HCSR04_g.timing();
	// USDistance_g = HCSR04_g.convert(microsec, HCSR04::CM);

	if (AppStateFlag_g == ApplicationState::WaitForCalibration)
	{
		if (UserButtonState_g == LOW)
		{
			AppStateFlag_g = ApplicationState::CalibrateSensors;
		}
	}
	else if (AppStateFlag_g == ApplicationState::CalibrateSensors)
	{
		if (CalibrationL < LINE_SENSORS_CALIBRATION_SIZE)
		{
			LineSensor.calibrate();
			CalibrationL++;
		}
		else
		{
			CalibrationL = 0;
			AppStateFlag_g = ApplicationState::WaitForStart;
		}
	}
	else if (AppStateFlag_g == ApplicationState::WaitForStart)
	{
		if (UserButtonState_g == LOW)
		{
			AppStateFlag_g = ApplicationState::Run;
		}
	}
	else if (AppStateFlag_g == ApplicationState::Run)
	{
		if (UserButtonState_g == LOW)
		{
			AppStateFlag_g = ApplicationState::SafetyStop;
			return;
		}

		if (USDistance_g < SAFETY_DISTANCE)
		{
			AppStateFlag_g = ApplicationState::SafetyStop;
			return;
		}

		// Get Road conditions.
		LinePosition_g = LineSensor.getLinePosition();

		if (LinePosition_g > 1000)
		{
			AppStateFlag_g = ApplicationState::SafetyStop;
			return;
		}

		// Throttle_g = analogRead(PIN_THROTTLE);
		if (Throttle_g < 512 + 20 && Throttle_g > 512 - 20)
		{
			Throttle_g = 512;
		}
		XYData_g.X = map(LinePosition_g, 700, 0, 0, 1023);
		XYData_g.Y = Throttle_g;

		// Convert X and  data to Left and Right PWM data.
		LRData_g = xy_to_lr(XYData_g);

		// Control the robot.
		MotorController.SetPWM(LRData_g.L, LRData_g.R);
	}
	else if (AppStateFlag_g == ApplicationState::SafetyStop)
	{
		if (UserButtonState_g == LOW)
		{
			SafetyFlag_g = false;
			AppStateFlag_g = ApplicationState::WaitForStart;
		}

		if (SafetyFlag_g == false)
		{
			SafetyFlag_g = true;
			MotorController.SetPWM(0, 0);
		}
	}
}

#pragma region Functions

/** @brief Read analog line sensor callback function.
 * 
 *  @param index int, Sensor index it exists in [0 to Sensor count -1].
 *  @return uint16_t Read sensor data.
 */
uint16_t readSensor(int index)
{
	return analogRead(PinsLineSensor_g[index]);
}

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
