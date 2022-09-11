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
    void ui();
    void destroyUi();
};
#endif //M_HOLO_TIME_H
