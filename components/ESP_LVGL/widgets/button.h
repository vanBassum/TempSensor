#pragma once
#include "esp_system.h"
#include "widget.h"

namespace ESP_LVGL
{
	class Button : public Widget
	{
	public:
		Button(Widget& parent)
		{
			LVGL::ExecuteSafely([&]() {
				handle = lv_btn_create(parent.handle);
			});
		}
	};
}
