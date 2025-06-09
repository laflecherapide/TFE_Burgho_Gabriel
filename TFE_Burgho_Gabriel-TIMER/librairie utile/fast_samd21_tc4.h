/*
  Author: Daniel Mohr
  Date: 2023-09-28

  This header file allows using the TC4_Handler routine triggered by
  the TC4 timer on SAMD21 (e. g. Arduino MKR Zero).

  You have to provide the TC4_Handler routine, e. g.:

  void TC4_Handler(void) {
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  TC4->COUNT16.INTFLAG.bit.MC0 = 1; // clears the interrupt
  }

  For more information look at the README.md.

*/
#include <cstdint>

#include <stdbool.h>

#define fast_samd21_tc4_h

static void fast_samd21_tc4_reset();
void fast_samd21_tc4_start();
void fast_samd21_tc4_disable();
void fast_samd21_tc4_stop();
uint8_t fast_samd21_tc4_configure(double);


#if defined(fast_samd21_tc4_tc5_h)
  #error “TC4 counter can not be used together with TC4/TC5 32-bit counter.”
#endif

#if defined(ARDUINO_ARCH_SAMD)

#include "Arduino.h"

#include "fast_samd21_tc_calculate_compare_register.h"



#else
  #error “This library only supports boards with a SAMD processor.”

#endif /* fast_samd21_tc4_h */
