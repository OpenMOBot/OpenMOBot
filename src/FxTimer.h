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
