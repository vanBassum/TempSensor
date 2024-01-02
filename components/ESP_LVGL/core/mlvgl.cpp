#include "mlvgl.h"

ESP_LVGL::LVGL::LVGL()
{
	lv_init();

	task.Init("LVGL", 2, 2048 * 4);
	task.SetHandler([&](Task* task, void* args){this->Work();});
	task.RunPinned(0);
			
	timer.Init("LVGL", TimeSpan(LVGL_TIMER_TICK_MS));
	timer.SetHandler([](Timer* t) { lv_tick_inc(t->GetPeriod().GetMiliSeconds()); });
	timer.Start();
}



void ESP_LVGL::LVGL::Work()
{
	mutex.Take();
	coreId = Task::GetCurrentCoreID();	//Note the coreId.
	mutex.Give();
	ESP_LOGI(TAG, "Running on core %d", coreId);
	while (1)
	{
		if (mutex.Take(pdMS_TO_TICKS(1000)))
		{
			lv_timer_handler();
			mutex.Give();
		}
		else
			ESP_LOGE(TAG, "Mutex in Work");
		
		vTaskDelay(pdMS_TO_TICKS(LVGL_HANDLER_TICK_MS));
	}
}


void ESP_LVGL::LVGL::ExecuteSafely(std::function<void()> function)
{
	return GetOrCreateInstance().Execute(function);
}


void ESP_LVGL::LVGL::Execute(std::function<void()> function)
{
	if (mutex.Take(pdMS_TO_TICKS(1000)))
	{
		if (coreId != Task::GetCurrentCoreID())
			ESP_LOGE(TAG, "Called LVGL function from wrong core. Use core %d", coreId);
		function();	
		mutex.Give();		
	}
	else
		ESP_LOGE(TAG, "Mutex in Execute");
	
}



ESP_LVGL::LVGL& ESP_LVGL::LVGL::GetOrCreateInstance()
{
	static LVGL instance;  // Constructed on first use
	return instance;
}

