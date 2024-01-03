/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_log.h"
#include "servicelocator.h"
#include "esp_drivers.h"

#define LV_HOR_RES_MAX 320
#define LV_VER_RES_MAX 480
#define SPI_HOST_MAX 3
#define DISP_SPI_MISO -1
#define DISP_SPI_MOSI 13
#define DISP_SPI_CLK 14
#define DISP_PIN_BCKL GPIO_NUM_23
#define DISP_SPI_CS 15
#define DISP_SPI_IO2 -1
#define DISP_SPI_IO3 -1
#define DISP_BUF_SIZE  (LV_HOR_RES_MAX * 40)
#define SPI_BUS_MAX_TRANSFER_SZ (DISP_BUF_SIZE * 2)
#define SPI_TFT_CLOCK_SPEED_HZ  (40*1000*1000)
#define SPI_TFT_SPI_MODE    (0)
#define DISP_SPI_INPUT_DELAY_NS (0)
#define SPI_TRANSACTION_POOL_SIZE 50	/* maximum number of DMA transactions simultaneously in-flight */
/* DMA Transactions to reserve before queueing additional DMA transactions. A 1/10th seems to be a good balance. Too many (or all) and it will increase latency. */
#define SPI_TRANSACTION_POOL_RESERVE_PERCENTAGE 10
#define SPI_TRANSACTION_POOL_RESERVE (SPI_TRANSACTION_POOL_SIZE / SPI_TRANSACTION_POOL_RESERVE_PERCENTAGE)	

void ConfigSPIBus(SPIBus& bus)
{
    bus.host = SPI2_HOST;
    bus.dmaChannel = 1;
    bus.config = {
        .mosi_io_num = DISP_SPI_MOSI,
        .miso_io_num = DISP_SPI_MISO,
        .sclk_io_num = DISP_SPI_CLK,
        .quadwp_io_num = DISP_SPI_IO2,
        .quadhd_io_num = DISP_SPI_IO3,
        .max_transfer_sz = SPI_BUS_MAX_TRANSFER_SZ
    };
}

void ConfigSPIDevice_ST7796S(SPIDevice& device)
{
    device.devConfig = {0};
    device.devConfig.clock_speed_hz = SPI_TFT_CLOCK_SPEED_HZ   ; 
    device.devConfig.mode = SPI_TFT_SPI_MODE;
    device.devConfig.spics_io_num=DISP_SPI_CS            ;
    device.devConfig.input_delay_ns=DISP_SPI_INPUT_DELAY_NS;
    device.devConfig.queue_size=SPI_TRANSACTION_POOL_SIZE;
    device.devConfig.pre_cb=NULL;
    device.devConfig.post_cb=NULL;
 
}

void Config_ST7796S(ST7796S& device)
{
    device.dc=GPIO_NUM_21;
    device.rst=GPIO_NUM_22;
    device.blck = DISP_PIN_BCKL;
}

uint16_t colors[10*10];

extern "C" void app_main(void)
{
    ESP_LOGI("MAIN", "Started");

    Builder builder;
    builder.Services.addService<SPIBus>(
        ServiceTags::SPIBUS, 
        ConfigSPIBus);

    builder.Services.addService<SPIDevice>(
        ServiceTags::SPIDevice_ST7796S, 
        builder.Services.getService<SPIBus>(ServiceTags::SPIBUS), 
        ConfigSPIDevice_ST7796S);

    builder.Services.addService<ST7796S>(
        ServiceTags::DRIVER_ST7796S, 
        builder.Services.getService<SPIDevice>(ServiceTags::SPIDevice_ST7796S), 
        Config_ST7796S);

    auto driver = builder.Services.getService<ST7796S>(ServiceTags::DRIVER_ST7796S);

    driver->st7796s_init();
//driver->DrawPixel(20, 20, 0xFFFF);
    
    driver->SetWindow(0, 0, 10, 10);
    driver->WriteWindow(colors, 10*10);

    //driver->DrawPixel(10, 10, 0x0000);
    //driver->DrawPixel(20, 20, 0xFFFF);

    while(1)
        vTaskDelay(pdMS_TO_TICKS(1000));
}
