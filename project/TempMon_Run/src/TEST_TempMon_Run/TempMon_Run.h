/**
 * @file tempMon.h
 * @brief ECU temperature monitoring and overtemperature diagnostic handling.
 *
 * This module monitors the ECU temperature and manages an overtemperature
 * diagnostic state with hysteresis and time-based debouncing. The main API
 * is @ref TempMon_Run, which should be called periodically (e.g., every 100 ms).
 */

#ifndef TEMPMON_H
#define TEMPMON_H

#include <stdint.h>

/** AUTOSAR-like types for clarity */
typedef int16_t  sint16;
typedef int8_t   sint8;
typedef uint8_t  uint8;

extern uint8 count_u8;

/** Number of scheduler ticks corresponding to 500ms */
#define COUNT_500MS               (5u)
/** Number of scheduler ticks corresponding to 2000ms (unused in TempMon_Run) */
#define COUNT_2000MS              (20u)

/** Upper threshold for overtemperature condition [°C] */
#define TEMPMON_OVERTEMP_UPTH     (135)
/** Lower threshold for overtemperature condition [°C] */
#define TEMPMON_OVERTEMP_LWTH     (130)





/*==============================================================*/
/*   TEMPMON_RUN                                                */
/*==============================================================*/

/**
 * @brief Execute the ECU temperature monitoring and overtemperature debounce.
 *
 * @details
 * **Function goal**
 *
 * The purpose of @ref TempMon_Run is to:
 * - Monitor the filtered ECU temperature.
 * - Detect overtemperature conditions using an upper and lower threshold
 *   with hysteresis.
 * - Apply a time-based debounce (using an internal counter @c count_u8)
 *   to avoid spurious state changes due to short spikes or noise.
 * - Update the overtemperature diagnostic state via
 *   @ref EC_Set_error_overtemperature_State_e.
 *.
 *
 * Debounce filtering is provided by @ref COUNT_500MS and the internal counter  @ref count_u8 
 * @par Interface summary
 *
 * | Interface                            | In | Out | Data type         | Param            | Data factor | Data offset | Data size | Data range | Data unit |
 * |--------------------------------------|:--:|:---:|-------------------|------------------|------------:|------------:|----------:|------------|-----------|
 * | TempMeas_get_filtEcuTemp_s16         | ✓  |     | sint16            | (void)           | 1           | 0           | 1         | [-40, 150] | [°C]      |
 * | TEMPMON_OVERTEMP_UPTH                | ✓  |     | macro             | –                | 1           | 0           | n/a       | [135]      | [°C]      |
 * | TEMPMON_OVERTEMP_LWTH                | ✓  |     | macro             | –                | 1           | 0           | n/a       | [130]      | [°C]      |
 * | COUNT_500MS                          | ✓  |     | macro             | –                | 1           | 0           | n/a       | [5]        | [-]       |
 * | EC_Get_error_overtemperature_State_e | ✓  |     | ErrorFlagState_t  | (void)           | 1           | 0           | 1         | [0..3]     | [-]       |
 * | EC_Set_error_overtemperature_State_e |    | ✓   | void              |(ErrorFlagState_t)| 1           | 0           | 1         | [0..3]     | [-]       |
 * | count_u8                             | ✓  | ✓   | uint8             | –                | 1           | 0           | 1         | [0..255]   | [-]       |
 *
 * @par Activity diagram (PlantUML)
 * @startuml
 * start
 * 
 * :Read ECU temperature\n tempEcu_s16 = TempMeas_get_filtEcuTemp_s16();
 * 
 * if (tempEcu_s16 >= TEMPMON_OVERTEMP_UPTH?) then (yes)
 *   if (EC_Get_error_overtemperature_State_e() == EFS_TRUE?) then (yes)
 *     :Reset debounce counter\n count_u8 = 0;
 *   else (no)
 *     :Increment debounce counter\n count_u8++;
 *     if (count_u8 > COUNT_500MS?) then (yes)
 *       :Reset debounce counter\n count_u8 = 0;
 *       :Set diagnostic state\n EC_Set_error_overtemperature_State_e(EFS_TRUE);
 *     endif
 *   endif
 * else (no)
 *   if (EC_Get_error_overtemperature_State_e() == EFS_TRUE?) then (yes)
 *     :Increment debounce counter\n count_u8++;
 *     if (count_u8 > COUNT_500MS?) then (yes)
 *       :Reset debounce counter\n count_u8 = 0;
 *       :Clear diagnostic state\n EC_Set_error_overtemperature_State_e(EFS_FALSE);
 *     endif
 *   else (no)
 *     :Reset debounce counter\n count_u8 = 0;
 *   endif
 * endif
 * 
 * stop
 * @enduml
 *
 * @note
 * - This function must be called periodically with a fixed time base
 *   consistent with @ref COUNT_500MS.
 * - The variable @c count_u8 is an internal debounce counter located in the
 *   implementation file (tempMon.c).
 *
 * @return None.
 */
void TempMon_Run(void);

#endif /* TEMPMON_H */
