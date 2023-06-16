#include <src/font/lv_font.h>
#include <unistd.h>
#define SDL_MAIN_HANDLED        /*To fix SDL's "undefined reference to WinMain" issue*/
#include <SDL2/SDL.h>
#include <lvgl/lvgl.h>
#include "app_hal.h"

#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 320

static void *fb1, *fb2;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;
static lv_disp_drv_t disp_drv;
static lv_disp_draw_buf_t draw_buf;
static int DISPLAY_WIDTH;
static int DISPLAY_HEIGHT;
static lv_indev_drv_t indev_drv;
static bool cur_pressed = false;
#define WINDOW_NAME "LVGL"

void monitor_flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * buf);
void mouse_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);

void hal_loop() {

}

void lv_port_disp_init(int width, int height)
{
    DISPLAY_WIDTH = width;
    DISPLAY_HEIGHT = height;
    SDL_InitSubSystem(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(WINDOW_NAME,
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              DISPLAY_WIDTH, DISPLAY_HEIGHT, 0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    texture = SDL_CreateTexture(renderer,
                                (LV_COLOR_DEPTH == 32) ? (SDL_PIXELFORMAT_ARGB8888) : (SDL_PIXELFORMAT_RGB565),
                                SDL_TEXTUREACCESS_STREAMING,
                                DISPLAY_WIDTH,
                                DISPLAY_HEIGHT);

    fb1 = malloc(DISPLAY_WIDTH * DISPLAY_HEIGHT * ((LV_COLOR_DEPTH + 7) / 8));
    fb2 = malloc(DISPLAY_WIDTH * DISPLAY_HEIGHT * ((LV_COLOR_DEPTH + 7) / 8));

    lv_disp_draw_buf_init(&draw_buf, fb1, fb2, DISPLAY_WIDTH * DISPLAY_HEIGHT);
    lv_disp_drv_init(&disp_drv);

    disp_drv.hor_res = DISPLAY_WIDTH;
    disp_drv.ver_res = DISPLAY_HEIGHT;
    disp_drv.flush_cb = monitor_flush;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.full_refresh = 1;
    lv_disp_drv_register(&disp_drv);
}

void lv_port_indev_init() {
	lv_indev_drv_init(&indev_drv);
	indev_drv.type = LV_INDEV_TYPE_POINTER;
	indev_drv.read_cb = mouse_read;
	lv_indev_drv_register(&indev_drv);
}

void hal_setup() {
	lv_port_disp_init(WINDOW_WIDTH, WINDOW_HEIGHT);
	lv_port_indev_init();
}

void monitor_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    SDL_Rect r;
    r.x = area->x1;
    r.y = area->y1;
    r.w = area->x2 - area->x1 + 1;
    r.h = area->y2 - area->y1 + 1;
    SDL_UpdateTexture(texture, &r, color_p, r.w * ((LV_COLOR_DEPTH + 7) / 8));
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    lv_disp_flush_ready(disp_drv);
}

void mouse_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                data->state = LV_INDEV_STATE_PR;
				cur_pressed = true;
            }
        } else if (event.type == SDL_MOUSEBUTTONUP) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                data->state = LV_INDEV_STATE_REL;
				cur_pressed = false;
            }
        } else if (event.type == SDL_MOUSEMOTION) {
			data->state = cur_pressed ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
            data->point.x = event.motion.x;
            data->point.y = event.motion.y;
        }
    }
}
