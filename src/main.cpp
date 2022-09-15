#include "driver/display/display.h"
#include "appController/app_controller.h"
#include "app/time/time.h"
#include "sys/webServer/webServer.h"
#include "sys/http/http.h"
#include "app/test/test.h"
#include "driver/imu/imu.h"
#include "sys/mqtt/mqttClient.h"
#include "app/computerWatcher/watcher.h"
#include <Arduino.h>

Display screen;
AppController *appController;
WebServer *webServer;
IMU mpu;

void setup() {
    Serial.begin(115200);
    screen.init(4, 100);
    mpu.init();
//    mpu.adjust();
    webServer = new WebServer();
    appController = new AppController();
    appController->addApp(new Time());
    appController->addApp(new Watcher());
    appController->addApp(new Test());
    appController->Gui();
    mqttClient.init();

}

boolean isLogin = false;


void App_process() {
    mpu.update();
    appController->controller(mpu.getAction());
//
    if (!isLogin) {
        isLogin = httpLogin();
    }
    mqttClient.loop();
}

void loop() {
    screen.routine(App_process);
}