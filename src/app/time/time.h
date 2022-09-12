//
// Created by acer-zgy on 2022/8/28.
//

#ifndef M_HOLO_TIME_H
#define M_HOLO_TIME_H

#include "appController/app_controller.h"
#include "ESP32Time.h"
#include <lvgl.h>

extern ESP32Time localTime;

class Time : public App {
public:
    Time();

    Time(char *name, char *author, char *version, char *info);

    void setup() override;

    void loop() override;

    void exit() override;

private:
    lv_obj_t *act_screen;
    lv_obj_t *dateView;
    lv_obj_t *timeView;
    lv_obj_t *hourL;
    lv_obj_t *hourR;
    lv_obj_t *minL;
    lv_obj_t *minR;
    lv_obj_t *sec;
    lv_obj_t *colon;
    unsigned long lastsec = 0;
    int updateIndex = 0;
    boolean calibration = true;

    void screenInit();

    void showDateView();

    void showTimeView();

    lv_obj_t *showTimeItem(const char *);

    void update();

    void ui();

    void destroyUi();
};

#endif //M_HOLO_TIME_H
