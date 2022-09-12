//
// Created by acer-zgy on 2022/8/28.
//

#include "test.h"

//


void Test::setup() {
    screen = lv_scr_act();
    static lv_style_t screenStyle;
    lv_style_init(&screenStyle);
    lv_style_set_bg_color(&screenStyle, lv_color_black());
    lv_obj_add_style(screen, &screenStyle, LV_STATE_DEFAULT);

    obj = lv_label_create(screen);
//    lv_obj_set_style_text_font(obj, LV_FONT_MONTSERRAT_28, LV_STATE_DEFAULT);
    lv_label_set_text(obj, "TEXT");
    lv_obj_set_style_text_color(obj, lv_color_white(), LV_STATE_DEFAULT);
    lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);
}

void Test::loop() {

}

void Test::exit() {
    lv_obj_clean(obj);
    lv_obj_clean(screen);
}
