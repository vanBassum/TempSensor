#pragma once
#include "esp_system.h"
#include "widget.h"

namespace ESP_LVGL
{
	class Screen : public Widget
	{
	public:
		Screen(lv_obj_t* obj)
		{
			LVGL::ExecuteSafely([&]() {
				handle = obj;
			});
		}
	};
}
