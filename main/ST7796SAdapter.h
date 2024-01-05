#pragma once
#include <functional>
#include "esp_lvgl.h"
#include "st7796s.h"
#include "lvgl/src/display/lv_display_private.h"
#include "kernel.h"

class ST47796SAdapter : public ESP_LVGL::Display
{


































	const char* TAG = "ST7796SAdapter";
    std::shared_ptr<ST7796S> driver;
	size_t bufferSize = 0;
	uint8_t* disp_buf;

	void Flush_cb(struct _lv_display_t * disp, const lv_area_t * area, uint8_t * px_map)
	{
        driver->SetWindow(area->x1, area->y1, area->x2, area->y2);
        size_t size = (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1);
        driver->WriteWindow((uint16_t*)px_map, size);
	    lv_display_flush_ready(disp);
	}
	
	static void StaticFlush_cb(struct _lv_display_t * disp, const lv_area_t * area, uint8_t * px_map)
	{
		ST47796SAdapter* instance = (ST47796SAdapter*)disp->user_data;
		instance->Flush_cb(disp, area, px_map);
	}

public:
    ST47796SAdapter(std::shared_ptr<ST7796S> driver)
        : driver(driver)
    {
        lv_coord_t width = driver->getWidth();
		lv_coord_t height = driver->getHeight();

        //One buffer for partial rendering
        bufferSize = width * height / 20;
        disp_buf = (uint8_t*)malloc(bufferSize);
        if (disp_buf == NULL)
            return;

        ESP_LVGL::LVGL::ExecuteSafely([&]() {
            //Create a display and set a flush_cb
            handle = lv_display_create(width, height);
            lv_display_set_flush_cb(handle, StaticFlush_cb);
            lv_display_set_draw_buffers(handle, disp_buf, NULL, bufferSize, LV_DISPLAY_RENDER_MODE_PARTIAL);
        });

        handle->user_data = this;
    }

public:
	~ST47796SAdapter()
	{
		free(disp_buf);
	}
};
