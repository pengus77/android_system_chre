#ifndef HAP_PERF_H
#define HAP_PERF_H
/*==============================================================================
  Copyright (c) 2012-2017 Qualcomm Technologies, Inc.
  All rights reserved. Qualcomm Proprietary and Confidential.
==============================================================================*/

#include "AEEStdDef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 
  HAP_perf_get_time_us 
   
  Gets the current micro-seconds conversion of the 56 bit, 19.2MHz global
  hardware clock count.
 
  Returns the current value of a 56-bit hardware counter in micro-seconds.  This
  hardware counter is shared amongst all the cores on the device.
 
  The value wraps around to zero when it exceeds the maximum value.  This
  operation must be used with care because of the wrap-around behavior and
  should be treated as relative rather than absolute.
 
  Returns:
  Integer -- Current value of 56-bit hardware counter converted to micro-seconds
*/
uint64 HAP_perf_get_time_us(void);

/* 
  HAP_perf_get_qtimer_count 
   
  Gets the current 56 bit, 19.2MHz global hardware clock count.
   
  Returns the current value of a 56-bit hardware counter.  This hardware counter
  is shared amongst all the cores on the device.
 
  The value wraps around to zero when it exceeds the maximum value. This
  operation must be used with care because of the wrap-around behavior and
  should be treated as relative rather than absolute.

  Returns:
  Integer -- Current count of global hardware clock
*/
uint64 HAP_perf_get_qtimer_count(void);

/* 
  HAP_perf_qtimer_count_to_us 
   
  Converts from a 19.2 MHz global hardware count to micro-seconds.
   
  Returns:
  Integer -- micro-second conversion of the supplied count
*/
uint64 HAP_perf_qtimer_count_to_us(uint64 count);

/* 
  HAP_perf_get_pcycles 
   
  Gets the current 64-bit processor cycle count
   
  The processor cycle count is the current number of processor cycles executed
  since the Hexagon processor was last reset.
 
  Note that this counter stops incrementing whenever the DSP enters a low-power
  state (such as clock gating), as opposed to the qtimer, which increments 
  regardless of the DSP power state.

	
  Returns:
  Integer -- Current count of Hexagon processor cycle count.
*/
uint64 HAP_perf_get_pcycles(void);

#ifdef __cplusplus
}
#endif

#endif // HAP_PERF_H
