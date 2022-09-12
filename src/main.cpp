#include "driver/display/display.h"
#include "appController/app_controller.h"
#include "app/time/time.h"
#include "sys/webServer/webServer.h"
#include "sys/http/http.h"
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

boolean isLogin = false;
void processLoop() {
    appController->mainProcess();
    if(!isLogin){
        isLogin = httpLogin();
    }
}

void loop() {
    screen.routine(processLoop);
}