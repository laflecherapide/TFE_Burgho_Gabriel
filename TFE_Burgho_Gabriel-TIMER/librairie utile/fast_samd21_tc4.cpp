#include "fast_samd21_tc4.h"

static void fast_samd21_tc4_reset() {
  TC4->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
  while (TC4->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY);
  while (TC4->COUNT16.CTRLA.bit.SWRST);
}

/*
  Starts the timer.
*/
void fast_samd21_tc4_start() {
  TC4->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;
  while (TC4->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY);
}

/*
  Pauses the timer.
*/
void fast_samd21_tc4_disable() {
  TC4->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
  while (TC4->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY);
}

/*
  Stops the timer. You should start with fast_samd21_tc4_configure again.
*/
void fast_samd21_tc4_stop() {
  fast_samd21_tc4_disable();
  fast_samd21_tc4_reset();
}

/*
  Configures the timer interrupt TC4 on SAMD21.

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
uint8_t fast_samd21_tc4_configure(double us) {
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

  // select clock generator TC4
  GCLK->CLKCTRL.reg = (uint16_t) (GCLK_CLKCTRL_CLKEN |
                                  GCLK_CLKCTRL_GEN_GCLK0 |
                                  GCLK_CLKCTRL_ID(GCM_TC4_TC5)) ;
  while (GCLK->STATUS.bit.SYNCBUSY);

  fast_samd21_tc4_reset();

  // set 16-bit mode and set waveform 'match frequency'
  TC4->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16 | TC_CTRLA_WAVEGEN_MFRQ;

  // prescaler
  switch (prescaler) {
  case 1:
    TC4->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1 | TC_CTRLA_ENABLE;
    break;
  case 2:
    TC4->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV2 | TC_CTRLA_ENABLE;
    break;
  case 4:
    TC4->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV4 | TC_CTRLA_ENABLE;
    break;
  case 8:
    TC4->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV8 | TC_CTRLA_ENABLE;
    break;
  case 16:
    TC4->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV16 | TC_CTRLA_ENABLE;
    break;
  case 64:
    TC4->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV64 | TC_CTRLA_ENABLE;
    break;
  case 256:
    TC4->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV256 | TC_CTRLA_ENABLE;
    break;
  case 1024:
    TC4->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1024 | TC_CTRLA_ENABLE;
    break;
  }

  // set compare register value
  TC4->COUNT16.CC[0].reg = compare_register;

  while (TC4->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY);

  NVIC_DisableIRQ(TC4_IRQn);
  NVIC_ClearPendingIRQ(TC4_IRQn);
  NVIC_SetPriority(TC4_IRQn, 0);
  NVIC_EnableIRQ(TC4_IRQn);

  // enable interrupt
  TC4->COUNT16.INTENSET.bit.MC0 = 1;
  while (TC4->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY);

  return 0;
}
