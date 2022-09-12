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
        String res = http.getString();
        int timeIndex = (res.indexOf("\"t\":"));
//        Serial.printf("%s\n", res.substring(timeIndex+4,timeIndex+14).c_str());
        return atol(res.substring(timeIndex + 4, timeIndex + 14).c_str());
    } else {
        Serial.printf("请求网络时间失败！\n");
    }
    http.end();
    return 0;
}

boolean httpLogin() {
    boolean isLogin = false;
    if (WiFi.status() == WL_CONNECTED) {
        String url = "http://192.168.200.2:801/eportal/?c=Portal&a=login&callback=dr1003&login_method=1&user_account=%2C1%2C3116503%40cmcc&user_password=china19971013&wlan_user_ip=";
        url += WiFi.localIP().toString();
        url += "&wlan_user_ipv6=&wlan_user_mac=000000000000&wlan_ac_ip=&wlan_ac_name=&jsVersion=3.3.3&v=9312";
        http.begin(url);
        int httpCode = http.GET();
        if (httpCode > 0) {
            String res = http.getString();

            int resCode = atol(res.substring(18, 19).c_str());
            int retCode=atol(res.substring(41, 42).c_str());
            if (resCode==0&&retCode==2){
                isLogin = true;
            }
//            Serial.println(res);
//            Serial.printf("resCode:%d,retCode:%d\n",resCode,retCode);
        }
    }

    return isLogin;
}