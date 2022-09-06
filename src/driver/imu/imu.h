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

class IMU
{
private:
    MPU6050 imu;
    int flag;
    int16_t ax, ay, az;
    int16_t gx, gy, gz;

    long  last_update_time;

public:
    void init();

    void update(int interval);

    int16_t getAccelX();
    int16_t getAccelY();
    int16_t getAccelZ();

    int16_t getGyroX();
    int16_t getGyroY();
    int16_t getGyroZ();

};

#endif //M_HOLO_IMU_H
