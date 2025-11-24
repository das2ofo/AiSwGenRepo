#include <stdint.h>
#include <stdbool.h>
#include "currDer.h"

/*==============================================================*/
/*   LOCAL STATE                                                 */
/*==============================================================*/

static sint16 IdcLim_s16;
static sint16 IlineLim_s16;


/*==============================================================*/
/*   DERATING TABLE DEFINITIONS                                 */
/*   (spostate qui dal .h per evitare multiple definitions)     */
/*==============================================================*/

const CurrentMon_mathApplPoint_t CurrentMon_IdcVdcTab[CURRENTMON_IDC_VDC_N_POINTS] = 
{
  //       dV                           MOTCTRL_RES = 2^7
  { .x_s16 =  1000, .y_s16 = (  5 * CURRENTMON_AMPS2SW_RES ) },
  { .x_s16 =  8500, .y_s16 = (  5 * CURRENTMON_AMPS2SW_RES ) },
  { .x_s16 = 10000, .y_s16 = ( 20 * CURRENTMON_AMPS2SW_RES ) },
  { .x_s16 = 13000, .y_s16 = ( 27 * CURRENTMON_AMPS2SW_RES ) },
  { .x_s16 = 16000, .y_s16 = ( 27 * CURRENTMON_AMPS2SW_RES ) },
  { .x_s16 = 16500, .y_s16 = ( 26 * CURRENTMON_AMPS2SW_RES ) },
  { .x_s16 = 18000, .y_s16 = ( 26 * CURRENTMON_AMPS2SW_RES ) }
};

/** \brief Idc vs T ECU derating curve */
const CurrentMon_mathApplPoint_t CurrentMon_IdcTecuTab[CURRENTMON_IDC_TECU_N_POINTS] = 
{
  //       deg                          MOTCTRL_RES = 2^7
  { .x_s16 =   -50, .y_s16 = ( 27 * CURRENTMON_AMPS2SW_RES ) },
  { .x_s16 =    60, .y_s16 = ( 27 * CURRENTMON_AMPS2SW_RES ) },
  { .x_s16 =   110, .y_s16 = ( 20 * CURRENTMON_AMPS2SW_RES ) },
  { .x_s16 =   130, .y_s16 = ( 13 * CURRENTMON_AMPS2SW_RES ) },
  { .x_s16 =   145, .y_s16 = (  0 * CURRENTMON_AMPS2SW_RES ) }
};

/** \brief I line vs T ECU derating curve */
const CurrentMon_mathApplPoint_t CurrentMon_IlineTecuTab[CURRENTMON_ILINE_TECU_N_POINTS] = 
{
  //       deg                          MOTCTRL_RES = 2^7
  { .x_s16 =   -50, .y_s16 = ( 69 * CURRENTMON_AMPS2SW_RES ) },
  { .x_s16 =    0 , .y_s16 = ( 69 * CURRENTMON_AMPS2SW_RES ) },
  { .x_s16 =    40, .y_s16 = ( 63 * CURRENTMON_AMPS2SW_RES ) },
  { .x_s16 =   110, .y_s16 = ( 48 * CURRENTMON_AMPS2SW_RES ) },
  { .x_s16 =   140, .y_s16 = (  5 * CURRENTMON_AMPS2SW_RES ) },
  { .x_s16 =   145, .y_s16 = (  0 * CURRENTMON_AMPS2SW_RES ) }
};

/*==============================================================*/
/*   CURRENT MONITOR DERATING                                   */
/*==============================================================*/

void CurrentMon_Derating(void)
{
  sint16 l_supplyVoltage_mv_u16 = 0;
  sint16 l_ecuTemp_deg_s16      = 0;
  sint16 l_limitIdcVdc_s16      = 0;
  sint16 l_limitIdctemp_s16     = 0;

  // Read inputs
  l_supplyVoltage_mv_u16 = (sint16)VoltMeas_get_vbat_u16();
  l_ecuTemp_deg_s16      = TempMeas_get_filtEcuTemp_s16();

  // Evaluate limits
  l_limitIdcVdc_s16 = CurrentMon_SetDeratingLimits_s16(l_supplyVoltage_mv_u16,
                                                        CurrentMon_IdcVdcTab,
                                                        CURRENTMON_IDC_VDC_N_POINTS);

  l_limitIdctemp_s16 = CurrentMon_SetDeratingLimits_s16(l_ecuTemp_deg_s16,
                                                         CurrentMon_IdcTecuTab,
                                                         CURRENTMON_IDC_TECU_N_POINTS);

  // Set output interfaces
  IlineLim_s16 = CurrentMon_SetDeratingLimits_s16(l_ecuTemp_deg_s16,
                                                   CurrentMon_IlineTecuTab,
                                                   CURRENTMON_ILINE_TECU_N_POINTS);

  IdcLim_s16   = CURRENTMON_MIN(l_limitIdcVdc_s16, l_limitIdctemp_s16);
}

int main()
{
  CurrentMon_Derating();
  return 0;
}
