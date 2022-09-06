//
// Created by acer-zgy on 2022/8/28.
//

#ifndef M_HOLO_NETWORK_H
#define M_HOLO_NETWORK_H

#include <WiFi.h>


class NetWork {

private:
    char *host_name;
    char *ap_ssid = "holo-ap";
    char *ap_pwd = "12345678";
public:
    NetWork();

    void scan_wifi();

//连接wifi
    void connectWifi(const char *ssid, const char *password);

    bool disconnectWifi();

    void open_ap();
    String getWifiStatus();
    String getSearchedWifi();
};

NetWork *getNetWorkIns();

#endif //M_HOLO_NETWORK_H
