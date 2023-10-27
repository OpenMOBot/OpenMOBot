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

// LineSensor.h

#ifndef _LINE_SENSOR_h
#define _LINE_SENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

// #include "DebugPort.h"
#ifndef DEBUGLOG
#define DEBUGLOG(...)
#endif


#define UPPER_HIGH 100
#define UPPER_LOW 80
#define LOWER_HIGH 20
#define LOWER_LOW 0

/** @brief Logic state description enum. */
enum SensorState : uint8_t
{
	S_LOW = 0U, ///< Logic LOW
	S_HIGH, ///< Logic HIGH
	S_Z, ///< High impedance.
};

class LineSensorClass
{
private:

#pragma region Variables

	/** @brief Callback function. */
	uint16_t(*callbackGetSensorValue)(int);

	/** @brief Line position */
	float m_linePosition;

	/** @brief Weighted total. */
	uint32_t m_weightedTotal = 0;

	/** @brief Denominator */
	uint16_t m_denominator = 0;

	/** @brief Sensors count. */
	uint8_t m_sensorsCount = 8;

	/** @brief Average filter count. */
	uint8_t m_avgFilterCount = 5;

	/** @brief Sensor resolution. */
	uint32_t m_resolution = 100;

	/* @brief On the line flag. */
	bool m_onTheLineFlag = false;

	/* @brief Inverted readings flag. */
	bool m_invertedReadings = false;

	/* @brief Average sensors values. */
	uint16_t * m_curSensorsValues;

	/* @brief Minimum sensors values. */
	uint16_t * m_minSensorsValues;

	/* @brief Maximum sensors values. */
	uint16_t * m_maxSensorsValues;

	/* @brief Actual sensors values. */
	uint16_t * m_actSensorsValues;

#pragma endregion

protected:

#pragma region Methods


	/** @brief Read a single sensor.
	 *  @param int sensor, Sensor index.
	 *  @return uint16_t, ADC sensor value.
	 */
	uint16_t readSensor(int sensorIndex);

	/** @brief Read a single sensor.
	 *  @param int sensor, Sensor index.
	 *  @return uint16_t, ADC filtered sensor value.
	 */
	uint16_t readFilteredSensor(int sensorIndex);

#pragma endregion

public:

#pragma region Methods

	/** @brief Configure the sensor.
	 *  @param sensorCount int, Sensor count.
	 *  @param calibrationSize int, Calibration size count.
	 *  @return Void.
	 */
	void init(int sensorCount);

	/** @brief Set the read callback.
	 *  @param callback, Callback pointer.
	 *  @return Void.
	 */
	void setCbReadSensor(uint16_t(*callback)(int));
	/** @brief Set inverted readings flag.
	 *  @param value bool, Inverted flag.
	 *  @return Void.
	 */
	void setInvertedReadings(bool value);

	/** @brief Get inverted readings flag.
	 *  @return bool, Inverted flag value.
	 */
	bool getInvertedReadings();

	/** @brief Set sensor resolution.
	 *  @param value int, Resolution value.
	 *  @return Void.
	 */
	void setResolution(int resolution);

	/** @brief Get gets resolution value.
	 *  @return int, Resolution value.
	 */
	int getResolution();


	/** @brief read a single sensor.
	 *  @param int sensor, Sensor index.
	 *  @return uint16_t, ADC filtered sensor value.
	 */
	void update();

	/** @brief Calibrate sensor array.
	 *  @return Void.
	 */
	void calibrate();

	/** @brief Read line position.
	 *  @return float, Weighted position determination.
	 */
	float getLinePosition();

	/** @brief Create hysteresis binarization.
	 *  @param int sensor, Sensor index.
	 *  @return bool, Threshold level.
	 */
	SensorState thresholdSensor(int sensorIndex);

	/**
	 * @brief Get the specified sensor value.
	 * 
	 * @param index Index of the sensor in array.
	 * @return uint16_t Value of the sensor.
	 */
	uint16_t getSensor(uint8_t index);

#pragma endregion

};

/** @brief Instance of the line sensor. */
extern LineSensorClass LineSensor;

#endif
