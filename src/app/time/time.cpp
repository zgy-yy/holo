//
// Created by acer-zgy on 2022/8/28.
//

#include "time.h"
#include <Arduino.h>
#include <lv_conf.h>

void Time::main_process() {

    this->ui();
}

Time::Time() {

}

Time::Time(char *name, char *author, char *version, char *info) : App(name, author, version, info) {

}

void Time::ui() {
    lv_obj_t *act_screen = lv_scr_act(); // 获取当前活动页

    static lv_style_t app_style;
    lv_style_set_bg_color(&app_style, lv_color_black());
    lv_obj_add_style(act_screen,&app_style,LV_STATE_DEFAULT);


    lv_obj_t *label = lv_label_create(act_screen);
    lv_label_set_text(label, "TIME");
    lv_obj_set_align(label, LV_ALIGN_CENTER);

    static lv_style_t style_text;
    lv_style_init(&style_text);
    lv_style_set_text_color(&style_text, lv_color_white());
    lv_style_set_bg_color(&style_text, lv_color_black());
    lv_style_set_border_width(&style_text, 2);
    lv_style_set_border_color(&style_text, lv_color_white());
    lv_style_set_text_font(&style_text, &lv_font_montserrat_30);
    lv_obj_add_style(label,&style_text,LV_STATE_DEFAULT);


}