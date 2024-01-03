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

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 480

// Helper function to convert HSV to RGB
void HSVtoRGB(float h, float s, float v, uint8_t& r, uint8_t& g, uint8_t& b) {
    int i;
    float f, p, q, t;

    if (s == 0) {
        r = g = b = static_cast<uint8_t>(v * 255);
        return;
    }

    h /= 60; // sector 0 to 5
    i = static_cast<int>(h);
    f = h - i; // factorial part of h
    p = v * (1 - s);
    q = v * (1 - s * f);
    t = v * (1 - s * (1 - f));

    switch (i) {
    case 0:
        r = static_cast<uint8_t>(v * 255);
        g = static_cast<uint8_t>(t * 255);
        b = static_cast<uint8_t>(p * 255);
        break;
    case 1:
        r = static_cast<uint8_t>(q * 255);
        g = static_cast<uint8_t>(v * 255);
        b = static_cast<uint8_t>(p * 255);
        break;
    case 2:
        r = static_cast<uint8_t>(p * 255);
        g = static_cast<uint8_t>(v * 255);
        b = static_cast<uint8_t>(t * 255);
        break;
    case 3:
        r = static_cast<uint8_t>(p * 255);
        g = static_cast<uint8_t>(q * 255);
        b = static_cast<uint8_t>(v * 255);
        break;
    case 4:
        r = static_cast<uint8_t>(t * 255);
        g = static_cast<uint8_t>(p * 255);
        b = static_cast<uint8_t>(v * 255);
        break;
    default: // case 5:
        r = static_cast<uint8_t>(v * 255);
        g = static_cast<uint8_t>(p * 255);
        b = static_cast<uint8_t>(q * 255);
        break;
    }
}

void draw(std::shared_ptr<ST7796S> driver) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        // Calculate hue based on the position from left to right
        float hue = (x * 360.0) / SCREEN_WIDTH;

        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            // Calculate brightness based on the position from top to bottom
            float brightness = static_cast<float>(y) / SCREEN_HEIGHT;

            // Convert HSV to RGB
            uint8_t r, g, b;
            HSVtoRGB(hue, 1.0, brightness, r, g, b);

            // Convert RGB to 5-6-5 bit format used by many displays
            uint16_t color = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);

            // Draw a pixel at the current position with the calculated color
            driver->DrawPixel(x, y, color);
        }
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}


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



    // Some testing
    auto driver = builder.Services.getService<ST7796S>(ServiceTags::DRIVER_ST7796S);

    driver->st7796s_init();

    draw(driver);


    while(1)
        vTaskDelay(pdMS_TO_TICKS(1000));
}
