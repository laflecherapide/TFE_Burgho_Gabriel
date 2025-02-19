/*
config clock {
  To connect a Generic Clock Generator to a peripheral within the device, a Generic Clock Channel is used. Each
peripheral or peripheral group has an associated Generic Clock Channel, which serves as the clock input for
AT07627: ASF Manual (SAM D21) [APPLICATION NOTE]
42258A-SAMD21-04/2014
403
the peripheral(s). To supply a clock to the peripheral module(s), the associated channel must be connected to a
running Generic Clock Generator and the channel enabled.
A Basic Timer is a simple application where compare match operations is used to determine when a specific period
has elapsed. In Basic Timer operations, one or more values in the module's Compare/Capture registers are used
to specify the time (as a number of prescaled GCLK cycles) when an action should be taken by the microcontroller.
This can be an Interrupt Service Routine (ISR), event generator via the event system, or a software flag that is
polled via the user application.

}
config dmac {
  12 channels
  DMA transfer is based on descriptors, which holds
transfer properties such as the source and destination addresses, transfer counter and other additional transfer
control information. The descriptors can be static or linked. When static, a single block transfer is performed.
When linked, a number of transfer descriptors can be used to enable multiple block transfers within a single DMA
transaction.
The DMAC in each device consists of several DMA channels, which along with the transfer descriptors defines the
data transfer properties.
● The transfer control descriptor defines the source and destination addresses, source and destination address
increment settings, the block transfer count and event output condition selection.
● Dedicated channel registers control the peripheral trigger source, trigger mode settings, event input actions
and channel priority level settings.
With a successful DMA resource allocation, a dedicated DMA channel will be assigned. The channel will be
occupied until the DMA resource is freed. A DMA resource handle is used to identify the specific DMA resource.
When there are multiple channels with active requests, the arbiter prioritizes the channels requesting access to the
bus.
Both an interrupt callback and an peripheral event can be triggered by the DMA transfer. Three types of callbacks
are supported by the DMA driver: transfer complete, channel suspend and transfer error. Each of these callback
types can be registered and enabled for each channel independently through the DMA driver API.
The DMAC module can also generate events on transfer complete. Event generation is enabled through the DMA
channel, event channel configuration and event user multiplexing is done through the events driver.
The DMAC can generate events in the below cases:
● When a block transfer is complete
● When each beat transfer within a block transfer is complete

}
config clock dma
config DAC
*/



void configure_gclock_generator(void)//source AT07627: ASF Manual (SAM D21) [APPLICATION NOTE] 431
{
 struct system_gclk_gen_config gclock_gen_conf;
 system_gclk_gen_get_config_defaults(&gclock_gen_conf);
 gclock_gen_conf.source_clock = SYSTEM_CLOCK_SOURCE_OSC8M;
 gclock_gen_conf.division_factor = 128;
 system_gclk_gen_set_config(GCLK_GENERATOR_1, &gclock_gen_conf);
 system_gclk_gen_enable(GCLK_GENERATOR_1);
}
void configure_gclock_channel(void) //source AT07627: ASF Manual (SAM D21) [APPLICATION NOTE] 432
{
 struct system_gclk_chan_config gclk_chan_conf;
 system_gclk_chan_get_config_defaults(&gclk_chan_conf);
 gclk_chan_conf.source_generator = GCLK_GENERATOR_1;
 system_gclk_chan_set_config(TC3_GCLK_ID, &gclk_chan_conf);
 system_gclk_chan_enable(TC3_GCLK_ID);
}

struct tc_module tc_instance;//source AT07627: ASF Manual (SAM D21) [APPLICATION NOTE] 483
void tc_callback_to_toggle_led(
 struct tc_module *const module_inst)
{
 port_pin_toggle_output_level(LED0_PIN);
}
void configure_tc(void)
{
 struct tc_config config_tc;
 tc_get_config_defaults(&config_tc);
 config_tc.counter_size = TC_COUNTER_SIZE_8BIT;
 config_tc.clock_source = GCLK_GENERATOR_1;
 config_tc.clock_prescaler = TC_CLOCK_PRESCALER_DIV1024;
 config_tc.counter_8_bit.period = 100;
 config_tc.counter_8_bit.compare_capture_channel[0] = 50;
 config_tc.counter_8_bit.compare_capture_channel[1] = 54;
 tc_init(&tc_instance, TC3, &config_tc);
 tc_enable(&tc_instance);
}
void configure_tc_callbacks(void)
{
 tc_register_callback(&tc_instance, tc_callback_to_toggle_led,
 TC_CALLBACK_OVERFLOW);
 tc_register_callback(&tc_instance, tc_callback_to_toggle_led,
 TC_CALLBACK_CC_CHANNEL0);
 tc_register_callback(&tc_instance, tc_callback_to_toggle_led,
 TC_CALLBACK_CC_CHANNEL1);
 tc_enable_callback(&tc_instance, TC_CALLBACK_OVERFLOW);
 tc_enable_callback(&tc_instance, TC_CALLBACK_CC_CHANNEL0);
 tc_enable_callback(&tc_instance, TC_CALLBACK_CC_CHANNEL1);
}

void setup() {
  configure_gclock_generator();
configure_gclock_channel();
configure_tc();
configure_tc_callbacks();
}

void loop() {
}