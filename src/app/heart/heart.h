//
// Created by acer-zgy on 2022/9/16.
//

#ifndef M_HOLO_HEART_H
#define M_HOLO_HEART_H

#include "appController/app_controller.h"
#include <lvgl.h>

class Heart : public App {
public:
    Heart();

    void setup() override;

    void loop(int active) override;

    void exit() override;
};

#endif //M_HOLO_HEART_H
