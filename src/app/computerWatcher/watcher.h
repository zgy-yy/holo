//
// Created by acer-zgy on 2022/9/15.
//

#ifndef M_HOLO_WATCHER_H
#define M_HOLO_WATCHER_H

#include "appController/app_controller.h"

class Watcher : public App {
public:
    Watcher();

    void setup() override;

    void loop(int active) override;

    void exit() override;
};

#endif //M_HOLO_WATCHER_H
