
//-- unity: unit test framework
#include "unity.h"

//-- mocked modules
#include "TempMon_Run.h"
#include "mock_test_TempMon_RunAddFunc.h"


/**
 * External declaration of the internal debounce counter used by TempMon_Run.
 * This counter is managed by the function for debouncing transitions.
 */


void setUp(void)
{
    // Reset the debounce counter before each test
    count_u8 = 0u;
}

void tearDown(void)
{
}

/**
 * @brief Test: Temperature below threshold with no error state - counter resets
 * @details
 * When temperature is below upper threshold and error state is FALSE,
 * the counter should be reset to 0.
 */
void test_TempMon_Run_TempBelowThreshold_NoError_CounterReset(void)
{
    // Arrange
    sint16 temp_below = TEMPMON_OVERTEMP_UPTH - 10;  // 125°C (below 135°C threshold)
    count_u8 = 5u;  // Pre-set counter to non-zero value
    
    TempMeas_get_filtEcuTemp_s16_ExpectAndReturn(temp_below);
    EC_Get_error_overtemperature_State_e_ExpectAndReturn(EFS_FALSE);
    
    // Act
    TempMon_Run();
    
    // Assert
    TEST_ASSERT_EQUAL_UINT8(0u, count_u8);
}

/**
 * @brief Test: Temperature at upper threshold with no error state - counter increments
 * @details
 * When temperature reaches or exceeds upper threshold and error state is FALSE,
 * the counter should increment for debouncing.
 */
void test_TempMon_Run_TempAtUpperThreshold_NoError_CounterIncrement(void)
{
    // Arrange
    sint16 temp_at_threshold = TEMPMON_OVERTEMP_UPTH;  // 135°C
    count_u8 = 0u;
    
    TempMeas_get_filtEcuTemp_s16_ExpectAndReturn(temp_at_threshold);
    EC_Get_error_overtemperature_State_e_ExpectAndReturn(EFS_FALSE);
    
    // Act
    TempMon_Run();
    
    // Assert
    TEST_ASSERT_EQUAL_UINT8(1u, count_u8);
}

/**
 * @brief Test: Temperature above threshold with no error state - counter increments
 * @details
 * When temperature exceeds upper threshold and error state is FALSE,
 * the counter should increment for debouncing.
 */
void test_TempMon_Run_TempAboveUpperThreshold_NoError_CounterIncrement(void)
{
    // Arrange
    sint16 temp_above = TEMPMON_OVERTEMP_UPTH + 10;  // 145°C
    count_u8 = 2u;
    
    TempMeas_get_filtEcuTemp_s16_ExpectAndReturn(temp_above);
    EC_Get_error_overtemperature_State_e_ExpectAndReturn(EFS_FALSE);
    
    // Act
    TempMon_Run();
    
    // Assert
    TEST_ASSERT_EQUAL_UINT8(3u, count_u8);
}

/**
 * @brief Test: Counter reaches debounce limit at upper threshold - error state set to TRUE
 * @details
 * When counter exceeds COUNT_500MS at upper threshold,
 * error state should be set to TRUE and counter reset to 0.
 */
void test_TempMon_Run_TempAboveThreshold_CounterExceedsLimit_ErrorSetTrue(void)
{
    // Arrange
    sint16 temp_above = TEMPMON_OVERTEMP_UPTH + 5;  // 140°C
    count_u8 = COUNT_500MS;  // Counter at limit (5)
    
    TempMeas_get_filtEcuTemp_s16_ExpectAndReturn(temp_above);
    EC_Get_error_overtemperature_State_e_ExpectAndReturn(EFS_FALSE);
    EC_Set_error_overtemperature_State_e_Expect(EFS_TRUE);
    
    // Act
    TempMon_Run();
    
    // Assert
    TEST_ASSERT_EQUAL_UINT8(0u, count_u8);
}

/**
 * @brief Test: Temperature at upper threshold with error state TRUE - counter resets
 * @details
 * When temperature is at/above upper threshold and error state is already TRUE,
 * the counter should be reset to 0.
 */
void test_TempMon_Run_TempAtThreshold_ErrorTrue_CounterReset(void)
{
    // Arrange
    sint16 temp_above = TEMPMON_OVERTEMP_UPTH + 5;  // 140°C
    count_u8 = 3u;
    
    TempMeas_get_filtEcuTemp_s16_ExpectAndReturn(temp_above);
    EC_Get_error_overtemperature_State_e_ExpectAndReturn(EFS_TRUE);
    
    // Act
    TempMon_Run();
    
    // Assert
    TEST_ASSERT_EQUAL_UINT8(0u, count_u8);
}

/**
 * @brief Test: Temperature below threshold with error state TRUE - counter increments
 * @details
 * When temperature drops below upper threshold and error state is TRUE,
 * the counter should increment for debouncing the recovery.
 */
void test_TempMon_Run_TempBelowThreshold_ErrorTrue_CounterIncrement(void)
{
    // Arrange
    sint16 temp_below = TEMPMON_OVERTEMP_UPTH - 10;  // 125°C
    count_u8 = 1u;
    
    TempMeas_get_filtEcuTemp_s16_ExpectAndReturn(temp_below);
    EC_Get_error_overtemperature_State_e_ExpectAndReturn(EFS_TRUE);
    
    // Act
    TempMon_Run();
    
    // Assert
    TEST_ASSERT_EQUAL_UINT8(2u, count_u8);
}

/**
 * @brief Test: Counter reaches debounce limit below threshold - error state set to FALSE
 * @details
 * When counter exceeds COUNT_500MS while temperature is below threshold with error TRUE,
 * error state should be set to FALSE and counter reset to 0.
 */
void test_TempMon_Run_TempBelowThreshold_CounterExceedsLimit_ErrorSetFalse(void)
{
    // Arrange
    sint16 temp_below = TEMPMON_OVERTEMP_UPTH - 10;  // 125°C
    count_u8 = COUNT_500MS;  // Counter at limit (5)
    
    TempMeas_get_filtEcuTemp_s16_ExpectAndReturn(temp_below);
    EC_Get_error_overtemperature_State_e_ExpectAndReturn(EFS_TRUE);
    EC_Set_error_overtemperature_State_e_Expect(EFS_FALSE);
    
    // Act
    TempMon_Run();
    
    // Assert
    TEST_ASSERT_EQUAL_UINT8(0u, count_u8);
}

/**
 * @brief Test: Temperature below threshold with error state FALSE - counter remains zero
 * @details
 * When temperature is below threshold and error state is FALSE,
 * the counter should be reset and remain at 0.
 */
void test_TempMon_Run_TempBelowThreshold_ErrorFalse_CounterZero(void)
{
    // Arrange
    sint16 temp_below = TEMPMON_OVERTEMP_UPTH - 20;  // 115°C
    count_u8 = 4u;
    
    TempMeas_get_filtEcuTemp_s16_ExpectAndReturn(temp_below);
    EC_Get_error_overtemperature_State_e_ExpectAndReturn(EFS_FALSE);
    
    // Act
    TempMon_Run();
    
    // Assert
    TEST_ASSERT_EQUAL_UINT8(0u, count_u8);
}

/**
 * @brief Test: Multiple calls at threshold without reaching debounce limit
 * @details
 * Verify counter increments correctly over multiple calls but error state
 * is not set until COUNT_500MS is exceeded.
 */
void test_TempMon_Run_MultipleCallsAtThreshold_CounterIncrementsGradually(void)
{
    // Arrange
    sint16 temp_above = TEMPMON_OVERTEMP_UPTH + 3;  // 138°C
    
    // First call: counter at 0
    TempMeas_get_filtEcuTemp_s16_ExpectAndReturn(temp_above);
    EC_Get_error_overtemperature_State_e_ExpectAndReturn(EFS_FALSE);
    TempMon_Run();
    TEST_ASSERT_EQUAL_UINT8(1u, count_u8);
    
    // Second call: counter at 1
    TempMeas_get_filtEcuTemp_s16_ExpectAndReturn(temp_above);
    EC_Get_error_overtemperature_State_e_ExpectAndReturn(EFS_FALSE);
    TempMon_Run();
    TEST_ASSERT_EQUAL_UINT8(2u, count_u8);
    
    // Third call: counter at 2
    TempMeas_get_filtEcuTemp_s16_ExpectAndReturn(temp_above);
    EC_Get_error_overtemperature_State_e_ExpectAndReturn(EFS_FALSE);
    TempMon_Run();
    TEST_ASSERT_EQUAL_UINT8(3u, count_u8);
}

/**
 * @brief Test: Extreme high temperature - immediate error after debounce
 * @details
 * Verify behavior with very high temperature that remains stable,
 * leading to error state set after debounce period.
 */
void test_TempMon_Run_ExtremeHighTemp_ErrorSetAfterDebounce(void)
{
    // Arrange
    sint16 extreme_temp = 150;  // 150°C - well above threshold
    
    // Simulate 5 consecutive calls to exceed COUNT_500MS
    for (int i = 0; i < COUNT_500MS; i++)
    {
        TempMeas_get_filtEcuTemp_s16_ExpectAndReturn(extreme_temp);
        EC_Get_error_overtemperature_State_e_ExpectAndReturn(EFS_FALSE);
        TempMon_Run();
    }
    
    // On the 6th call (counter > COUNT_500MS), error should be set
    TempMeas_get_filtEcuTemp_s16_ExpectAndReturn(extreme_temp);
    EC_Get_error_overtemperature_State_e_ExpectAndReturn(EFS_FALSE);
    EC_Set_error_overtemperature_State_e_Expect(EFS_TRUE);
    TempMon_Run();
    
    // Assert
    TEST_ASSERT_EQUAL_UINT8(0u, count_u8);
}

/**
 * @brief Test: Hysteresis behavior - recovery after error set
 * @details
 * Verify that after error state is set, temperature must be held low
 * for COUNT_500MS to clear the error.
 */
void test_TempMon_Run_Hysteresis_RecoveryAfterError(void)
{
    // Arrange
    sint16 temp_above = TEMPMON_OVERTEMP_UPTH + 5;
    sint16 temp_below = TEMPMON_OVERTEMP_UPTH - 15;
    
    // Trigger error state by exceeding threshold
    for (int i = 0; i < COUNT_500MS + 1; i++)
    {
        if (i < COUNT_500MS)
        {
            TempMeas_get_filtEcuTemp_s16_ExpectAndReturn(temp_above);
            EC_Get_error_overtemperature_State_e_ExpectAndReturn(EFS_FALSE);
        }
        else
        {
            TempMeas_get_filtEcuTemp_s16_ExpectAndReturn(temp_above);
            EC_Get_error_overtemperature_State_e_ExpectAndReturn(EFS_FALSE);
            EC_Set_error_overtemperature_State_e_Expect(EFS_TRUE);
        }
        TempMon_Run();
    }
    
    // Now simulate recovery: temperature drops below threshold
    for (int i = 0; i < COUNT_500MS; i++)
    {
        TempMeas_get_filtEcuTemp_s16_ExpectAndReturn(temp_below);
        EC_Get_error_overtemperature_State_e_ExpectAndReturn(EFS_TRUE);
        TempMon_Run();
    }
    
    // Final call should clear error state
    TempMeas_get_filtEcuTemp_s16_ExpectAndReturn(temp_below);
    EC_Get_error_overtemperature_State_e_ExpectAndReturn(EFS_TRUE);
    EC_Set_error_overtemperature_State_e_Expect(EFS_FALSE);
    TempMon_Run();
    
    // Assert
    TEST_ASSERT_EQUAL_UINT8(0u, count_u8);
}

/**
 * @brief Test: Oscillating temperature near threshold
 * @details
 * Verify stability when temperature oscillates near the threshold.
 * Counter should reset when crossing above threshold to start debouncing fresh.
 */
void test_TempMon_Run_OscillatingTemp_CounterBehavior(void)
{
    // Arrange
    sint16 temp_above = TEMPMON_OVERTEMP_UPTH + 2;
    sint16 temp_below = TEMPMON_OVERTEMP_UPTH - 2;
    
    // First call: above threshold, counter increments
    TempMeas_get_filtEcuTemp_s16_ExpectAndReturn(temp_above);
    EC_Get_error_overtemperature_State_e_ExpectAndReturn(EFS_FALSE);
    TempMon_Run();
    TEST_ASSERT_EQUAL_UINT8(1u, count_u8);
    
    // Second call: below threshold, counter resets
    TempMeas_get_filtEcuTemp_s16_ExpectAndReturn(temp_below);
    EC_Get_error_overtemperature_State_e_ExpectAndReturn(EFS_FALSE);
    TempMon_Run();
    TEST_ASSERT_EQUAL_UINT8(0u, count_u8);
    
    // Third call: above threshold again, counter increments
    TempMeas_get_filtEcuTemp_s16_ExpectAndReturn(temp_above);
    EC_Get_error_overtemperature_State_e_ExpectAndReturn(EFS_FALSE);
    TempMon_Run();
    TEST_ASSERT_EQUAL_UINT8(1u, count_u8);
}
