
// Created by zgy on 2022/8/27.
//

#include "imu.h"

boolean doDelayMillisTime(unsigned long interval, unsigned long *previousMillis, boolean state) {
    unsigned long currentMillis = millis();
    if (currentMillis - *previousMillis >= interval) {
        *previousMillis = currentMillis;
        state = !state;
    }
    return state;
}

const char *active_type_info[] = {"TURN_RIGHT", "RETURN",
                                  "TURN_LEFT", "UP",
                                  "DOWN", "GO_FORWORD",
                                  "SHAKE", "UNKNOWN"};

IMU::IMU() {
    action_info.isValid = false;
    action_info.active = ACTIVE_TYPE::UNKNOWN;
    action_info.long_time = true;
    // 初始化数据
    for (int pos = 0; pos < ACTION_HISTORY_BUF_LEN; ++pos) {
        // act_info_history.push_back(UNKNOWN);
        act_info_history[pos] = UNKNOWN;
    }
    act_info_history_ind = ACTION_HISTORY_BUF_LEN - 1;
    this->order = 0; // 表示方位
}

void IMU::init(uint8_t order) {
    this->setOrder(order); // 设置方向
    Wire.begin(IMU_I2C_SDA, IMU_I2C_SCL);
    Wire.setClock(400000);
    unsigned long timeout = 5000;
    unsigned long preMillis = millis();
    // mpu = MPU6050(0x68, &Wire);
    mpu = MPU6050(0x68);
    Serial.print(F("Unable to connect to 4.\n"));
    while (!mpu.testConnection() && !doDelayMillisTime(timeout, &preMillis, false));
    Serial.print(F("Unable to connect to 5.\n"));

    if (!mpu.testConnection()) {
        Serial.print(F("Unable to connect to MPU6050.\n"));
        return;
    }

    Serial.print(F("Initialization MPU6050 now, Please don't move.\n"));
    mpu.initialize();
    // 启动自动校准
    // 7次循环自动校正
    mpu.CalibrateAccel(2);
    mpu.CalibrateGyro(2);
    mpu.PrintActiveOffsets();


    Serial.print(F("Initialization MPU6050 success.\n"));
}

void IMU::setOrder(uint8_t order) // 设置方向
{
    this->order = order; // 表示方位
}

bool IMU::Encoder_GetIsPush(void) {
#ifdef PEAK
    return (digitalRead(CONFIG_ENCODER_PUSH_PIN) == LOW);
#else
    return false;
#endif
}

ImuAction *IMU::update(int interval) {
    getVirtureMotion6(&action_info);
    // 原先判断的只是加速度，现在要加上陀螺仪
    if (millis() - last_update_time > interval) {
        if (!action_info.isValid) {
            if (action_info.v_ay > 4000) {
                encoder_diff--;
                action_info.isValid = 1;
                action_info.active = ACTIVE_TYPE::TURN_LEFT;
            } else if (action_info.v_ay < -4000) {
                encoder_diff++;
                action_info.isValid = 1;
                action_info.active = ACTIVE_TYPE::TURN_RIGHT;
            } else if (action_info.v_ay > 1000 || action_info.v_ay < -1000) {
                // 震动检测
                encoder_diff--;
                action_info.isValid = 1;
                action_info.active = ACTIVE_TYPE::SHAKE;
            } else {
                action_info.isValid = 0;
            }
        }

        if (!action_info.isValid) {
            if (action_info.v_ax > 5000) {
                action_info.isValid = 1;
                action_info.active = ACTIVE_TYPE::UP;
                delay(500);
                getVirtureMotion6(&action_info);
                if (action_info.v_ax > 5000) {
                    action_info.isValid = 1;
                    action_info.active = ACTIVE_TYPE::GO_FORWORD;
                    encoder_state = LV_INDEV_STATE_PR;
                }
            } else if (action_info.v_ax < -5000) {
                action_info.isValid = 1;
                action_info.active = ACTIVE_TYPE::DOWN;
                delay(500);
                getVirtureMotion6(&action_info);
                if (action_info.v_ax < -5000) {
                    action_info.isValid = 1;
                    action_info.active = ACTIVE_TYPE::RETURN;
                    encoder_state = LV_INDEV_STATE_REL;
                }
            } else if (action_info.v_ax > 1000 || action_info.v_ax < -1000) {
                // 震动检测
                action_info.isValid = 1;
                action_info.active = ACTIVE_TYPE::SHAKE;
            } else {
                action_info.isValid = 0;
            }
        }

        last_update_time = millis();
    }
    return &action_info;
}

ImuAction *IMU::getAction(void) {
    // 基本方法: 通过对近来的动作数据简单的分析，确定出动作的类型
    ImuAction tmp_info;
    getVirtureMotion6(&tmp_info);

    // Serial.printf("gx = %d\tgy = %d\tgz = %d", tmp_info.v_gx, tmp_info.v_gy, tmp_info.v_gz);
    // Serial.printf("\tax = %d\tay = %d\taz = %d\n", tmp_info.v_ax, tmp_info.v_ay, tmp_info.v_az);

    tmp_info.active = ACTIVE_TYPE::UNKNOWN;

    // 原先判断的只是加速度，现在要加上陀螺仪
    if (ACTIVE_TYPE::UNKNOWN == tmp_info.active) {
        if (tmp_info.v_ay > 4000) {
            tmp_info.active = ACTIVE_TYPE::TURN_LEFT;
        } else if (tmp_info.v_ay < -4000) {
            tmp_info.active = ACTIVE_TYPE::TURN_RIGHT;
        } else if (tmp_info.v_ay > 1000 || tmp_info.v_ay < -1000) {
            // 震动检测
            tmp_info.active = ACTIVE_TYPE::SHAKE;
        }
    }

    if (ACTIVE_TYPE::UNKNOWN == tmp_info.active) {
        if (tmp_info.v_ax > 5000) {
            tmp_info.active = ACTIVE_TYPE::UP;
        } else if (tmp_info.v_ax < -5000) {
            tmp_info.active = ACTIVE_TYPE::DOWN;
        } else if (action_info.v_ax > 1000 || action_info.v_ax < -1000) {
            // 震动检测
            tmp_info.active = ACTIVE_TYPE::SHAKE;
        }
    }

    // 储存当前检测的动作数据到动作缓冲区中
    act_info_history_ind = (act_info_history_ind + 1) % ACTION_HISTORY_BUF_LEN;
    int index = act_info_history_ind;
    act_info_history[index] = tmp_info.active;

    // 本次流程的动作识别
    if (!action_info.isValid) {
        bool isHoldDown = false;// 长按的标志位
        // 本次流程的动作识别
        int second = (index + ACTION_HISTORY_BUF_LEN - 1) % ACTION_HISTORY_BUF_LEN;
        int third = (index + ACTION_HISTORY_BUF_LEN - 2) % ACTION_HISTORY_BUF_LEN;
        // 先识别"短按" （注：不要写成else if）
        if (ACTIVE_TYPE::UNKNOWN != tmp_info.active) {
            action_info.isValid = 1;
            action_info.active = tmp_info.active;
        }
        // 识别"长按","长按"相对"短按"高级（所以键值升级放在短按之后）
        if (act_info_history[index] == act_info_history[second] &&
            act_info_history[second] == act_info_history[third]) {
            // 目前只识别前后的长按
            if (ACTIVE_TYPE::UP == tmp_info.active) {
                isHoldDown = true;
                action_info.isValid = 1;
                action_info.active = ACTIVE_TYPE::GO_FORWORD;
            } else if (ACTIVE_TYPE::DOWN == tmp_info.active) {
                isHoldDown = true;
                action_info.isValid = 1;
                action_info.active = ACTIVE_TYPE::RETURN;
            }
            // 如需左右的长按可在此处添加"else if"的逻辑

            if (isHoldDown) {
                // 本次识别为长按，则手动清除识别过的历史数据 避免对下次动作识别的影响
                act_info_history[second] = ACTIVE_TYPE::UNKNOWN;
                act_info_history[third] = ACTIVE_TYPE::UNKNOWN;
            }
        }
    }
//    Serial.println(active_type_info[action_info.active]);

    return &action_info;
}

void IMU::getVirtureMotion6(ImuAction *action_info) {
    mpu.getMotion6(&(action_info->v_ax), &(action_info->v_ay),
                   &(action_info->v_az), &(action_info->v_gx),
                   &(action_info->v_gy), &(action_info->v_gz));

    if (order & X_DIR_TYPE) {
        action_info->v_ax = -action_info->v_ax;
        action_info->v_gx = -action_info->v_gx;
    }

    if (order & Y_DIR_TYPE) {
        action_info->v_ay = -action_info->v_ay;
        action_info->v_gy = -action_info->v_gy;
    }

    if (order & Z_DIR_TYPE) {
        action_info->v_az = -action_info->v_az;
        action_info->v_gz = -action_info->v_gz;
    }

    if (order & XY_DIR_TYPE) {
        int16_t swap_tmp;
        swap_tmp = action_info->v_ax;
        action_info->v_ax = action_info->v_ay;
        action_info->v_ay = swap_tmp;
        swap_tmp = action_info->v_gx;
        action_info->v_gx = action_info->v_gy;
        action_info->v_gy = swap_tmp;
    }
}