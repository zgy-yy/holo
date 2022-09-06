//
// Created by zgy on 2022/8/28.
//

#ifndef M_HOLO_LV_PORT_INDEV_H
#define M_HOLO_LV_PORT_INDEV_H
#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl.h"

extern lv_indev_t* indev_encoder;

void lv_port_indev_init(void);


#ifdef __cplusplus
} /* extern "C" */
#endif
#endif //M_HOLO_LV_PORT_INDEV_H
