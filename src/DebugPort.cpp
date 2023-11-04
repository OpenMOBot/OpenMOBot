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

#include "DebugPort.h"

/** @brief Configure debug port.
 *  @return Void
 */
void configure_debug_port()
{

#ifdef ENABLE_DEBUG_OUT

	DEBUG_PORT.begin(DEBUG_PORT_BAUDRATE, SERIAL_8N1);
	DEBUG_PORT.print("\r\n\r\n\r\n");
	//DEBUG_PORT.setDebugOutput(true);

	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");

#endif // ENABLE_DEBUG_OUT

}
