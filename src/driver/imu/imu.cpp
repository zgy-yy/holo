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


void IMU::adjust() {
    imu.CalibrateAccel(7);
    imu.CalibrateGyro(7);
    imu.PrintActiveOffsets();
    imu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    ad_ax = ax;
    ad_ay = ay;
    Serial.printf("ax:%d,ay:%d\n", ax, ay);
}

int active_type[3] = {ACTION_TYPE::unknown};

void pushAction(int type) {
    for (int i = 0; i < 3; i++) {
        active_type[i] = active_type[i + 1];
        if (i == 2) {
            active_type[i] = type;
        }
    }
}

void IMU::update() {
    static int interval = 300;
    if (millis() - last_update_time > interval) {
        imu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
//        Serial.printf("ax:%d,ay:%d\n", ax, ay);
        if (ay - ad_ay > 4000) {
            encoder_diff--;
            pushAction(ACTION_TYPE::left);
        } else if (ay - ad_ay < -4000) {
            encoder_diff++;
            pushAction(ACTION_TYPE::right);
        }

        if (ax - ad_ax > 5000) {
            encoder_state = LV_INDEV_STATE_PR;//按下
            pushAction(ACTION_TYPE::up);
        } else if (ax - ad_ax < -5000) {
            encoder_state = LV_INDEV_STATE_REL;//松开
            pushAction(ACTION_TYPE::down);
        }

        if (ay - ad_ay < 4000 && ay - ad_ay > -4000 && ax - ad_ax < 5000 && ax - ad_ax > -5000) {
            pushAction(ACTION_TYPE::unknown);

        }

        last_update_time = millis();
    }
}

int IMU::getAction() {
    if (active_type[0] == active_type[1] == active_type[1]) {
        active_type[2] = 4;
    }
    if (active_type[0] == active_type[1]) {
        active_type[1] = 4;
    }
    return active_type[0];
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



