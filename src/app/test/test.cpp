//
// Created by acer-zgy on 2022/8/28.
//

#include <WiFiClient.h>
#include "test.h"
#include "sys/mqtt/mqttClient.h"

lv_obj_t *appscreen;
lv_obj_t *obj;

Test::Test() {

    this->app_name="TEXT";

    appscreen = NULL;
    obj = NULL;
    mqttClient.addReceiveCallback("/yy", [](byte *payload, unsigned int length) {
        payload[length] = '\0';
        showMe("TEXT");
//        if (obj) {
//            lv_label_set_text(obj, (char *) payload);
//        }
    });
}



void Test::setup() {
    appscreen = lv_scr_act();
    static lv_style_t screenStyle;
    lv_style_init(&screenStyle);
    lv_style_set_bg_color(&screenStyle, lv_color_black());
    lv_obj_add_style(appscreen, &screenStyle, LV_STATE_DEFAULT);

    obj = lv_label_create(appscreen);
//    lv_obj_set_style_text_font(obj, LV_FONT_MONTSERRAT_28, LV_STATE_DEFAULT);
    lv_label_set_text(obj, "Hello");
    lv_obj_set_style_text_color(obj, lv_color_white(), LV_STATE_DEFAULT);
    lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);
}


void Test::loop(int active) {
    if (Serial.available()) {
        String str = Serial.readString();
        lv_label_set_text(obj, str.c_str());
    }
}

void Test::exit() {
    lv_obj_clean(appscreen);
    appscreen = NULL;
}



