//
// Created by acer-zgy on 2022/8/28.
//

#include "app_controller.h"

void AppController::run_app(App *app) {
    this->currentApp = app;
    app->setup();
}

void AppController::mainProcess() {
    this->currentApp->loop();
}
