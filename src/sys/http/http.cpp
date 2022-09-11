//
// Created by acer-zgy on 2022/9/10.
//

#include "http.h"

HTTPClient http;

unsigned long httpGetTime() {
    http.begin("http://vv.video.qq.com/checktime?otype=json");
    int httpCode = http.GET(); // 发起GET请求
    if (httpCode > 0) // 如果状态码大于0说明请求过程无异常
    {
        String res =  http.getString();
        int timeIndex = (res.indexOf("\"t\":"));
//        Serial.printf("%s\n", res.substring(timeIndex+4,timeIndex+14).c_str());
        return atol(res.substring(timeIndex+4,timeIndex+14).c_str());
    } else {
        Serial.printf("请求网络时间失败！\n");
    }
    http.end();
    return 0;
}
