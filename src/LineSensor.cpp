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

#include "LineSensor.h"

/** @brief Configure the sensor.
 *  @param sensorCount int, Sensor count.
 *  @param calibrationSize int, Calibration size count.
 *  @return Void.
 */
void LineSensorClass::init(int sensorCount)
{
	m_sensorsCount = sensorCount;

	/* @brief Average sensors values. */
	m_curSensorsValues = new uint16_t[m_sensorsCount];

	/* @brief Minimum sensors values. */
	m_minSensorsValues = new uint16_t[m_sensorsCount];

	/* @brief Maximum sensors values. */
	m_maxSensorsValues = new uint16_t[m_sensorsCount];

	/* @brief Actual sensors values. */
	m_actSensorsValues = new uint16_t[m_sensorsCount];

	// Init
	for (uint8_t index = 0; index < m_sensorsCount; index++)
	{
		m_curSensorsValues[index] = 0;
		m_minSensorsValues[index] = 1023;
		m_maxSensorsValues[index] = 0;
		m_actSensorsValues[index] = 0;
	}
}

/** @brief read a single sensor.
 *  @param int sensor, Sensor index.
 *  @return uint16_t, ADC filtered sensor value.
 */
void LineSensorClass::update()
{
	static uint16_t MaxValueL;
	static uint16_t MinValueL;

	for (uint8_t index = 0; index < m_sensorsCount; index++)
	{
		m_curSensorsValues[index] = readFilteredSensor(index);
	}
	
	// Clamp and scale to resolution.
	for (uint8_t index = 0; index < m_sensorsCount; index++)
	{
		MaxValueL = max(m_curSensorsValues[index], m_maxSensorsValues[index]);
		MinValueL = min(m_curSensorsValues[index], m_minSensorsValues[index]);
		m_actSensorsValues[index] = map(m_curSensorsValues[index], MinValueL, MaxValueL, 0, m_resolution);
	}

	// Reuse max and min values, so clear it.
	MinValueL = m_resolution;
	MaxValueL = 0;

	// Extract minimums and maximums from local resolution.
	for (uint8_t index = 0; index < m_sensorsCount; index++)
	{
		if (m_actSensorsValues[index] < MinValueL)
		{
			MinValueL = m_actSensorsValues[index];
		}
		if (m_actSensorsValues[index] > MaxValueL)
		{
			MaxValueL = m_actSensorsValues[index];
		}
	}

	// Scale and make it to dynamic range.
	for (uint8_t index = 0; index < m_sensorsCount; index++)
	{
		m_actSensorsValues[index] = map(m_actSensorsValues[index], MinValueL, MaxValueL, 0, m_resolution);
	}
}

/** @brief Calibrate sensor array.
 *  @return bool, True when calibrated.
 */
void LineSensorClass::calibrate()
{
	static uint16_t MaxValueL;
	static uint16_t MinValueL;

	// Find minimums and maximums.
	for (uint8_t index = 0; index < m_sensorsCount; index++)
	{
		m_curSensorsValues[index] = readFilteredSensor(index);

		if (m_curSensorsValues[index] < m_minSensorsValues[index])
		{
			m_minSensorsValues[index] = m_curSensorsValues[index];
		}

		if (m_curSensorsValues[index] > m_maxSensorsValues[index])
		{
			m_maxSensorsValues[index] = m_curSensorsValues[index];
		}
	}

	// Clamp and scale to resolution.
	for (uint8_t index = 0; index < m_sensorsCount; index++)
	{
		MaxValueL = max(m_curSensorsValues[index], m_maxSensorsValues[index]);
		MinValueL = min(m_curSensorsValues[index], m_minSensorsValues[index]);
		m_actSensorsValues[index] = map(m_curSensorsValues[index], MinValueL, MaxValueL, 0, m_resolution);
	}

	// Reuse max and min values, so clear it.
	MinValueL = m_resolution;
	MaxValueL = 0;

	// Extract minimums and maximums from local resolution.
	for (uint8_t index = 0; index < m_sensorsCount; index++)
	{
		if (m_actSensorsValues[index] < MinValueL)
		{
			MinValueL = m_actSensorsValues[index];
		}
		if (m_actSensorsValues[index] > MaxValueL)
		{
			MaxValueL = m_actSensorsValues[index];
		}
	}

	// Scale and make it to dynamic range.
	for (uint8_t index = 0; index < m_sensorsCount; index++)
	{
		m_actSensorsValues[index] = map(m_actSensorsValues[index], MinValueL, MaxValueL, 0, m_resolution);
	}

	DEBUGLOG("\r\n");
	DEBUGLOG("Actual: ");
	for (uint8_t index = 0; index < m_sensorsCount; index++)
	{
		DEBUGLOG(m_actSensorsValues[index]);
		DEBUGLOG(", ");
	}
	DEBUGLOG("\r\n");


	DEBUGLOG("\r\n");
	DEBUGLOG("Current: ");
	for (uint8_t index = 0; index < m_sensorsCount; index++)
	{
		DEBUGLOG(m_curSensorsValues[index]);
		DEBUGLOG(", ");
	}
	DEBUGLOG("\r\n");

	DEBUGLOG("Minimum: ");
	for (uint8_t index = 0; index < m_sensorsCount; index++)
	{
		DEBUGLOG(m_minSensorsValues[index]);
		DEBUGLOG(", ");
	}
	DEBUGLOG("\r\n");

	DEBUGLOG("Maximum: ");
	for (uint8_t index = 0; index < m_sensorsCount; index++)
	{
		DEBUGLOG(m_maxSensorsValues[index]);
		DEBUGLOG(", ");
	}
	DEBUGLOG("\r\n");
}



/** @brief Set the read callback.
 *  @param callback, Callback pointer.
 *  @return Void.
 */
void LineSensorClass::setCbReadSensor(uint16_t(*callback)(int))
{
	callbackGetSensorValue = callback;
}


/** @brief Set inverted readings flag.
 *  @param value bool, Inverted flag.
 *  @return Void.
 */
void LineSensorClass::setInvertedReadings(bool value)
{
	m_invertedReadings = value;
}

/** @brief Get inverted readings flag.
 *  @return bool, Inverted flag value.
 */
bool LineSensorClass::getInvertedReadings()
{
	return m_invertedReadings;
}

/** @brief Set sensor resolution.
 *  @param value int, Resolution value.
 *  @return Void.
 */
void LineSensorClass::setResolution(int value)
{
	m_resolution = value;
}

/** @brief Get gets resolution value.
 *  @return int, Resolution value.
 */
int LineSensorClass::getResolution()
{
	return m_resolution;
}


/** @brief Read a single sensor.
 *  @param int sensor, Sensor index.
 *  @return uint16_t, ADC sensor value.
 */
uint16_t LineSensorClass::readSensor(int sensorIndex)
{
	if (callbackGetSensorValue != nullptr)
	{
		return callbackGetSensorValue(sensorIndex);
	}

	return 0;
}

/** @brief Read a single sensor.
 *  @param int sensor, Sensor index.
 *  @return uint16_t, ADC filtered sensor value.
 */
uint16_t LineSensorClass::readFilteredSensor(int sensorIndex)
{
	uint16_t SensorValueL = 0;

	for (int index = 0; index < m_avgFilterCount; index++)
	{
		SensorValueL += readSensor(sensorIndex);
	}

	//if (SensorValueL <= 0)
	//{
	//	return 0;
	//}

	SensorValueL /= m_avgFilterCount;

	return SensorValueL;
}

/** @brief Create hysteresis binarization.
 *  @param int sensor, Sensor index.
 *  @return bool, Threshold level.
 */
SensorState LineSensorClass::thresholdSensor(int sensorIndex)
{
	SensorState StateL = S_Z;

	if (UPPER_HIGH <= m_curSensorsValues[sensorIndex]
		&& UPPER_LOW >= m_curSensorsValues[sensorIndex])
	{
		StateL = S_HIGH;
	}

	if (LOWER_HIGH <= m_curSensorsValues[sensorIndex]
		&& LOWER_LOW >= m_curSensorsValues[sensorIndex])
	{
		StateL = S_LOW;
	}

	return StateL;
}

/** @brief Read line position.
 *  @return float, Weighted position determination.
 */
float LineSensorClass::getLinePosition()
{
	m_weightedTotal = 0;
	m_denominator = 0;
	m_linePosition = 0;
	m_onTheLineFlag = false;

	// Determine line position.
	for (uint8_t index = 0; index < m_sensorsCount; index++)
	{
		uint16_t value = m_actSensorsValues[index];

		if (m_invertedReadings)
		{
			value = m_resolution - value;
		}

		// keep track of whether we see the line at all
		if (value > 70)
		{
			m_onTheLineFlag = true;
		}

		// Only average in values that are above a noise threshold
		if (value > 50)
		{
			m_weightedTotal += (uint32_t)value * (index * m_resolution);
			m_denominator += value;
		}
	}

	if (m_onTheLineFlag == false)
	{
		// If it last read to the left of center, return 0.
		if (m_linePosition < (m_sensorsCount - 1) * m_resolution / 2)
		{
			return 0;
		}
		// If it last read to the right of center, return the max.
		else
		{
			return (m_sensorsCount - 1) * m_resolution;
		}
	}
	
	DEBUGLOG("\r\n");
	DEBUGLOG("Actual: ");
	for (uint8_t index = 0; index < m_sensorsCount; index++)
	{
		DEBUGLOG(m_actSensorsValues[index]);
		DEBUGLOG(", ");
	}
	DEBUGLOG("\r\n");

	m_linePosition = m_weightedTotal / m_denominator;
	return m_linePosition;
}

/**
 * @brief Get the specified sensor value.
 * 
 * @param index Index of the sensor in array.
 * @return uint16_t Value of the sensor.
 */
uint16_t LineSensorClass::getSensor(uint8_t index)
{
	return m_curSensorsValues[index];
}

/**
 * @brief Line sensor instance.
 * 
 */
LineSensorClass LineSensor;
