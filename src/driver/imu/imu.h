//
// Created by zgy on 2022/8/27.
//

#ifndef M_HOLO_IMU_H
#define M_HOLO_IMU_H
#include <I2Cdev.h>
#include <MPU6050.h>
#include "../display/lv_port_indev.h"

#define IMU_I2C_SDA 32
#define IMU_I2C_SCL 33

extern int32_t encoder_diff;
extern lv_indev_state_t encoder_state;



enum Action{
    left=0,
    right,
    up,
    down,
    unknown
};

class IMU
{
private:
    MPU6050 imu;
    int16_t ax, ay, az;
    int16_t gx, gy, gz;
    int16_t ad_ax,ad_ay;
    long  last_update_time;

public:
    void init();
    void adjust();
    void update(int interval,int *activeType);

    int16_t getAccelX();
    int16_t getAccelY();
    int16_t getAccelZ();

    int16_t getGyroX();
    int16_t getGyroY();
    int16_t getGyroZ();

};

#endif //M_HOLO_IMU_H
