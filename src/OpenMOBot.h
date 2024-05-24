/*

MIT License

Copyright (c) [2023] [OpenMOBOt]

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

// Notes.h

#ifndef _OPENMOBOT_h
#define _OPENMOBOT_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#pragma region GPIO Map

  // Check the microcontroller type
  #if defined(__AVR_ATmega328P__)

	/** @brief Line sensors count. */
	#define LINE_SENSORS_COUNT 6

	/** @brief Pin line sensor 1. */
	#define PIN_LS_1 A0

	/** @brief Pin line sensor 2. */
	#define PIN_LS_2 A1

	/** @brief Pin line sensor 3. */
	#define PIN_LS_3 A2

	/** @brief Pin line sensor 4. */
	#define PIN_LS_4 A3

	/** @brief Pin line sensor 5. */
	#define PIN_LS_5 A4

	/** @brief Pin line sensor 6. */
	#define PIN_LS_6 A5

	/** @brief Pin left encoder. */
	#define PIN_LEFT_ENCODER 3

	/** @brief Pin right encoder. */
	#define PIN_RIGHT_ENCODER 2

	/** @brief Pin left forward. */
	#define PIN_L_F 7

	/** @brief Pin left backward. */
	#define PIN_L_B 6

	/** @brief Pin left PWM. */
	#define PIN_L_PWM 10

	/** @brief Pin right forward. */
	#define PIN_R_F 5

	/** @brief Pin right backward. */
	#define PIN_R_B 4

	/** @brief Pin right PWM. */
	#define PIN_R_PWM 9

	/** @brief Pin ultrasonic servo PWM. */
	#define PIN_US_SERVO 11

	/** @brief Pin user LED */
	#define PIN_USER_LED 13

	/** @brief Pin ultra sonic trigger. */
	#define PIN_US_TRIG 8
	
	/** @brief Pin ultra sonic echo. */
	#define PIN_US_ECHO 12

  // Check the microcontroller type
  #elif defined(__AVR_ATmega2560__)

	/** @brief Line sensors count. */
	#define LINE_SENSORS_COUNT 8

	/** @brief Pin line sensor 1. */
	#define PIN_LS_1 8

	/** @brief Pin line sensor 2. */
	#define PIN_LS_2 9

	/** @brief Pin line sensor 3. */
	#define PIN_LS_3 10

	/** @brief Pin line sensor 4. */
	#define PIN_LS_4 11

	/** @brief Pin line sensor 5. */
	#define PIN_LS_5 12

	/** @brief Pin line sensor 6. */
	#define PIN_LS_6 13

	/** @brief Pin line sensor 7. */
	#define PIN_LS_7 14

	/** @brief Pin line sensor 8. */
	#define PIN_LS_8 15

	/** @brief Pin left encoder. */
	#define PIN_LEFT_ENCODER 3

	/** @brief Pin right encoder. */
	#define PIN_RIGHT_ENCODER 2

	/** @brief Pin left forward. */
	#define PIN_L_F 7

	/** @brief Pin left backward. */
	#define PIN_L_B 6

	/** @brief Pin left PWM. */
	#define PIN_L_PWM 10

	/** @brief Pin right forward. */
	#define PIN_R_F 5

	/** @brief Pin right backward. */
	#define PIN_R_B 4

	/** @brief Pin right PWM. */
	#define PIN_R_PWM 9

	/** @brief Pin ultrasonic servo PWM. */
	#define PIN_US_SERVO 11

	/** @brief Pin user LED */
	#define PIN_USER_LED 13
  
  // Check the microcontroller type
  #elif defined(ESP8266)
	#pragma message ("Unsupported ESP8266")

  // Check the microcontroller type
  #elif defined(ESP32)
	// https://mischianti.org/wp-content/uploads/2020/11/ESP32-DOIT-DEV-KIT-v1-pinout-mischianti.png

	/** @brief Line sensors count. */
	#define LINE_SENSORS_COUNT 6

	/** @brief Pin line sensor 1. */
	#define PIN_LS_1 2

	/** @brief Pin line sensor 2. */
	#define PIN_LS_2 4

	/** @brief Pin line sensor 3. */
	#define PIN_LS_3 35

	/** @brief Pin line sensor 4. */
	#define PIN_LS_4 34

	/** @brief Pin line sensor 5. */
	#define PIN_LS_5 36

	/** @brief Pin line sensor 6. */
	#define PIN_LS_6 39

	/** @brief Pin left encoder. */
	#define PIN_LEFT_ENCODER 25

	/** @brief Pin right encoder. */
	#define PIN_RIGHT_ENCODER 26

	/** @brief Pin left forward. */
	#define PIN_L_F 27

	/** @brief Pin left backward. */
	#define PIN_L_B 14

	/** @brief Pin left PWM. */
	#define PIN_L_PWM 5

	/** @brief Pin right forward. */
	#define PIN_R_F 17

	/** @brief Pin right backward. */
	#define PIN_R_B 16

	/** @brief Pin right PWM. */
	#define PIN_R_PWM 13

	/** @brief Pin ultrasonic servo PWM. */
	#define PIN_US_SERVO 23

	/** @brief Pin user LED */
	#define PIN_USER_LED 18

	/** @brief Pin ultra sonic trigger. */
	#define PIN_US_TRIG 12
	
	/** @brief Pin ultra sonic echo. */
	#define PIN_US_ECHO 19

  // Unsupported
  #else
	#pragma message ("Unsupported MCU")

  #endif

#pragma endregion

#pragma region Line Sensor

#define LINE_SENSORS_CALIBRATION_SIZE 50

#pragma endregion

#pragma region Wheels & Differential Model

// Float for number of slots in encoder disk
// Change to match value of encoder disk
#define ENCODER_TRACKS 20

#define WHEEL_DIAMETER 66.00F

#define DISTANCE_BETWEEN_WHEELS 130.00F

#pragma endregion

#define DEBOUNCE_TIME 100

#define DEFAULT_BAUD 115200

#endif

