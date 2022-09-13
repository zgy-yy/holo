//
// Created by zgy on 2022/8/27.
//

#include "imu.h"

void IMU::init() {
    Wire.begin(IMU_I2C_SDA, IMU_I2C_SCL);
    Wire.setClock(400000);
    while (!imu.testConnection());
    imu.initialize();
}

char *active_type[] = {"left", "right", "up", "down", "unknown"};

void IMU::adjust() {
    imu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    ad_ax = ax;
    ad_ay = ay;
    Serial.printf("ax:%d,ay:%d", ax, ay);
}


void IMU::update(int interval, int *activeType) {
    if (millis() - last_update_time > interval) {
        imu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        int actType = 4;//unknown

        if (ay - ad_ay > 3000) {
            encoder_diff--;
            actType = 0;
        } else if (ay - ad_ay < -3000) {
            encoder_diff++;
            actType = 1;
        }

        if (ax - ad_ax > 3000) {
            encoder_state = LV_INDEV_STATE_PR;//按下
            actType = 2;
        } else if (ax - ad_ax < -3000) {
            encoder_state = LV_INDEV_STATE_REL;//松开
            actType = 3;//down
        }

        last_update_time = millis();
        *activeType = actType;
        Serial.println(active_type[actType]);
    }
}

int16_t IMU::getAccelX() {
    return ax;
}

int16_t IMU::getAccelY() {
    return ay;
}

int16_t IMU::getAccelZ() {
    return az;
}

int16_t IMU::getGyroX() {
    return gx;
}

int16_t IMU::getGyroY() {
    return gy;
}

int16_t IMU::getGyroZ() {
    return gz;
}


