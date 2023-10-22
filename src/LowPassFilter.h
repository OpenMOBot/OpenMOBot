/*

MIT License

Copyright (c) [2019] [OpenMOBot]

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

// LowPassFilter.h

#ifndef _LOWPASSFILTER_h
#define _LOWPASSFILTER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class LowPassFilter
{
  protected:

    bool m_adaptive;

    float *m_a;

    float *m_b;

    float m_omega0;

    float m_dt;

    float m_tn1 = 0;

    float *m_x; // Raw values

    float *m_y; // Filtered values

	int m_order; // 1 or 2 order.

  public:  

    LowPassFilter(int order, float f0, float fs, bool adaptive);

    void setCoef();

    float filter(float xn);
};

#endif // _LOWPASSFILTER_h
