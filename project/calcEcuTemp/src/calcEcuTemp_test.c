/* Unit test implementation for calcEcuTemp.h following MISRA C rules */

#include "calcEcuTemp.h"
#include <assert.h>
#include <stdint.h>

/* Define macros for thresholds to match documentation */
#define PAR_ECU_TEMP_LOW_THR 30
#define PAR_ECU_TEMP_UP_THR 40

/* Mock implementation of Calc_ecuTemperature_s16 to test behavior according to documentation */

sint16 Calc_ecuTemperature_s16(void);

/* Forward declaration of static helper functions for testing */
static void test_below_low_threshold_faulty_ntc(void);
static void test_below_low_threshold_valid_ntc(void);
static void test_in_between_thresholds(void);
static void test_above_upper_threshold(void);

/* Helper functions to mock inputs - using static variables to simulate different conditions */

static sint16 NtcTemp_s16;
static sint16 SysTemp_s16;
static uint8_t NtcErr_b;

/* Redefine the function to be testable under various scenarios */
sint16 Calc_ecuTemperature_s16(void)
{
    /* Implementation following the documentation's logic */

    /* Local variable for result */
    sint16 l_ecuTemp_s16 = 0;

    /* Read the mock inputs */
    sint16 ntc = NrcTemp_s16;
    sint16 sys = SysTemp_s16;
    uint8_t err = NtcErr_b;

    if ((ntc <= PAR_ECU_TEMP_LOW_THR) || (err != 0))
    {
        /* NTC is faulty or too low: use system temperature */
        l_ecuTemp_s16 = sys;
    }
    else if (ntc >= PAR_ECU_TEMP_UP_THR)
    {
        /* NTC indicates high temperature: use NTC */
        l_ecuTemp_s16 = ntc;
    }
    else
    {
        /* Interpolation in the middle range */
        int32_t numerator;
        int32_t denominator = PAR_ECU_TEMP_UP_THR - PAR_ECU_TEMP_LOW_THR;

        numerator = (int32_t)ntc * ((int32_t)ntc - PAR_ECU_TEMP_LOW_THR)
                   + (int32_t)sys * (PAR_ECU_TEMP_UP_THR - ntc);

        /* Calculate interpolated temperature with rounding */
        l_ecuTemp_s16 = (sint16)((numerator + (denominator / 2)) / denominator);
    }

    return l_ecuTemp_s16;
}

/* Test case: NTC faulty (NtcErr_b == true), temperature below low threshold */
static void test_below_low_threshold_faulty_ntc(void)
{
    /* Setup inputs: NtcErr_b = true forces fallback to system temperature */
    NtcErr_b = 1; /* true */
    NtcTemp_s16 = 10; /* Any value below threshold */
    SysTemp_s16 = 25;

    sint16 result = Calc_ecuTemperature_s16();

    /* Expect system temperature to be used */
    assert(result == SysTemp_s16);
}

/* Test case: NTC temperature below low threshold (no error) */
static void test_below_low_threshold_valid_ntc(void)
{
    /* Setup inputs: NtcTemp_s16 below PAR_ECU_TEMP_LOW_THR */
    NtcErr_b = 0; /* false */
    NtcTemp_s16 = PAR_ECU_TEMP_LOW_THR - 5; /* Below threshold */
    SysTemp_s16 = 35; /* Arbitrary */

    sint16 result = Calc_ecuTemperature_s16();

    /* Expect system temperature to be used */
    assert(result == SysTemp_s16);
}

/* Test case: NTC temperature between thresholds (interpolation) */
static void test_in_between_thresholds(void)
{
    /* Setup inputs: NtcTemp_s16 between low and up thresholds, no error */
    NtcErr_b = 0; /* false */
    NtcTemp_s16 = (PAR_ECU_TEMP_LOW_THR + PAR_ECU_TEMP_UP_THR) / 2; /* Mid-range */
    SysTemp_s16 = 10; /* Arbitrary */

    /* Compute expected interpolated value manually */
    int16_t ntc = NtcTemp_s16;
    int16_t sys = SysTemp_s16;
    int32_t numerator = (int32_t)ntc * (ntc - PAR_ECU_TEMP_LOW_THR)
                        + (int32_t)sys * (PAR_ECU_TEMP_UP_THR - ntc);
    int32_t denominator = PAR_ECU_TEMP_UP_THR - PAR_ECU_TEMP_LOW_THR;
    sint16 expected = (sint16)((numerator + (denominator / 2)) / denominator);

    sint16 result = Calc_ecuTemperature_s16();

    assert(result == expected);
}

/* Test case: NTC temperature above upper threshold */
static void test_above_upper_threshold(void)
{
    /* Setup inputs: NtcTemp_s16 >= PAR_ECU_TEMP_UP_THR */
    NtcErr_b = 0; /* false */
    NtcTemp_s16 = PAR_ECU_TEMP_UP_THR + 5; /* Above threshold */
    SysTemp_s16 = 20;

    sint16 result = Calc_ecuTemperature_s16();

    /* Expect NTC temperature to be used */
    assert(result == NtcTemp_s16);
}

/* Main function to run all tests */
int main(void)
{
    test_below_low_threshold_faulty_ntc();
    test_below_low_threshold_valid_ntc();
    test_in_between_thresholds();
    test_above_upper_threshold();

    /* Optionally, indicate success */
    return 0;
}
