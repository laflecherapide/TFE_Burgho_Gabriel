/*
config clock {
  To connect a Generic Clock Generator to a peripheral within the device, a Generic Clock Channel is used. Each
peripheral or peripheral group has an associated Generic Clock Channel, which serves as the clock input for
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

//****************LIBRAIRIE*****************
#include "talkie_walkie.h"

void setup() {
  pinMode(PIN_ADC, INPUT);
  analogWriteResolution(10);//resolution de 10 bits la même que l'ADC
}

void loop() {
  if (buffer[100] != 0) //si le buffer est vide on échantillonne
  {
    for (int i = 0; i<buffer_size;i++)//remplit le buffer
  {
    buffer[i] = analogRead(PIN_ADC);
  }
  }
  for (int i = 0; i<buffer_size;i++)
  {
    analogWrite(PIN_DAC, buffer[i]);
    buffer[i] = 0;//vide le buffer aprés lecture
  }   
}