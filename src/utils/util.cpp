//
// Created by zgy on 2022/9/4.
//

#include "util.h"
#include "driver/imu/imu.h"

ESP32Time localTime(28800);

void m_log(char *title, ...) {
    va_list vaList;
    char *p;
    va_start(vaList, '\0');

    Serial.println("----------------------------------");
    Serial.printf(" *\t\t%s\n", title);

    while ((p = va_arg(vaList, char *)) != NULL) {
        Serial.printf(" | \n");
        Serial.printf(" | %s\n", p);
    }
    Serial.println("----------------------------------");
    va_end(vaList);

}
