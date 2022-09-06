#include "driver/display/display.h"
#include "appController/app_controller.h"
#include "app/time/time.h"
#include "sys/webServer/webServer.h"
#include <Arduino.h>

Display screen;
WebServer *webServer;
void setup() {
    Serial.begin(115200);
    screen.init(4, 100);

    AppController *appController = new AppController();
    appController->run_app(new Time());

    webServer = new WebServer();
}

void loop() {
    screen.routine();

}