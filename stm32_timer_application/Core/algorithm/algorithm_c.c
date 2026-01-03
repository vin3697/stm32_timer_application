/*
 * algorithm_c.c
 *
 *  Created on: 3 Jan 2026
 *      Author: VIN
 */




#include "gloabal_header_file.h"


// library functions
uint32_t percentage_calculation_u32(uint32_t min_val,
                                    uint32_t max_val,
                                    uint32_t actual_val)
{
    uint32_t percent_u32 = 0U;

    if (max_val > min_val)
    {
        uint32_t clamped_u32 = actual_val;

        if (clamped_u32 < min_val)
        {
            clamped_u32 = min_val;
        }
        else if (clamped_u32 > max_val)
        {
            clamped_u32 = max_val;
        }
        else
        {
            /* within range */
        }

        /* Range is guaranteed > 0 here */
        {
            uint32_t range_u32 = (max_val - min_val);
            uint32_t num_u32   = (clamped_u32 - min_val);

            /* Compute: round( num*100 / range )
             * Avoid overflow of num*100 using divide-first strategy.
             */
            uint32_t q_u32 = num_u32 / range_u32;      /* 0 or 1 after clamp */
            uint32_t r_u32 = num_u32 % range_u32;

            /* q_u32 is 0..1, so q_u32*100 is safe */
            percent_u32 = (q_u32 * 100U);

            /* Add fractional part with rounding */
            percent_u32 += (uint32_t)((r_u32 * 100U + (range_u32 / 2U)) / range_u32);
        }
    }
    else
    {
        /* invalid range: return 0 */
        percent_u32 = 0U;
    }

    /* Clamp to 100 just in case (should already be 0..100) */
    if (percent_u32 > 100U)
    {
        percent_u32 = 100U;
    }

    return percent_u32;
}
