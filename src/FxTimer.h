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

// FxTimer.h

#ifndef _FXTIMER_h
#define _FXTIMER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class FxTimer
{
 protected:

	 /** @brief Expired flag. */
	 bool m_expired = false;

	 /** @brief Expiration time. */
	 unsigned long m_expiration_time = 0;
	 
	 /** @brief Last time. */
	 unsigned long m_last_time = 0;
	 
	 /** @brief Current time. */
	 unsigned long m_now = 0;
	 
	 /** @brief Callback when expire. */
	 void(*callbackExpiration)(unsigned long now);

 public:

	 void update();

	 void updateLastTime();

	 void clear();

	 void setExpirationCb(void(*callback)(unsigned long day));

	 void setExpirationTime(unsigned long);

	 unsigned long getExpirationTime();

	 bool expired();
};

#endif
