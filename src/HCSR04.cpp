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

#include <stdlib.h>
#include <string.h>
#include "HCSR04.h"

void HCSR04::init(int tp, int ep)
{
	pinMode(tp, OUTPUT);
	pinMode(ep, INPUT);
	m_trigPin = tp;
	m_echoPin = ep;
	m_cmDivisor = 27.6233;
	m_inDivisor = 70.1633;
}

long HCSR04::timing()
{
	digitalWrite(m_trigPin, LOW);
	delayMicroseconds(2);
	digitalWrite(m_trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(m_trigPin, LOW);
	return pulseIn(m_echoPin, HIGH);
}

float HCSR04::convert(long microsec, int metric)
{
	// microsec / 29 / 2;
	if (metric)
		return microsec / m_cmDivisor / 2.0; // CM
	// microsec / 74 / 2;
	else
		return microsec / m_inDivisor / 2.0; // IN
}

void HCSR04::setDivisor(float value, int metric)
{
	if (metric)
		m_cmDivisor = value;
	else
		m_inDivisor = value;
}

#ifdef COMPILE_STD_DEV
bool HCSR04::sampleCreate(size_t numBufs, ...)
{
	bool result = false;
	va_list ap;
	m_numBufs = numBufs;

	if ((m_pBuffers = (BufCtl *)calloc(numBufs, sizeof(BufCtl))) != NULL)
	{
		va_start(ap, numBufs);
		BufCtl *buf;
		size_t smpSize;

		for (size_t i = 0; i < m_numBufs; i++)
		{
			buf = &m_pBuffers[i];
			smpSize = va_arg(ap, size_t);

			if ((buf->pBegin = (float *)calloc(smpSize, sizeof(float))) != NULL)
			{
				buf->pIndex = buf->pBegin;
				buf->length = smpSize;
				buf->filled = false;
				result = true;
			}
			else
			{
				result = false;
				break;
			}
		}

		va_end(ap);
	}

	if (!result)
		freeBuffers();
	return result;
}

void HCSR04::sampleClear()
{
	if (m_pBuffers)
	{
		BufCtl *buf;

		for (size_t i = 0; i < m_numBufs; i++)
		{
			buf = &m_pBuffers[i];
			memset(buf, '\0', sizeof(float) * buf->length);
			buf->pIndex = buf->pBegin;
			buf->filled = false;
		}
	}
}

float HCSR04::unbiasedStdDev(float value, size_t bufNum)
{
	float result = 0.0;

	if (m_pBuffers)
	{
		BufCtl *buf = &m_pBuffers[bufNum];

		if (buf->length > 1)
		{
			sampleUpdate(buf, float(value));

			if (buf->filled)
			{
				float sum = 0.0, mean, tmp;

				for (size_t i = 0; i < buf->length; i++)
					sum += buf->pBegin[i];

				mean = sum / buf->length;
				sum = 0.0;

				for (size_t i = 0; i < buf->length; i++)
				{
					tmp = buf->pBegin[i] - mean;
					sum += (tmp * tmp);
				}

				result = sqrt(sum / (buf->length - 1));
				// DEBUGLOG(bufNum);
				// DEBUGLOG(" : ");
				// DEBUGLOG(result);
				// DEBUGLOG("\r\n");
			}
		}
	}

	return result;
}

void HCSR04::sampleUpdate(BufCtl *buf, float msec)
{
	if (buf->pIndex >= (buf->pBegin + buf->length))
	{
		buf->pIndex = buf->pBegin;
		buf->filled = true;
	}

	*(buf->pIndex++) = msec;
}

void HCSR04::freeBuffers()
{
	if (m_pBuffers)
	{
		BufCtl *buf;

		for (size_t i = 0; i < m_numBufs; i++)
		{
			buf = &m_pBuffers[i];
			free(buf->pBegin);
		}

		free(m_pBuffers);
	}
}
#endif // COMPILE_STD_DEV
