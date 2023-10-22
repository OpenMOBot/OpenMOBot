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

// MotorController.h

#ifndef _MOTORCONTROLLER_h
#define _MOTORCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include "FxTimer.h"
#include "LowPassFilter.h"
// #include "DebugPort.h"

typedef struct
{
	uint8_t PinLeftForward; ///< Left speed pin.
	uint8_t PinLeftBackward; ///< Left direction pin.
	uint8_t PinLeftPWM; ///< Left PWM pin.
	uint8_t PinRightForward; ///< Right speed pin.
	uint8_t PinRightBackward; ///< Right direction pin.
	uint8_t PinRightPWM; ///< Right PWM pin.
	double WheelDiameter; ///< Wheels diameter.
	double DistanceBetweenWheels; ///< Distance between wheels.
	uint32_t EncoderTracs; ///< Number of encoders track.
/** @brief H-bridge motor Controller. */
} MotorModel_t;

/** @brief H-bridge motor hontroller. */
class MotorControllerClass
{

 protected:

#pragma region Variables

	 /** @brief Motor settings. */
	 MotorModel_t m_motorModel;

	 /** @brief Left encoder counter. */
	 volatile long m_cntLeft;

	 /** @brief Right encoder counter. */
	 volatile long m_cntRight;

	 /** @brief Left encoder counter enable flag. */
	 volatile int8_t m_dirCntLeft;

	 /** @brief Right encoder counter enable flag. */
	 volatile int8_t m_dirCntRight;

	/**
	 * @brief Left motor PWM value.
	 * 
	 */
	int16_t m_leftPWM;

	/**
	 * @brief Right motor PWM value.
	 * 
	 */
	int16_t m_rightPWM;
	
	/** 
	 * @brief Motor speed timer instance.
	 */
	FxTimer *m_MotorSpeedTimer;

	double m_encLeftPulses;

	double m_encRightPulses;

	/** 
	 * @brief Left motor RPM.
	 */
	double m_leftMotorRPM;

	/** 
	 * @brief Right motor RPM.
	 */
	double m_rightMotorRPM;

	/** 
	 * @brief Low Pass filter left speed.
	 */
	LowPassFilter *m_LPFLeftSpeed; // (2, 5, 1e3, true);

	/** 
	 * @brief Low Pass filter right speed.
	 */
	LowPassFilter *m_LPFRightSpeed; // (2, 5, 1e3, true);
	
#pragma endregion

#pragma region Methods

	/** @brief Function to convert from millimeters to steps.
	 *  @param mm float, Millimeters distance.
	 *  @return encoder counts.
	 */
	unsigned int MM2Steps(float mm);

	/** @brief Function to calculate motor speed.
	 *  @return Void.
	 */
	void calc_motors_speed();

#pragma endregion

 public:

#pragma region Methods

	/** @brief Initialize the bridge controller.
	 *  @param model MotorModel_t, Motor the controller.
	 *  @return Void.
	 */
	void init(MotorModel_t* model);

	/** @brief Update the bridge controller.
	 *  @return Void.
	 */
	void update();


	/** @brief Incremet the left encoder value.
	 *  @return Void.
	 */
	void UpdateLeftEncoder();

	/** @brief Incremet the right encoder value.
	 *  @return Void.
	 */
	void UpdateRightEncoder();

	/** @brief Function to Move Forward/Backwards.
	 *  @param mm float, Millimeters to be done.
	 *  @param mspeed int, input value holding values of the PWM.
	 *  @return Void.
	 */
	void MoveMM(float mm, int mspeed);

	/** @brief Control the PWM chanels of the H bridge for motor control.
	 *  @param left int16_t, input value holding values of the left pair PWMs.
	 *  @param right int16_t, input value holding values of the right pair PWMs.
	 *  @return Void.
	 */
	void SetPWM(int16_t left, int16_t right);



	/** @brief Function to Spin Right.
	 *  @param mm float, Millimeters to be done.
	 *  @param mspeed int, input value holding values of the PWM.
	 *  @return Void.
	 */
	void SpinRight(float mm, int mspeed);

	/** @brief Function to Spin Left.
	 *  @param mm float, Millimeters to be done.
	 *  @param mspeed int, input value holding values of the PWM.
	 *  @return Void.
	 */
	void SpinLeft(float mm, int mspeed);

	void MoveSpeed(int16_t left, int16_t right);

	/**
	 * @brief Get the Left Encoder value.
	 * 
	 * @return unsigned int Value
	 */
	long GetLeftEncoder();

	/**
	 * @brief Get the Right Encoder value.
	 * 
	 * @return unsigned int Value
	 */
	long GetRightEncoder();

	/**
	 * @brief Set the Left Encoder value.
	 * 
	 * @param value Value
	 */
	void SetLeftEncoder(long value);

	/**
	 * @brief Set the Right Encoder value.
	 * 
	 * @param value Value
	 */
	void SetRightEncoder(long value);

	/**
	 * @brief Get the Left Motor PWM value.
	 * 
	 * @return int16_t Value
	 */
	int16_t GetLeftMotor();

	/**
	 * @brief Get the Right Motor PWM value.
	 * 
	 * @return int16_t Value
	 */
	int16_t GetRightMotor();

	/**
	 * @brief Get the Left wheel RPM.
	 * 
	 * @return double RPM Value
	 */
	double GetLeftMotorRPM();

	/**
	 * @brief Get the Right wheel RPM.
	 * 
	 * @return double RPM Value
	 */
	double GetRightMotorRPM();

#pragma endregion

};

/** @brief Instance of the /h-Motor controller. */
extern MotorControllerClass MotorController;

#endif

