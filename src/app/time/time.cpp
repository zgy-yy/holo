//
// Created by acer-zgy on 2022/8/28.
//

#include "time.h"
#include "sys/http/http.h"
#include <Arduino.h>
#include <lv_conf.h>

//int offset=28800;

Time::Time() {
    this->app_name ="TIME";
}

LV_FONT_DECLARE(mFont);
LV_FONT_DECLARE(timeFont);


void Time::screenInit() {
    act_screen = lv_scr_act();
    static lv_style_t screenStyle;
    lv_style_init(&screenStyle);
    lv_style_set_bg_color(&screenStyle, lv_color_black());
    lv_obj_add_style(act_screen, &screenStyle, LV_STATE_DEFAULT);
}

void Time::showDateView() {
    static lv_style_t dateStyle;
    lv_style_init(&dateStyle);
    lv_style_set_text_color(&dateStyle, lv_color_hex(0xf0faff));
    lv_style_set_x(&dateStyle, 20);
    lv_style_set_y(&dateStyle, 50);

    dateView = lv_label_create(act_screen);

    lv_obj_set_style_text_font(dateView, &mFont, LV_STATE_DEFAULT);
    lv_label_set_text(dateView, "2020年9月7日");
    lv_obj_add_style(dateView, &dateStyle, LV_STATE_DEFAULT);
}

void Time::showTimeView() {
    timeView = lv_obj_create(act_screen);
    static lv_style_t boxStyle;
    lv_style_init(&boxStyle);
    lv_style_set_border_width(&boxStyle, 0);
    lv_style_set_pad_all(&boxStyle, 0);
    lv_style_set_bg_color(&boxStyle, lv_color_black());
    lv_style_set_align(&boxStyle, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_size(timeView, 240, 125);
    lv_obj_add_style(timeView, &boxStyle, LV_STATE_DEFAULT);
    lv_obj_clear_flag(timeView, LV_OBJ_FLAG_SCROLLABLE);
}

static void anim_cb(void *var, int32_t v) {
    lv_obj_set_style_translate_y((lv_obj_t *) var, v, LV_STATE_DEFAULT);
}

void animationShow(lv_obj_t *obj) {
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_values(&a, lv_obj_get_x(obj) - 50, lv_obj_get_x(obj));
    lv_anim_set_time(&a, 300);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);
    lv_anim_set_exec_cb(&a, anim_cb);
    lv_anim_start(&a);
}

lv_obj_t *Time::showTimeItem(const char *val) {
    static lv_style_t labelStyle;
    lv_style_init(&labelStyle);
    lv_style_set_text_font(&labelStyle, &timeFont);
    lv_style_set_text_color(&labelStyle, lv_color_white());

    lv_obj_t *obj = lv_label_create(timeView);
    lv_label_set_text(obj, val);
    lv_obj_add_style(obj, &labelStyle, LV_STATE_DEFAULT);
    animationShow(obj);
    return obj;
}

static void anim_des_cb(void *var, int32_t v) {
    lv_obj_set_style_translate_y((lv_obj_t *) var, v, LV_STATE_DEFAULT);
    if (v == 70) {
        lv_obj_del_async((lv_obj_t *) var);
    }
}

void animationDes(lv_obj_t *obj) {
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_values(&a, lv_obj_get_y(obj) + 10, lv_obj_get_y(obj) + 70);
    lv_anim_set_time(&a, 300);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);
    lv_anim_set_exec_cb(&a, anim_des_cb);
    lv_anim_start(&a);
}

void delTimeItem(lv_obj_t *obj) {
    animationDes(obj);
}


void Time::ui() {
    screenInit();
    showDateView();
    showTimeView();
    hourL = showTimeItem("0");
    hourR = showTimeItem("0");
    colon = showTimeItem(":");
    minL = showTimeItem("0");
    minR = showTimeItem("0");
    lv_obj_set_style_translate_x(hourL, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_translate_x(hourR, 50, LV_STATE_DEFAULT);
    lv_obj_set_style_translate_x(colon, 85, LV_STATE_DEFAULT);
    lv_obj_set_style_translate_x(minL, 120, LV_STATE_DEFAULT);
    lv_obj_set_style_translate_x(minR, 170, LV_STATE_DEFAULT);

    sec = lv_label_create(timeView);
    lv_obj_set_style_text_font(sec, &mFont, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(sec, lv_color_white(), LV_STATE_DEFAULT);
    lv_label_set_text(sec, "0");
    lv_obj_set_align(sec, LV_ALIGN_BOTTOM_RIGHT);
    lv_obj_set_style_translate_y(sec, -67, LV_STATE_DEFAULT);
    lv_obj_set_style_translate_x(sec, 2, LV_STATE_DEFAULT);
}

void Time::destroyUi() {
    lv_obj_clean(act_screen);
}


void Time::update() {

    int secNum = localTime.getSecond();
    lv_label_set_text(sec, String(secNum).c_str());
    int H = localTime.getHour(true);
    int M = localTime.getMinute();
    if (secNum == 0 || calibration) {
        if (M == 0 || calibration) {
            if (H % 10 == 0 || calibration) {
                delTimeItem(hourL);
                hourL = showTimeItem(String(H / 10).c_str());
                delTimeItem(hourR);
                hourR = showTimeItem(String(H % 10).c_str());
            } else {
                delTimeItem(hourR);
                hourR = showTimeItem(String(H % 10).c_str());
            }
        }
        if (M % 10 == 0 || calibration) {
            delTimeItem(minL);
            minL = showTimeItem(String(M / 10).c_str());
            delTimeItem(minR);
            minR = showTimeItem(String(M % 10).c_str());
        } else {
            delTimeItem(minR);
            minR = showTimeItem(String(M % 10).c_str());
        }
        lv_obj_set_style_translate_x(hourL, 0, LV_STATE_DEFAULT);
        lv_obj_set_style_translate_x(hourR, 50, LV_STATE_DEFAULT);
        lv_obj_set_style_translate_x(colon, 85, LV_STATE_DEFAULT);
        lv_obj_set_style_translate_x(minL, 120, LV_STATE_DEFAULT);
        lv_obj_set_style_translate_x(minR, 170, LV_STATE_DEFAULT);
    }
//    整点更新
    if (localTime.getHour() == 0 && localTime.getMinute() == 0 || calibration) {
        String date = "";
        date += localTime.getYear();
        date += "年";
        date += localTime.getMonth() + 1;
        date += "月";
        date += localTime.getDay();
        date += "日";
        lv_label_set_text(dateView, date.c_str());
    }
}

//void IRAM_ATTR onTimer() {
//    Time::update();
//}


void Time::setup() {
    localTime.setTime(1652112000);
    ui();
    lastsec = millis();
//    hw_timer_t *timer = timerBegin(0, 80, true);
//    timerAttachInterrupt(timer, &onTimer, true);
//    timerAlarmWrite(timer, 1000000, true);
//    timerAlarmEnable(timer);
}


void Time::loop(int active) {
//    隔一段时间校准;
    if (millis() - lastsec > 1000) {
        lastsec = millis();
        if (calibration) {
            unsigned long stamp = httpGetTime();
            if (stamp > 0) {
                localTime.setTime(stamp);
                update();
                calibration = false;
            }
        }
        update();
        updateIndex++;
        if (updateIndex == 50) {
            updateIndex = 0;
            update();
        }
//        Serial.printf("upDateIndex;%d\n", updateIndex);
    }

}

void Time::exit() {
    destroyUi();
    calibration = true;
    updateIndex = 0;
}




//extern lv_font_t mFont;
//LV_FONT_DECLARE(mFont);