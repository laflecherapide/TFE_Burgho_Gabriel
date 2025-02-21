//****************LIBRAIRIE*****************
typedef struct dac_module dac_module;
typedef struct adc_module adc_module;
typedef struct dma_resource dma_resource;

dac_module dac_instance;
adc_module adc_instance;
dma_resource example_resource;

//COMPILER_ALIGNED(16)
DmacDescriptor example_descriptor;

void configure_adc(void);
void configure_dac(void);
void configure_dac_channel(void);
void configure_dma_resource(struct dma_resource *resource);
void setup_transfer_descriptor(int *descriptor);

void adc_start_conversion(
 struct adc_module *const module_inst);

enum status_code dma_start_transfer_job(
 struct dma_resource * resource);

enum status_code dma_add_descriptor(
 struct dma_resource * resource,
 DmacDescriptor * descriptor);


