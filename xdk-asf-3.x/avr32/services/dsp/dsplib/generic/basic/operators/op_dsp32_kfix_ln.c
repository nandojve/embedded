/*****************************************************************************
 *
 * \file
 *
 * \brief Natural logarithm operator for the AVR32 UC3.
 *
 * Copyright (c) 2009 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 *****************************************************************************/


#include "dsp.h"
#include "preprocessor.h"

#if defined(FORCE_ALL_GENERICS) || \
    defined(FORCE_GENERIC_OP32_LN) || \
    !defined(TARGET_SPECIFIC_OP32_LN)

S64 dsp32_op_ln_raw(dsp32_t num)
{
  dsp32_t num_temp;
  S64 a, a_sqr, res;
  int under_bit_val;

  // Limit
  if (num <= DSP32_Q(0.))
    return DSP_Q_MIN(DSP32_QA, DSP32_QB);

// Remove warnings
#if DSP32_QA > 1
  // get ~ int(log2(num))
  // num is in the range [1; 0.5]
  if (num > DSP32_Q(1.))
  {
    under_bit_val = 0;
    num_temp = num;
    while (num_temp > DSP32_Q(1.))
    {
      num_temp >>= 1;
      under_bit_val++;
    }
    num_temp = (num >> under_bit_val);
  }
  else
#endif
  {
    under_bit_val = 0;
    num_temp = num;
    while (num_temp < DSP32_Q(0.5))
    {
      num_temp <<= 1;
      under_bit_val++;
    }
    num_temp = (num << under_bit_val);
  }

  // << (DSP32_QB - 1) to avoid overflows
  a = (((S64) num_temp) - DSP32_Q(1.)) << DSP32_QB;
  a = a/(((S64) num_temp) + DSP32_Q(1.));
  // calculate a^2
  a_sqr = (a*a) >> DSP32_QB;

  res = (a_sqr*DSP32_Q(1./13.)) >> DSP32_QB;
  res = (a_sqr*(res + DSP32_Q(1./11.))) >> DSP32_QB;
  res = (a_sqr*(res + DSP32_Q(1./9.))) >> DSP32_QB;
  res = (a_sqr*(res + DSP32_Q(1./7.))) >> DSP32_QB;
  res = (a_sqr*(res + DSP32_Q(1./5.))) >> DSP32_QB;
  res = (a_sqr*(res + DSP32_Q(1./3.))) >> DSP32_QB;
  res = (a*(res + DSP32_Q(1.))) >> (DSP32_QB - 1);

// Remove warnings
#if DSP32_QA > 1
  if (num > DSP32_Q(1.))
    res += ((S64) under_bit_val)*((S64) DSP32_Q(CST_LN_2));
  else
#endif
    res -= ((S64) under_bit_val)*((S64) DSP32_Q(CST_LN_2));

  return res;
}

#endif
