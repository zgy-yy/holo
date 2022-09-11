//
// Created by zgy on 2022/8/28.
//

#ifndef M_HOLO_DISPLAY_H
#define M_HOLO_DISPLAY_H
#include <lvgl.h>
class Display
{
public:
    void init(uint8_t rotation, uint8_t backLight);
    void routine(void (*fun)(void ));
    void setBackLight(float);
};
#endif //M_HOLO_DISPLAY_H
