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

#include "MotorController.h"

/** @brief Initialize the H bridge for motor control.
 *  @return Void.
 */
void MotorControllerClass::init(MotorModel_t *ModelL)
{
	m_motorModel = *ModelL;
	m_cntLeft = 0;
	m_cntRight = 0;
	m_dirCntLeft = 0;
	m_dirCntRight = 0;

	// Setup the motor driver.
	pinMode(m_motorModel.PinLeftForward, OUTPUT);
	pinMode(m_motorModel.PinRightForward, OUTPUT);
	pinMode(m_motorModel.PinLeftBackward, OUTPUT);
	pinMode(m_motorModel.PinRightBackward, OUTPUT);

	// Stop all directions
	digitalWrite(m_motorModel.PinLeftForward, 0);
	digitalWrite(m_motorModel.PinRightForward, 0);
	digitalWrite(m_motorModel.PinLeftBackward, 0);
	digitalWrite(m_motorModel.PinRightBackward, 0);

	// Stop all enables/PWMs.
	analogWrite(m_motorModel.PinLeftPWM, 0);
	analogWrite(m_motorModel.PinRightPWM, 0);

	m_MotorSpeedTimer = new FxTimer();
	m_MotorSpeedTimer->setExpirationTime(RPM_UPDATE_TIME);
	m_MotorSpeedTimer->updateLastTime();

	// Init the low pass filters.
	m_LPFLeftSpeed = new LowPassFilter(FILTER_ORDER, SUPPRESSION_FRQ, UPDATE_FRQ, FILTER_ADAPT);
	m_LPFRightSpeed = new LowPassFilter(FILTER_ORDER, SUPPRESSION_FRQ, UPDATE_FRQ, FILTER_ADAPT);
}

void MotorControllerClass::update()
{
	m_MotorSpeedTimer->update();
	if (m_MotorSpeedTimer->expired())
	{
		m_MotorSpeedTimer->updateLastTime();
		m_MotorSpeedTimer->clear();

		calc_motors_speed();
	}
}

/** @brief Update left encoder value.
 *  @return Void.
 */
void MotorControllerClass::UpdateLeftEncoder()
{
	// Increment left counter value.
	m_encLeftPulses++;

	if (m_dirCntLeft < 0)
	{
		// Decrement Motor Left counter value.
		m_cntLeft--;
	}
	else if (m_dirCntLeft > 0)
	{
		// Increment Motor Left counter value.
		m_cntLeft++;
	}
}

/** @brief Update right encoder value.
 *  @return Void.
 */
void MotorControllerClass::UpdateRightEncoder()
{
	// Increment right counter value.
	m_encRightPulses++;

	if (m_dirCntRight < 0)
	{
		// Decrement motor right counter value.
		m_cntRight--;
	}
	else if (m_dirCntRight > 0)
	{
		// Increment motor right counter value.
		m_cntRight++;
	}
}

// Function to convert from centimeters to steps
unsigned int MotorControllerClass::MM2Steps(float mm)
{
	// Final calculation result.
	unsigned int result;

	// Calculate wheel circumference in mm.
	float circumference = (m_motorModel.WheelDiameter * PI) / 10;

	// mm per Step.
	float mm_step = circumference / m_motorModel.EncoderTracks;

	// Calculate result as a float.
	float f_result = mm / mm_step;

	// Convert to an integer (note this is NOT rounded)
	result = (unsigned int)f_result;

	// End and return result.
	return result;
}

void MotorControllerClass::calc_motors_speed()
{
	// Declare motor speed, number of pulses and time elapsed
	static unsigned long PreviousTimeL = 0;
	static unsigned long CurrentTimeL = 0;
	static unsigned long DeltaTimeL = 0;
	static double LeftPulsesPerMsL = 0;
	static double RightPulsesPerMsL = 0;

	// Calculate motor speed based on the number of pulses and time elapsed
	CurrentTimeL = millis();
	DeltaTimeL = CurrentTimeL - PreviousTimeL;

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega2560__)
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
#endif
		// Calculate motor speed in pulses per millisecond
		LeftPulsesPerMsL = static_cast<double>(m_encLeftPulses) / static_cast<double>(DeltaTimeL);
		RightPulsesPerMsL = static_cast<double>(m_encRightPulses) / static_cast<double>(DeltaTimeL);

		// Reset encoder pulse count and update previous time
		m_encLeftPulses = 0;
		m_encRightPulses = 0;
		PreviousTimeL = CurrentTimeL;

		// Convert speed to desired units (e.g., RPM)
		m_leftMotorRPM = m_LPFLeftSpeed->filter(LeftPulsesPerMsL * (60000.0 / m_motorModel.EncoderTracks));
		m_rightMotorRPM = m_LPFRightSpeed->filter(RightPulsesPerMsL * (60000.0 / m_motorModel.EncoderTracks));

		// Set the sign.
		m_leftMotorRPM *= m_dirCntLeft;
		m_rightMotorRPM *= m_dirCntRight;

		// Apply average
		m_avgLeft += (m_leftMotorRPM - m_avgLeft) * m_K;
		m_avgRight += (m_leftMotorRPM - m_avgRight) * m_K;

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega2560__)
	}
#endif
}

/** @brief Control the PWM chanels of the H bridge for motor control.
 *  @param left int16_t, input value holding values of the left pair PWMs.
 *  @param right int16_t, input value holding values of the right pair PWMs.
 *  @return Void.
 */
void MotorControllerClass::SetPWM(int16_t left, int16_t right)
{
	if (left > PWM_MAX)
	{
		left = PWM_MAX;
	}

	if (left < PWM_MIN)
	{
		left = PWM_MIN;
	}

	if (right > PWM_MAX)
	{
		right = PWM_MAX;
	}

	if (right < PWM_MIN)
	{
		right = PWM_MIN;
	}

	// If the values are the same exit.
	if (m_leftPWM == left && m_rightPWM == right)
	{
		return;
	}

	// Else update new values.
	m_leftPWM = left;
	m_rightPWM = right;

	if (left > 0)
	{
		// Forward.
		m_dirCntLeft = 1;
		digitalWrite(m_motorModel.PinLeftBackward, LOW);
		digitalWrite(m_motorModel.PinLeftForward, HIGH);
		analogWrite(m_motorModel.PinLeftPWM, abs(left));
	}
	else if (left < 0)
	{
		// Revers.
		m_dirCntLeft = -1;
		digitalWrite(m_motorModel.PinLeftForward, LOW);
		digitalWrite(m_motorModel.PinLeftBackward, HIGH);
		analogWrite(m_motorModel.PinLeftPWM, abs(left));
	}
	else
	{
		m_dirCntLeft = 0;
		digitalWrite(m_motorModel.PinLeftForward, LOW);
		digitalWrite(m_motorModel.PinLeftBackward, LOW);
		analogWrite(m_motorModel.PinLeftPWM, 0);
	}

	if (right > 0)
	{
		// Forward.
		m_dirCntRight = 1;
		digitalWrite(m_motorModel.PinRightBackward, LOW);
		digitalWrite(m_motorModel.PinRightForward, HIGH);
		analogWrite(m_motorModel.PinRightPWM, abs(right));
	}
	else if (right < 0)
	{
		// Revers.
		m_dirCntRight = -1;
		digitalWrite(m_motorModel.PinRightForward, LOW);
		digitalWrite(m_motorModel.PinRightBackward, HIGH);
		analogWrite(m_motorModel.PinRightPWM, abs(right));
	}
	else
	{
		m_dirCntRight = 0;
		digitalWrite(m_motorModel.PinRightForward, LOW);
		digitalWrite(m_motorModel.PinRightBackward, LOW);
		analogWrite(m_motorModel.PinRightPWM, 0);
	}
}

/**
 * @brief Get the Left Encoder value.
 *
 * @return unsigned int Value
 */
long MotorControllerClass::GetLeftEncoder()
{
	return this->m_cntLeft;
}

/**
 * @brief Get the Right Encoder value.
 *
 * @return unsigned int Value
 */
long MotorControllerClass::GetRightEncoder()
{
	return this->m_cntRight;
}

/**
 * @brief Set the Left Encoder value.
 *
 * @param value Value
 */
void MotorControllerClass::SetLeftEncoder(long value)
{
	this->m_cntLeft = value;
}

/**
 * @brief Set the Right Encoder value.
 *
 * @param value Value
 */
void MotorControllerClass::SetRightEncoder(long value)
{
	this->m_cntRight = value;
}

/**
 * @brief Get the Left Motor PWM value.
 *
 * @return int16_t Value
 */
int16_t MotorControllerClass::GetLeftMotor()
{
	return this->m_leftPWM;
}

/**
 * @brief Get the Right Motor PWM value.
 *
 * @return int16_t Value
 */
int16_t MotorControllerClass::GetRightMotor()
{
	return this->m_rightPWM;
}

/**
 * @brief Get the Left wheel RPM.
 *
 * @return double RPM Value
 */
double MotorControllerClass::GetLeftMotorRPM()
{
	return this->m_leftMotorRPM;
}

/**
 * @brief Get the Right wheel RPM.
 *
 * @return double RPM Value
 */
double MotorControllerClass::GetRightMotorRPM()
{
	return this->m_rightMotorRPM;
}

/**
 * @brief Bridge controller instance.
 *
 */
MotorControllerClass MotorController;
