#Created by VisualGDB. Right-click on the component in Solution Explorer to edit properties using convenient GUI.

idf_component_register(
	SRCS
		core/mlvgl.cpp
	INCLUDE_DIRS
		.
		core
		widgets
	REQUIRES lvgl ESP_Base ESP_Drivers)
