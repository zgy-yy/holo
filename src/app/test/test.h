//
// Created by acer-zgy on 2022/8/28.
//

#ifndef M_HOLO_TEST_H
#define M_HOLO_TEST_H

#include "appController/app_controller.h"

class Test : public App {
public:
    Test() {
    }
    Test(char *name, char *author, char *version, char *info) : App(name, author, version, info) {
    }

    void main_process() override;
};
#endif //M_HOLO_TEST_H
