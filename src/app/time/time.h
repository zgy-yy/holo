//
// Created by acer-zgy on 2022/8/28.
//

#ifndef M_HOLO_TIME_H
#define M_HOLO_TIME_H
#include "appController/app_controller.h"
#include <lvgl.h>


class Time : public App {
public:
    Time();
    Time(char *name, char *author, char *version, char *info);
    void main_process() override;
private:
    void ui();
};
#endif //M_HOLO_TIME_H
