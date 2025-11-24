#include <stdint.h>
#include <stdbool.h>
#include "calcEcuTemp.h"
/**
 * @file calcEcuTemp.c
 * @brief ECU temperature calculation implementation.
 *
 * This source file implements @ref Calc_ecuTemperature_s16, which computes
 * an ECU temperature estimate by combining an NTC-based measurement with
 * a system temperature measurement according to a piecewise algorithm
 * controlled by @ref PAR_ECU_TEMP_LOW_THR and @ref PAR_ECU_TEMP_UP_THR.
 */
/**
 * @brief Latest NTC temperature measurement [°C].
 *
 * This static variable stores the most recent temperature value coming
 * from the NTC sensor. It is used by @ref Calc_ecuTemperature_s16 as:
 * - A direct temperature source at high temperatures.
 * - One of the endpoints in the linear interpolation region.
 */
static sint16 NtcTemp_s16;

/**
 * @brief Latest system temperature measurement [°C].
 *
 * This static variable stores the most recent system temperature value,
 * typically coming from a different sensor or estimation path.
 * It is used by @ref Calc_ecuTemperature_s16 as:
 * - Backup/fallback temperature at low temperatures or when the NTC is faulty.
 * - One of the endpoints in the linear interpolation region.
 */
static sint16 SysTemp_s16;

/* External flag (declared elsewhere) indicating NTC error state. */
static bool NtcErr_b;


sint16 Calc_ecuTemperature_s16(void)
{
  sint16 l_ecuTemp_s16 = 0;

  if ( (NtcTemp_s16 <= PAR_ECU_TEMP_LOW_THR) || (true == NtcErr_b) )
  {
    // Use System temperature for low temperatures and if the NTC is broken
    l_ecuTemp_s16 = SysTemp_s16;
  }
  else if ( NtcTemp_s16 >= PAR_ECU_TEMP_UP_THR )
  {
    // use NTC for high temperature
    l_ecuTemp_s16 = NtcTemp_s16;
  }
  else
  {
    // Balance the two measurements in a middle range
    l_ecuTemp_s16 = (NtcTemp_s16 * (NtcTemp_s16 - PAR_ECU_TEMP_LOW_THR) + 
                      (SysTemp_s16 * (PAR_ECU_TEMP_UP_THR - NtcTemp_s16)) ) / 
                                  (PAR_ECU_TEMP_UP_THR - PAR_ECU_TEMP_LOW_THR);
  }

  return l_ecuTemp_s16;
}


int main()
{
  return Calc_ecuTemperature_s16();
}