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



/*void configure_gclock_generator(void)//source AT07627: ASF Manual (SAM D21) [APPLICATION NOTE] 431
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
}*/
#include "Adafruit_NeoMatrix_ZeroDMA.h"
//source AT07627: ASF Manual (SAM D21) [APPLICATION NOTE] 489
#define PWM_MODULE EXT1_PWM_MODULE
#define PWM_OUT_PIN EXT1_PWM_0_PIN
#define PWM_OUT_MUX EXT1_PWM_0_MUX
struct tc_module tc_instance;
struct dma_resource example_resource;
#define TRANSFER_SIZE (16)
#define TRANSFER_COUNTER (64)
static uint8_t source_memory[TRANSFER_SIZE*TRANSFER_COUNTER];
static uint8_t destination_memory[TRANSFER_SIZE*TRANSFER_COUNTER];
static volatile bool transfer_is_done = false;
COMPILER_ALIGNED(16)
DmacDescriptor example_descriptor;

#define TRANSFER_SIZE (16)
#define TRANSFER_COUNTER (64)
static uint8_t source_memory[TRANSFER_SIZE*TRANSFER_COUNTER];
static uint8_t destination_memory[TRANSFER_SIZE*TRANSFER_COUNTER];
static volatile bool transfer_is_done = false;
COMPILER_ALIGNED(16)
DmacDescriptor example_descriptor;
void configure_tc(void)
{
 struct tc_config config_tc;
 tc_get_config_defaults(&config_tc);
 config_tc.counter_size = TC_COUNTER_SIZE_16BIT;
 config_tc.wave_generation = TC_WAVE_GENERATION_NORMAL_PWM;
 config_tc.counter_16_bit.compare_capture_channel[0] = (0xFFFF / 4);
 config_tc.pwm_channel[0].enabled = true;
 config_tc.pwm_channel[0].pin_out = PWM_OUT_PIN;
 config_tc.pwm_channel[0].pin_mux = PWM_OUT_MUX;
 tc_init(&tc_instance, PWM_MODULE, &config_tc);
 tc_enable(&tc_instance);
}
void transfer_done( const struct dma_resource* const resource )
{
 UNUSED(resource);
 transfer_is_done = true;
}
void configure_dma_resource(struct dma_resource *resource)
{
 struct dma_resource_config config;
 dma_get_config_defaults(&config);
 config.peripheral_trigger = TC6_DMAC_ID_MC_0;
 dma_allocate(resource, &config);
}
void setup_dma_descriptor(DmacDescriptor *descriptor)
{
 struct dma_descriptor_config descriptor_config;
 dma_descriptor_get_config_defaults(&descriptor_config);
 descriptor_config.block_transfer_count = TRANSFER_SIZE;
 descriptor_config.source_address = (uint32_t)source_memory + TRANSFER_SIZE;
 descriptor_config.destination_address = (uint32_t)destination_memory + TRANSFER_SIZE;
 dma_descriptor_create(descriptor, &descriptor_config);
}

void setup() {
  configure_tc();

}

void loop() {
  for(i=0;i<TRANSFER_COUNTER;i++) {
 transfer_is_done = false;
 dma_start_transfer_job(&example_resource);
 while (!transfer_is_done) {
 /* Wait for transfer done */
 }
 example_descriptor.SRCADDR.reg += TRANSFER_SIZE;
 example_descriptor.DSTADDR.reg += TRANSFER_SIZE;
}
while(1);
}