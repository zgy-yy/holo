#include "driver/display/display.h"
#include "appController/app_controller.h"
#include "app/time/time.h"
#include "sys/webServer/webServer.h"
#include "sys/http/http.h"
#include "app/test/test.h"
#include <Arduino.h>

Display screen;
AppController *appController;
WebServer *webServer;


void setup() {
    Serial.begin(115200);
    screen.init(4, 100);

    webServer = new WebServer();
    appController = new AppController();
    appController->addApp(new Time("time","","",""));
    appController->addApp(new Test("Test","","",""));
}

boolean isLogin = false;


int ind = 0;
void App_process() {
    if (!appController->isSetup) {
        appController->run_app(ind);
    } else {
        appController->mainProcess();
    }

//
    if (!isLogin) {
        isLogin = httpLogin();
    }
}

void loop() {
    screen.routine(App_process);
}