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

#include "FxTimer.h"

void FxTimer::update()
{
	m_now = millis();

	if ((m_now - m_last_time) >= m_expiration_time)
	{
		if (m_expired == false)
		{
			m_expired = true;
		}

		if (callbackExpiration != nullptr)
		{
			callbackExpiration(m_now);
		}
	}
}

void FxTimer::updateLastTime()
{
	m_last_time = millis();
}

void FxTimer::clear()
{
	if (m_expired == true)
	{
		m_expired = false;
	}
}

/** @brief Set callback for expiration time.
 *  @param Callback function.
 *  @return Void.
 */
void FxTimer::setExpirationCb(void(*callback)(unsigned long day))
{
	callbackExpiration = callback;
}

void FxTimer::setExpirationTime(unsigned long time)
{
	m_expiration_time = time;
}

unsigned long FxTimer::getExpirationTime()
{
	return m_expiration_time;
}


bool FxTimer::expired()
{
	return m_expired;
}
