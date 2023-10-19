// 
// 
// 

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
