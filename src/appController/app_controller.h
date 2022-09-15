//
// Created by acer-zgy on 2022/8/28.
//

#ifndef M_HOLO_APP_CONTROLLER_H
#define M_HOLO_APP_CONTROLLER_H

#include <Arduino.h>
#include <lvgl.h>

class App {
private:


public:
    char *app_name;
    char *app_author;
    char *app_version;
    char *app_info;

    App() {}

    App(char *name, char *author, char *version, char *info) {
        this->app_name = name;
        this->app_author = author;
        this->app_version = version;
        this->app_info = info;
    }

    //子类必须实例化该函数，作为应用的入口,只执行一次，用于初始化
    virtual void setup() = 0;

    virtual void loop(int active) = 0;

//    程序退出，应打扫现场，防止内存泄露
    virtual void exit() = 0;
};

class AppController {
    int selIndex = -1;
    int len = 0;
    App *currentApp;

    void run_app();

    void mainProcess(int active);

    void exit_app();

    App *appList[10];
    boolean running = false;
public:


    AppController() {}

    void addApp(App *app);

    lv_obj_t *screen;
    lv_obj_t *appName;

    void Gui();

    void destroyGui();

    void controller(int active);

};
void showMe(char *app_name);
#endif //M_HOLO_APP_CONTROLLER_H
