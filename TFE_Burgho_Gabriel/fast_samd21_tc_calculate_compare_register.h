/*
  Author: Daniel Mohr
  Date: 2022-09-28

  This header file is used by 'fast_samd21_tc3.h', 'fast_samd21_tc4.h' and
  'fast_samd21_tc5.h'.

  This file calculates a compare register value and a prescaler for a 16-bit
  counter.
*/
#include <cstdint>

#define fast_samd21_tc_calculate_compare_register_h

static inline uint32_t
_fast_samd21_tc_calculate_compare_register(double,
                                           uint32_t);

  uint8_t fast_samd21_tc_calculate_compare_register(double,
                                                    uint16_t *,
                                                    uint16_t *);
