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
    virtual void main_process() = 0;
};

class AppController {
public:
    AppController() {
    }

    void run_app(App *app);
};

#endif //M_HOLO_APP_CONTROLLER_H
