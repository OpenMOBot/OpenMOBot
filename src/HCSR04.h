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

// HCSR04.h

#ifndef _HCSR04_h
#define _HCSR04_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// Undefine COMPILE_STD_DEV if you don't want Standard Deviation.
#define COMPILE_STD_DEV

typedef struct bufferCtl
{
	float *pBegin;
	float *pIndex;
	size_t length;
	bool filled;
} BufCtl;

class HCSR04
{

protected:
	int m_trigPin;
	int m_echoPin;
	float m_cmDivisor;
	float m_inDivisor;

#ifdef COMPILE_STD_DEV
	size_t m_numBufs;
	BufCtl *m_pBuffers;
	void sampleUpdate(BufCtl *buf, float msec);
	void freeBuffers();
#endif // COMPILE_STD_DEV

public:
	void init(int tp, int ep);
	long timing();
	float convert(long microsec, int metric);
	void setDivisor(float value, int metric);
	static const int IN = 0;
	static const int CM = 1;

#ifdef COMPILE_STD_DEV
	bool sampleCreate(size_t size, ...);
	void sampleClear();
	float unbiasedStdDev(float value, size_t bufNum);
#endif // COMPILE_STD_DEV
};

#endif
