//****************LIBRAIRIE*****************
#include "talkie_walkie.h"

void configure_adc(void)
{
 struct adc_config config_adc;
 adc_get_config_defaults(&config_adc);
 config_adc.gain_factor = ADC_GAIN_FACTOR_DIV2;
 config_adc.clock_prescaler = ADC_CLOCK_PRESCALER_DIV16;
 config_adc.reference = ADC_REFERENCE_INTVCC1;
 config_adc.positive_input = ADC_POSITIVE_INPUT_PIN4; //PA4 = A3
 config_adc.resolution = ADC_RESOLUTION_10BIT;
 config_adc.freerunning = true;
 config_adc.left_adjust = false;
 adc_init(&adc_instance, ADC, &config_adc);
 adc_enable(&adc_instance);
}
void configure_dac(void)
{
 struct dac_config config_dac;
 dac_get_config_defaults(&config_dac);
 config_dac.reference = DAC_REFERENCE_AVCC;
 dac_init(&dac_instance, DAC, &config_dac);
 dac_enable(&dac_instance);
}
void configure_dac_channel(void)
{
 struct dac_chan_config config_dac_chan;
 dac_chan_get_config_defaults(&config_dac_chan);
 dac_chan_set_config(&dac_instance, DAC_CHANNEL_0, &config_dac_chan);
 dac_chan_enable(&dac_instance, DAC_CHANNEL_0);
}
void configure_dma_resource(struct dma_resource *resource)
{
 struct dma_resource_config config;
 dma_get_config_defaults(&config);
 config.peripheral_trigger = ADC_DMAC_ID_RESRDY;
 config.trigger_action = DMA_TRIGGER_ACTON_BEAT;
 dma_allocate(resource, &config);
}
void setup_transfer_descriptor(DmacDescriptor *descriptor)
{
 struct dma_descriptor_config descriptor_config;
 dma_descriptor_get_config_defaults(&descriptor_config);
 descriptor_config.beat_size = DMA_BEAT_SIZE_HWORD;
 descriptor_config.dst_increment_enable = false;
 descriptor_config.src_increment_enable = false;
 descriptor_config.block_transfer_count = 1000;
 descriptor_config.source_address = (uint32_t)(&adc_instance.hw->RESULT.reg);
 descriptor_config.destination_address = (uint32_t)(&dac_instance.hw->DATA.reg);
 descriptor_config.next_descriptor_address = (uint32_t)descriptor;
 dma_descriptor_create(descriptor, &descriptor_config);
}

