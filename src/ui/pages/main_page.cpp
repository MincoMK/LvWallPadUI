#include "main_page.h"
#include "../utils/icons.h"
#include "../utils/pos.h"
#include <string>

#define NAVBTN_WIDTH 80
#define NAVBTN_HEIGHT 80

using namespace std;

lv_obj_t * navbtn_create(lv_obj_t * parent, Pos pos, string icon);

lv_obj_t * main_page_create(lv_obj_t * parent) {
	int row0_y = LV_VER_RES / 3 - NAVBTN_HEIGHT / 2;
	int row1_y = LV_VER_RES / 3 * 2 - NAVBTN_HEIGHT / 2;

	int col0_x = LV_HOR_RES / 4 - NAVBTN_WIDTH / 2;
	int col1_x = LV_HOR_RES / 4 * 2 - NAVBTN_WIDTH / 2;
	int col2_x = LV_HOR_RES / 4 * 3 - NAVBTN_WIDTH / 2;

	Pos pos0 = Pos{col0_x, row0_y};
	Pos pos1 = Pos{col1_x, row0_y};
	Pos pos2 = Pos{col2_x, row0_y};
	Pos pos3 = Pos{col0_x, row1_y};
	Pos pos4 = Pos{col1_x, row1_y};
	Pos pos5 = Pos{col2_x, row1_y};

	navbtn_create(parent, pos0, ICON_FAN);
	navbtn_create(parent, pos1, ICON_HOME);
	navbtn_create(parent, pos2, ICON_FAN);
	navbtn_create(parent, pos3, ICON_FAN);
	navbtn_create(parent, pos4, ICON_FAN);
	navbtn_create(parent, pos5, ICON_FAN);

	return parent;
}

lv_obj_t * navbtn_create(lv_obj_t * parent, Pos pos, string icon) {
	lv_obj_t * navbtn = lv_btn_create(parent);
	lv_obj_set_pos(navbtn, pos.x, pos.y);
	lv_obj_set_size(navbtn, NAVBTN_WIDTH, NAVBTN_HEIGHT);

	lv_obj_t * navbtn_img = lv_img_create(navbtn);
	lv_img_set_src(navbtn_img, icon.c_str());
	int z = 512 / 2 + 20;
	lv_obj_set_size(navbtn_img, z, z);
	lv_obj_align(navbtn_img, LV_ALIGN_CENTER, 0, 0);
	lv_img_set_zoom(navbtn_img, 150);

	return navbtn;
}
