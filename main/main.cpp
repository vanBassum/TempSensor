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
#include "deviceconfig.h"
#include <math.h>

#define LV_THEME_DEFAULT_DARK 1


Builder builder;

extern "C" void app_main(void)
{
    ESP_LOGI("MAIN", "Started");

    builder.Services.addService<SPIBus>().Config(ConfigSPIBus);
    builder.Services.addService<SPIDevice>(builder.Services.getService<SPIBus>()).Config(ConfigSPIDevice);
    builder.Services.addService<ST7796S>(builder.Services.getService<SPIDevice>()).Config(ConfigST7796S);
    builder.Services.addService<ESP_LVGL::Display, ST47796SAdapter>(builder.Services.getService<ST7796S>());


    auto display = builder.Services.getService<ESP_LVGL::Display>();

    assert(display);
    ESP_LVGL::Screen& screen = display->GetScreen();
    ESP_LVGL::Button button(screen);
    button.SetSize(200, 50);


    while(1)
        vTaskDelay(pdMS_TO_TICKS(1000));
}

