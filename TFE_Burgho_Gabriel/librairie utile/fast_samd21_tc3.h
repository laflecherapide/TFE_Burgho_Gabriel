/*
  Author: Daniel Mohr
  Date: 2023-09-28

  This header file allows using the TC3_Handler routine triggered by
  the TC3 timer on SAMD21 (e. g. Arduino MKRZERO).

  You have to provide the TC3_Handler routine, e. g.:

  void TC3_Handler(void) {
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  TC3->COUNT16.INTFLAG.bit.MC0 = 1; // clears the interrupt
  }

  For more information look at the README.md.

*/
#include <cstdint>

#include <stdbool.h>

#define fast_samd21_tc3_h

void fast_samd21_tc3_start();
void fast_samd21_tc3_stop();
static void fast_samd21_tc3_reset();
void fast_samd21_tc3_disable();
uint8_t fast_samd21_tc3_configure(double);

#if defined(ARDUINO_ARCH_SAMD)

#include "Arduino.h"

#include "fast_samd21_tc_calculate_compare_register.h"







#else
  #error “This library only supports boards with a SAMD processor.”

#endif /* fast_samd21_tc3_h */
