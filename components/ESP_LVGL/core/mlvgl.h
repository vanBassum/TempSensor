#pragma once
#include "esp_system.h"
#include "kernel.h"
#include "esp_log.h"
#include "lvgl.h"
#include <functional>
#include <type_traits>

#define LVGL_HANDLER_TICK_MS	5	//The timing is not critical but it should be about 5 milliseconds to keep the system responsive.
#define LVGL_TIMER_TICK_MS		1

namespace ESP_LVGL
{
	class LVGL
	{
		const char* TAG = "LVGL";

		int coreId = -1;
		Task task;
		Timer timer;	
		RecursiveMutex mutex;

		LVGL();
		void Work();
		void Execute(std::function<void()> function);
		
	public:
		static LVGL& GetOrCreateInstance();
		static void ExecuteSafely(std::function<void()> function);

		// The copy constructor and assignment operator are deleted to prevent cloning and assignment.
		LVGL(LVGL &other) = delete;
		void operator=(const LVGL &) = delete;
	};
}
