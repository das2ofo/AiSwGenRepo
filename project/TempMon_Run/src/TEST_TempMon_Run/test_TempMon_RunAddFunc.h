#include "TempMon_Run.h"

typedef enum {
    EFS_FALSE     = 0,  /**< No error.                 */
    EFS_TRUE      = 1,  /**< Overtemperature active.   */
    EFS_DEGRADED  = 2,  /**< Degraded condition.       */
    EFS_UNDEFINED = 3   /**< Undefined state.          */
} ErrorFlagState_t;

typedef struct ErrorFlag_s {
	ErrorFlagState_t state_e;
	uint8_t cnt_u8;
	uint8_t maxCnt_u8;
} ErrorFlag_t;
/**
 * @brief Diagnostic state for the overtemperature error.
 */



typedef struct {
    ErrorFlag_t error_overvoltage;		//ERROR
	ErrorFlag_t error_overtemperature;		//ERROR
} errorCollector_t;
/**
 * @brief Get filtered ECU temperature.
 *
 * @return Filtered ECU temperature in degrees Celsius [°C].
 */
sint16 TempMeas_get_filtEcuTemp_s16(void);

/*==============================================================*/
/*   OVERTEMPERATURE ERROR INTERFACE                            */
/*==============================================================*/

/**
 * @brief Get the current overtemperature diagnostic state.
 *
 * @return Current overtemperature state (see @ref ErrorFlagState_t).
 */
ErrorFlagState_t EC_Get_error_overtemperature_State_e(void);

/**
 * @brief Set the overtemperature diagnostic state.
 *
 * @param[in] new_state New diagnostic state value.
 */
void EC_Set_error_overtemperature_State_e(ErrorFlagState_t new_state);