#include "fast_samd21_tc3.h"

/*
  Starts the timer.
*/
void fast_samd21_tc3_start() {
  TC3->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;
  while (TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY);
}

/*
  Stops the timer. You should start with fast_samd21_tc3_configure again.
*/
void fast_samd21_tc3_stop() {
  fast_samd21_tc3_disable();
  fast_samd21_tc3_reset();
}

static void fast_samd21_tc3_reset() {
  TC3->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
  while (TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY);
  while (TC3->COUNT16.CTRLA.bit.SWRST);
}



/*
  Pauses the timer.
*/
void fast_samd21_tc3_disable() {
  TC3->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
  while (TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY);
}

/*
  Configures the timer interrupt TC3 on SAMD21.

  The parameter us is the time in 1e-6 s (= 1 us).

  us has to be less than or equal to 1398111: 0 << us <= 1398111
  Due to timer resolution you will not get 1398111 but 1398101.

  You can just reconfigure by call this routine again.

  Returns an error code:

  0: no error
  1: us == 0 and this is too small
  2: us > 1398111 and this is too large
  3: no combination of prescaler and compare register value found
  4: us < 0, but us cannot be negative
*/
uint8_t fast_samd21_tc3_configure(double us) {
  if (((uint32_t) us) == 0)
    return 1;
  if (us > 1398111)
    return 2;
  if (us < 0)
    return 4;
  // find prescaler and compare register value
  // try TC_CTRLA_PRESCALER_DIV1
  uint16_t prescaler;
  uint16_t compare_register;
  if (fast_samd21_tc_calculate_compare_register(us,
						&prescaler,
						&compare_register) == 3) {
    return 3;
  }


  // set compare register value
  TC3->COUNT16.CC[0].reg = compare_register;

  while (TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY);

  NVIC_DisableIRQ(TC3_IRQn);
  NVIC_ClearPendingIRQ(TC3_IRQn);
  NVIC_SetPriority(TC3_IRQn, 0);
  NVIC_EnableIRQ(TC3_IRQn);

  // enable interrupt
  TC3->COUNT16.INTENSET.bit.MC0 = 1;
  while (TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY);

  return 0;
}