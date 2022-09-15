//
// Created by acer-zgy on 2022/8/28.
//

#ifndef M_HOLO_TEST_H
#define M_HOLO_TEST_H

#include "appController/app_controller.h"
#include <lvgl.h>
#include "PubSubClient.h"

class Test : public App {

public:
    Test();

    void setup() override;

    void loop(int) override;

    void exit() override;

};

#endif //M_HOLO_TEST_H
