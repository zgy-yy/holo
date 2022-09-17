#include "driver/display/display.h"
#include "appController/app_controller.h"
#include "app/time/time.h"
#include "sys/webServer/webServer.h"
#include "sys/http/http.h"
#include "app/test/test.h"
#include "driver/imu/imu.h"
#include "sys/mqtt/mqttClient.h"
#include "app/computerWatcher/watcher.h"
#include "app/heart/heart.h"
#include <Arduino.h>

Display screen;
AppController *appController;
WebServer *webServer;
IMU mpu;
ImuAction *action;

void setup() {
    Serial.begin(115200);
    screen.init(4, 0);
    mpu.init(0);
    webServer = new WebServer();
    appController = new AppController();
    appController->addApp(new Watcher());
    appController->addApp(new Heart());
    appController->addApp(new Test());
    appController->addApp(new Time());
    appController->Gui();
    mqttClient.init();
    screen.setBackLight(100);
}

boolean isLogin = false;

void App_process() {
    action = mpu.getAction();
    mpu.update(300);
    appController->controller(action);
//
    if (!isLogin) {
        isLogin = httpLogin();
    }
    mqttClient.loop();
}

void loop() {
    screen.routine(App_process);
}