#define BLINK_INTERVAL 100

#include "OpenMOBot.h"
#include "FxTimer.h"
#include "LineSensor.h"
#include "DebugPort.h"

#pragma region Constants

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

bool CalibrationDone_g = false;

/**
  * @brief StateStatusLED_g used to set the LED.
  */
int StateStatusLED_g = LOW;

float LinePosition_g = 0.0;

/** 
 * @brief Blink timer instance.
 */
FxTimer *BlinkTimer_g;

/** 
 * @brief Send timer instance.
 */
FxTimer *LineSensorTimer_g;

/** @brief Read analog line sensor callback function.
 * 
 *  @param index int, Sensor index it exists in [0 to Sensor count -1].
 *  @return uint16_t Read sensor data.
 */
uint16_t read_sensor(int index)
{
	return analogRead(PinsLineSensor_g[index]);
}

void setup()
{
	configure_debug_port();

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
      Serial.println(LinePosition_g);
    }
  }
}
