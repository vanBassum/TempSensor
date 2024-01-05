#pragma once
#include <functional>
#include "esp_lvgl.h"
#include "st7796s.h"
#include "lvgl/src/display/lv_display_private.h"
#include "kernel.h"

class ST47796SAdapter : public ESP_LVGL::Display
{
	constexpr static const char* TAG = "ST7796SAdapter";
    std::shared_ptr<LVGL::LVGLService> lvgl;
    std::shared_ptr<ST7796S> driver;

protected:
	void Flush_cb(struct _lv_display_t * disp, const lv_area_t * area, uint8_t * px_map) override
	{
        driver->SetWindow(area->x1, area->y1, area->x2, area->y2);
        size_t size = (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1);
        driver->WriteWindow((uint16_t*)px_map, size);
	    lv_display_flush_ready(disp);
	}
	
public:
    ST47796SAdapter(std::shared_ptr<LVGL::LVGLService> lvgl, std::shared_ptr<ST7796S> driver)
        : lvgl(lvgl), driver(driver)
    {
        
    }
    
	~ST47796SAdapter()
	{
		free(disp_buf);
	}
};
