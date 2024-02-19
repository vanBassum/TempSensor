/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#define CONFIG_LV_COLOR_16_SWAP 1
//#define CONFIG_LV_THEME_DEFAULT_DARK 1
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
#include "deviceconfig.h"
#include <math.h>




Builder builder;

extern "C" void app_main(void)
{
    ESP_LOGI("MAIN", "Started");


    std::shared_ptr<DriverRegistry> driverRegistery = std::make_shared<DriverRegistry>();
    driverRegistery->RegisterDriver<SpiBus>("espSpiBus");
    driverRegistery->RegisterDriver<SpiDevice>("espSpiDevice");


    std::shared_ptr<DeviceManager> deviceManager = std::make_shared<DeviceManager>(driverRegistery);
    deviceManager->init();
    deviceManager->RegisterDetector(std::make_shared<DeviceTreeDetector>(deviceTree)); 

    

    uint32_t i = 0;
    while (1)
    {



        vTaskDelay(500);
        i++;
    }
    /*
    builder.Services.addService<SPIBus>().Config(ConfigSPIBus);
    builder.Services.addService<SPIDevice>(builder.Services.getService<SPIBus>()).Config(ConfigSPIDevice);
    builder.Services.addService<ST7796S>(builder.Services.getService<SPIDevice>()).Config(ConfigST7796S);
    builder.Services.addService<ESP_LVGL::LVGLService>().Config(ConfigLVGL);
    builder.Services.addService<ESP_LVGL::Display, ST47796SAdapter>(builder.Services.getService<ESP_LVGL::LVGLService>(), builder.Services.getService<ST7796S>()).Config(ConfigST47796SAdapter);


    auto lvgl = builder.Services.getService<ESP_LVGL::LVGLService>();
    auto display = builder.Services.getService<ESP_LVGL::Display>();

    assert(display);

    ESP_LVGL::Screen screen(lvgl);
    display->showScreen(screen);

    ESP_LVGL::Button button(screen);

    button.SetSize(200, 50);

    ESP_LVGL::Label label(screen);

    label.SetPosition(10, 300);
    label.SetText("Hello world");

    while(1)
        vTaskDelay(pdMS_TO_TICKS(1000));
    */
}

