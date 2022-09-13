#include "driver/display/display.h"
#include "appController/app_controller.h"
#include "app/time/time.h"
#include "sys/webServer/webServer.h"
#include "sys/http/http.h"
#include "app/test/test.h"
#include "driver/imu/imu.h"
#include <Arduino.h>

Display screen;
AppController *appController;
WebServer *webServer;
IMU mpu;

void setup() {
    Serial.begin(115200);
    screen.init(4, 100);
    mpu.init();
    mpu.adjust();
    webServer = new WebServer();
    appController = new AppController();
    appController->addApp(new Time("time", "", "", ""));
    appController->addApp(new Test("Test", "", "", ""));
    appController->Gui();
}

boolean isLogin = false;


int active = 4;

void App_process() {
    mpu.update(1000, &active);

    appController->controller(active);

//
    if (!isLogin) {
        isLogin = httpLogin();
    }
}

void loop() {
    screen.routine(App_process);
}