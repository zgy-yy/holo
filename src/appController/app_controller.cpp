//
// Created by acer-zgy on 2022/8/28.
//

#include "app_controller.h"

void AppController::run_app(int index) {
    this->currentApp = this->appList[index];
    this->currentApp->setup();
    Serial.printf("启动APP->%s\n",currentApp->app_name);
    isSetup = true;
}

void AppController::mainProcess() {
    this->currentApp->loop();
}

void AppController::addApp(App *app) {
    this->appList[++len] = app;
}

void AppController::exit_app() {
    Serial.printf("退出APP->%s\n",currentApp->app_name);
    if (this->currentApp) {
        this->currentApp->exit();
        this->isSetup = false;
    }
}

