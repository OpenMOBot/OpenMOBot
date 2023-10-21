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

#include "LowPassFilter.h"

LowPassFilter::LowPassFilter(int order, float f0, float fs, bool adaptive)
{
	m_order = order;
	
	m_a = new float[m_order];

    m_b = new float[m_order+1];

    m_x = new float[m_order+1]; // Raw values

    m_y = new float[m_order+1]; // Filtered values

      // f0: cutoff frequency (Hz)
      // fs: sample frequency (Hz)
      // adaptive: boolean flag, if set to 1, the code will automatically set
      // the sample frequency based on the time history.
      
      m_omega0 = 6.28318530718*f0;
      m_dt = 1.0 / fs;
      m_adaptive = adaptive;
      m_tn1 = -m_dt;

      for(int k = 0; k < m_order+1; k++)
      {
        m_x[k] = 0;
        m_y[k] = 0;        
      }

      setCoef();
    }

void LowPassFilter::setCoef()
{
  if (m_adaptive)
  {
    float t = micros() / 1.0e6;
    m_dt = t - m_tn1;
    m_tn1 = t;
  }
  
  float alpha = m_omega0 * m_dt;
  if (m_order == 1)
  {
    m_a[0] = -(alpha - 2.0) / (alpha+2.0);
    m_b[0] = alpha / (alpha+2.0);
    m_b[1] = alpha / (alpha+2.0);        
  }

  if (m_order == 2)
  {
    float alphaSq = alpha * alpha;
    float beta[] = {1, sqrt(2), 1};
    float D = alphaSq * beta[0] + 2 * alpha * beta[1] + 4 * beta[2];
    m_b[0] = alphaSq / D;
    m_b[1] = 2 * m_b[0];
    m_b[2] = m_b[0];
    m_a[0] = -(2 * alphaSq * beta[0] - 8 * beta[2]) / D;
    m_a[1] = -(beta[0] * alphaSq - 2 * beta[1] * alpha + 4 * beta[2]) / D;
  }
}

float LowPassFilter::filter(float xn)
{
  // Provide me with the current raw value: x
  // I will give you the current filtered value: y
  if(m_adaptive)
  {
    setCoef(); // Update coefficients if necessary      
  }

  m_y[0] = 0;
  m_x[0] = xn;

  // Compute the filtered values
  for(int k = 0; k < m_order; k++)
  {
    m_y[0] += m_a[k] * m_y[k+1] + m_b[k] * m_x[k];
  }
  m_y[0] += m_b[m_order] * m_x[m_order];

  // Save the historical values
  for(int k = m_order; k > 0; k--)
  {
    m_y[k] = m_y[k-1];
    m_x[k] = m_x[k-1];
  }

  // Return the filtered value    
  return m_y[0];
}
