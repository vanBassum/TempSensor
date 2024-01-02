#pragma once
#include "esp_system.h"
#include "mlvgl.h"

namespace ESP_LVGL
{
	class Widget
	{
	public:    
		lv_obj_t* handle = NULL;
		
		virtual ~Widget() 
		{
			LVGL::ExecuteSafely([&]() 
			{
				lv_obj_del_async(handle);
			});	
		}

		void SetPosition(int x, int y)	
		{ 
			LVGL::ExecuteSafely([&]() 
			{
				lv_obj_set_pos(handle, x, y);
			});	
		}	

		void SetSize(int width, int height)	
		{ 
			LVGL::ExecuteSafely([&]() 
			{
				lv_obj_set_size(handle, width, height);
			});
		}
		
		void SetAlign(lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs) 
		{ 
			LVGL::ExecuteSafely([&]() 
			{
				lv_obj_align(handle, align, x_ofs, y_ofs);
			});
		}
	};
}

