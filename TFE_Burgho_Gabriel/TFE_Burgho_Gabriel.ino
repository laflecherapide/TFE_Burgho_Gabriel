/*
config clock
config dma
config clock dma
config DAC
*/
#include <dac_feature.h>


static void system_gclk_chan_get_config_defaults(struct system_gclk_chan_config *const config);
//void dac_get_config_defaults(struct );

void setup() {
  dac_get_config_default();
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
