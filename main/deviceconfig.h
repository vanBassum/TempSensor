#pragma once
#include "esp_drivers.h"
#include "ST7796SAdapter.h"

void ConfigSPIBus(SPIBus& bus)
{
    bus.host = SPI2_HOST;
    bus.dmaChannel = 1;
    bus.config = {
        .mosi_io_num        = GPIO_NUM_13,
        .miso_io_num        = GPIO_NUM_NC,
        .sclk_io_num        = GPIO_NUM_14,
        .quadwp_io_num      = GPIO_NUM_NC,
        .quadhd_io_num      = GPIO_NUM_NC,
        .max_transfer_sz    = 40 * 320,
    };
}

void ConfigSPIDevice_ST7796S(SPIDevice& device)
{
    device.devConfig = {0};
    device.devConfig.clock_speed_hz     = 40 * 1000 * 1000; 
    device.devConfig.mode               = 0;
    device.devConfig.spics_io_num       = GPIO_NUM_15;
    device.devConfig.input_delay_ns     = 0;
    device.devConfig.queue_size         = 50;       /* maximum number of DMA transactions simultaneously in-flight */
    device.devConfig.pre_cb             = NULL;
    device.devConfig.post_cb            = NULL;
 
}

void Config_ST7796S(ST7796S& device)
{
    device.dc       = GPIO_NUM_21;
    device.rst      = GPIO_NUM_22;
    device.blck     = GPIO_NUM_23;
    device.hor_res  = 320;
    device.ver_res  = 480;
}

void Config_ST47796SAdapter(ST47796SAdapter& device)
{

}


