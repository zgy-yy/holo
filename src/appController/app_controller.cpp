//
// Created by acer-zgy on 2022/8/28.
//

#include "app_controller.h"
#include "sys/mqtt/mqttClient.h"
#include "driver/imu/imu.h"


void AppController::Gui() {
    screen = lv_scr_act();
    static lv_style_t screenStyle;
    lv_style_init(&screenStyle);
    lv_style_set_bg_color(&screenStyle, lv_color_black());
    lv_obj_add_style(screen, &screenStyle, LV_STATE_DEFAULT);

    appName = lv_label_create(screen);
    if (len >= 0) {
        if (!currentApp) {
            currentApp = appList[selIndex];
        }
//    lv_obj_set_style_text_font(obj, LV_FONT_MONTSERRAT_28, LV_STATE_DEFAULT);
        lv_label_set_text(appName, this->currentApp->app_name);
        lv_obj_set_style_pad_all(appName, 1, LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(appName, 1, LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(appName, lv_color_white(), LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(appName, lv_color_white(), LV_STATE_DEFAULT);
        lv_obj_align(appName, LV_ALIGN_CENTER, 0, 0);
    }
}

void AppController::destroyGui() {
    lv_obj_clean(screen);
}


//char *active_type[] = {"left", "right", "up", "down", "unknown"};
long lastMill = millis();

//
char *show_appName = "";

//app 主动通知 运行此app
void showMe(char *app_name) {
    show_appName = app_name;
}


void AppController::controller(ImuAction *action) {

    for (int i = 0; i < len; i++) {
        if (appList[i]->app_name == show_appName) {
            if (currentApp == appList[i]) {
                break;
            }
//            Serial.println("show");
            //退出当前app
            exit_app();
            currentApp = appList[i];
            run_app();
            show_appName = "";
        }
    }
    if (running) {
        mainProcess(action->active);
    }
    if (millis() - lastMill > 300) {
        if (action->active != 7) {
            Serial.println(active_type_info[action->active]);
        }

        if (!running) {
            switch (action->active) {
                case 0: { //turn right
                    selIndex = (selIndex + 1) % len;
                    currentApp = appList[selIndex];
                    lv_label_set_text(appName, currentApp->app_name);
                    break;
                }
                case 2: { //left
                    selIndex = (selIndex - 1 + len) % len;
                    currentApp = appList[selIndex];
                    lv_label_set_text(appName, currentApp->app_name);
                    break;
                }
                case 5:
                    if (!running) {
                        destroyGui();
                        run_app();
                    }
                    break;
            }
        } else {
            if (action->active == 1) {
                exit_app();
                Gui();
            }
        }
        lastMill = millis();
    }
    action->active = ACTIVE_TYPE::UNKNOWN;
    action->isValid = 0;
}


void AppController::run_app() {
    if (this->currentApp && !running) {
        destroyGui();
        this->currentApp->setup();
        Serial.printf("启动APP->%s\n", currentApp->app_name);
        running = true;
    }

}

void AppController::mainProcess(int active) {
    this->currentApp->loop(active);
}

void AppController::addApp(App *app) {
    this->appList[++selIndex] = app;
    len++;
}

void AppController::exit_app() {
    if (this->currentApp && running) {
        this->currentApp->exit();
        Serial.printf("退出APP->%s\n", currentApp->app_name);
        this->running = false;
    }
}






