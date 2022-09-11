//
// Created by acer-zgy on 2022/8/28.
//

#ifndef M_HOLO_APP_CONTROLLER_H
#define M_HOLO_APP_CONTROLLER_H

class App {
private:
    char *app_name;
    char *app_author;
    char *app_version;
    char *app_info;

public:
    App() {}
    App(char *name, char *author, char *version, char *info) {
        this->app_name = name;
        this->app_author = author;
        this->app_version = version;
        this->app_info = info;
    }

    //子类必须实例化该函数，作为应用的入口
    virtual void setup() = 0;
    virtual void loop()=0;
//    程序退出，应打扫现场，防止内存泄露
    virtual void exit()=0;
};

class AppController {
    App *currentApp;

public:
    AppController() {
    }

    void run_app(App *app);
    void mainProcess();
};

#endif //M_HOLO_APP_CONTROLLER_H
