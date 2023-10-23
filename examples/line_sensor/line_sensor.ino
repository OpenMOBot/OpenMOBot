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

#define BLINK_INTERVAL 100

#pragma endregion

#pragma region Headers

#include "OpenMOBot.h"
#include "FxTimer.h"
#include "LineSensor.h"

#ifdef DEBUG_TEXT
#include "DebugPort.h"
#endif

#pragma endregion

#pragma region Functions Prototypes

/** @brief Read analog line sensor callback function.
 * 
 *  @param index int, Sensor index it exists in [0 to Sensor count -1].
 *  @return uint16_t Read sensor data.
 */
uint16_t read_sensor(int index);

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

/**
  * @brief Line sensor calibration flag.
  */
bool CalibrationDone_g = false;

/**
  * @brief Line sensor position.
  */
float LinePosition_g = 0.0;

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
FxTimer *LineSensorTimer_g;

#pragma endregion

void setup()
{
#ifdef DEBUG_TEXT
	configure_debug_port();
#endif

#ifdef DEBUG_OSC
  Serial.begin(DEFAULT_BAUD);
#endif

	// Initialize the line sensor.
	LineSensor.init(LINE_SENSORS_COUNT);
	LineSensor.setCbReadSensor(read_sensor);
	LineSensor.setInvertedReadings(false);

  pinMode(PIN_USER_LED, OUTPUT);

  BlinkTimer_g = new FxTimer();
  BlinkTimer_g->setExpirationTime(100);
  BlinkTimer_g->updateLastTime();

  LineSensorTimer_g = new FxTimer();
  LineSensorTimer_g->setExpirationTime(100);
  LineSensorTimer_g->updateLastTime();
}

void loop()
{
  LineSensor.update();

  BlinkTimer_g->update();
  if(BlinkTimer_g->expired())
  {
    BlinkTimer_g->updateLastTime();
    BlinkTimer_g->clear();

    // set the LED with the StateStatusLED_g of the variable:
    StateStatusLED_g = !StateStatusLED_g;
    digitalWrite(PIN_USER_LED, StateStatusLED_g);
  }

  LineSensorTimer_g->update();
  if(LineSensorTimer_g->expired())
  {
    LineSensorTimer_g->updateLastTime();
    LineSensorTimer_g->clear();

    static int CalibartionsL = 0;

		if (CalibartionsL < LINE_SENSORS_CALIBRATION_SIZE && CalibrationDone_g == false)
		{
			LineSensor.calibrate();
			CalibartionsL++;
		}
		else
		{
			CalibartionsL = 0;
      CalibrationDone_g = true;
      BlinkTimer_g->setExpirationTime(500);
		}

    if (CalibrationDone_g)
    {
  		// Get Road conditions.
	  	LinePosition_g = LineSensor.getLinePosition();
#ifdef DEBUG_OSC
      Serial.println(LinePosition_g);
#endif
    }
  }
}

#pragma region Functions

/** @brief Read analog line sensor callback function.
 * 
 *  @param index int, Sensor index it exists in [0 to Sensor count -1].
 *  @return uint16_t Read sensor data.
 */
uint16_t read_sensor(int index)
{
	return analogRead(PinsLineSensor_g[index]);
}

#pragma endregion
