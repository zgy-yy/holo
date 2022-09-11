#include "driver/display/display.h"
#include "appController/app_controller.h"
#include "app/time/time.h"
#include "sys/webServer/webServer.h"
#include <Arduino.h>

Display screen;
AppController *appController;
WebServer *webServer;

void setup() {
    Serial.begin(115200);
    screen.init(4, 100);
    appController = new AppController();
    webServer = new WebServer();
    appController->run_app(new Time());
}

void processLoop() {
    appController->mainProcess();
}

void loop() {
    screen.routine(processLoop);
}