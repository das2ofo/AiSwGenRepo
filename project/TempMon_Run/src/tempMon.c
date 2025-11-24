#include <stdint.h>
#include <stdbool.h>
#include "tempMon.h"

/*==============================================================*/
/*   LOCAL STATE                                                 */
/*==============================================================*/
static uint8 count_u8 = 0u;




void TempMon_Run(void)
{
  sint16 l_tempEcu_s16 = 0;
  l_tempEcu_s16 = TempMeas_get_filtEcuTemp_s16();
  
  // Over Tempage
  if(l_tempEcu_s16 >= (sint16) TEMPMON_OVERTEMP_UPTH)
  {
    if(EC_Get_error_overtemperature_State_e() == EFS_TRUE)
    {
      // This reset command is mandatory
      count_u8 = 0u;
    }
    else
    {
      count_u8++;
      // Since we are starting to count at the detection, use > and not >=
      if(count_u8 > COUNT_500MS)
      {
        count_u8 = 0u;
        EC_Set_error_overtemperature_State_e(EFS_TRUE);
      }
    }
  }
  else
  {
    if(EC_Get_error_overtemperature_State_e() == EFS_TRUE)
    {
      // Since we are starting to count at the detection, use > and not >=
      count_u8++;
      
      if(count_u8 > COUNT_500MS)
      {
        count_u8 = 0u;
        EC_Set_error_overtemperature_State_e(EFS_FALSE);
      }
    }
    else
    {
      count_u8 = 0u;
    }
  }
}

int main()
{
  TempMon_Run();
  return 0;
}
