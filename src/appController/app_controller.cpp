//
// Created by acer-zgy on 2022/8/28.
//

#include "app_controller.h"


void AppController::Gui() {
    screen = lv_scr_act();
    static lv_style_t screenStyle;
    lv_style_init(&screenStyle);
    lv_style_set_bg_color(&screenStyle, lv_color_black());
    lv_obj_add_style(screen, &screenStyle, LV_STATE_DEFAULT);

    appName = lv_label_create(screen);
    if (len >= 0) {
        currentApp = appList[0];
//    lv_obj_set_style_text_font(obj, LV_FONT_MONTSERRAT_28, LV_STATE_DEFAULT);
        lv_label_set_text(appName, this->currentApp->app_name);
        lv_obj_set_style_text_color(appName, lv_color_white(), LV_STATE_DEFAULT);
        lv_obj_align(appName, LV_ALIGN_CENTER, 0, 0);
    }
}

void AppController::destroyGui() {
    lv_obj_clean(screen);
    lv_obj_clean(appName);
}


//char *active_type[] = {"left", "right", "up", "down", "unknown"};
long lastMill = millis();

void AppController::controller(int active) {
    if (running) {
        mainProcess(active);
    }

    if (millis() - lastMill > 800) {
        switch (active) {
            case 0: {
                selIndex = (selIndex + 1) % len;
                currentApp = appList[selIndex];
                lv_label_set_text(appName, currentApp->app_name);
                break;
            }
            case 1: {
                selIndex = (selIndex - 1) % len;
                currentApp = appList[selIndex];
                lv_label_set_text(appName, currentApp->app_name);
                break;
            }
            case 2: {
                if (running) {
                    exit_app();
                    Gui();
                }
                break;
            }
            case 3:
                if (!running) {
                    destroyGui();
                    run_app();
                }
                break;

        }
        lastMill = millis();
    }

}


void AppController::run_app() {
    this->currentApp->setup();
    Serial.printf("启动APP->%s\n", currentApp->app_name);
    running = true;
}

void AppController::mainProcess(int active) {
    this->currentApp->loop(active);
}

void AppController::addApp(App *app) {
    this->appList[++selIndex] = app;
    len++;
}

void AppController::exit_app() {
    Serial.printf("退出APP->%s\n", currentApp->app_name);
    if (this->currentApp) {
        this->currentApp->exit();
        this->running = false;
    }
}



