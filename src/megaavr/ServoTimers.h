/*
  Copyright (c) 2018 Arduino LLC. All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

/*
 * Defines for 16 bit timers used with Servo library
 *
 */

#ifndef __SERVO_TIMERS_H__
#define __SERVO_TIMERS_H__

#include <avr/io.h>

#if !defined(USE_TIMERB1) && !defined(USE_TIMERB2) && !defined(USE_TIMERB0)
    #define USE_TIMERB2  // default timer
#endif

static volatile TCB_t* _timer =
#if defined(USE_TIMERB0) // interferes with PWM on pin 6
&TCB0;
#endif
#if defined(USE_TIMERB1) // interferes with PWM on pin 3
&TCB1;
#endif
#if defined(USE_TIMERB2) // interferes with PWM on pin 11
&TCB2;
#endif

typedef enum {
    timer0,
    _Nbr_16timers } timer16_Sequence_t;


#endif  /* __SERVO_TIMERS_H__ */
