#include <signal.h>
#include <lvgl/lvgl.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include "hal/app_hal.h"
#include "ui/ui_main.h"

static bool quit = false;

void sigint_handler(int _);

int main() {
	signal(SIGINT, sigint_handler);
    lv_init();
	lv_fs_stdio_init();
	hal_setup();

	ui_main();

	while (!quit) {
		lv_timer_handler();
		lv_tick_inc(5);
		hal_loop();
		usleep(5);
	}
    return 0;
}

void sigint_handler(int _) {
	quit = true;
}

static int monitor_task(void* data) {
	(void)(data);
	while (1) {
		lv_tick_inc(5);
		usleep(5);
	}
	return 0;
}
