//
// Created by acer-zgy on 2022/8/28.
//

#include "network.h"
#include "utils/util.h"
#include <ArduinoJson.h>

IPAddress local_ip(192, 168, 4, 1); // Set your server's fixed IP address here
IPAddress gateway(192, 168, 4, 1);  // Set your network Gateway usually your Router base address
IPAddress subnet(255, 255, 255, 0); // Set your network sub-network mask here
IPAddress dns(192, 168, 3, 2);      // Set your network DNS usually your Router base address

NetWork *getNetWorkIns() {
    static NetWork *instance;
    if (instance == NULL) {
        instance = new NetWork();
    }
    return instance;
}

NetWork::NetWork() {
    this->host_name = "holo-zyg";
    WiFi.mode(WIFI_MODE_APSTA);
}

void NetWork::scan_wifi() {
    WiFi.scanNetworks(true, true);
    Serial.println("搜索 wifi...");
}


void NetWork::connectWifi(const char *ssid, const char *password) {
    WiFi.enableSTA(true);
//    WiFi.setHostname(this->host_name);
    Serial.printf("%s Connecting... \n", ssid);
    WiFi.begin(ssid, password);
}

boolean NetWork::disconnectWifi() {
    WiFi.disconnect();
    Serial.println(F("WiFi disconnect"));
   return WiFi.status() == WL_DISCONNECTED;
}

void NetWork::open_ap() {
    WiFi.enableAP(true); //配置为AP模式
    // 修改主机名
    WiFi.setHostname(this->ap_ssid);
    boolean result = WiFi.softAP(ap_ssid, ap_pwd); //开启WIFI热点
    if (result) {
        WiFi.softAPConfig(local_ip, gateway, subnet);
        IPAddress myIP = WiFi.softAPIP();
        //打印相关信息
        Serial.print(F("\nSoft-AP IP address = "));
        Serial.println(myIP);
        Serial.println(String("MAC address = ") + WiFi.softAPmacAddress().c_str());
    } else {
        //开启热点失败
        Serial.println(F("WiFiAP Failed"));
    }
}

String NetWork::getWifiStatus() {
    Serial.println("wifi status");
    String jsonData;
    StaticJsonDocument<256> doc;

    if (WiFi.status() == WL_CONNECTED) {
        String ip("IP 地址: ");
        ip = ip + WiFi.localIP().toString();
        String subnetMask("子网掩码: ");
        subnetMask += WiFi.subnetMask().toString();
        String gateway("网关地址: ");
        gateway += WiFi.gatewayIP().toString();
        String dns("DNS地址: ");
        dns += WiFi.dnsIP().toString();

        String mac("mac 地址: ");
        mac += WiFi.macAddress();
        String hostName("主机名: ");
        hostName += WiFi.getHostname();


        m_log("WIFI connected!", ip.c_str(), subnetMask.c_str(), gateway.c_str(), dns.c_str(), mac.c_str(),
              hostName.c_str(), NULL);
    }
    serializeJson(doc, jsonData);
    return jsonData;
}

String NetWork::getSearchedWifi() {
    String dataJson;
    StaticJsonDocument<512> doc;
    int16_t wifi_num = WiFi.scanComplete();

    String msg;
    String numInfo = "共搜索到" + String(wifi_num) + "个热点";
    doc["totle"] = wifi_num;
    JsonArray wifi = doc.createNestedArray("wifi");

    if (wifi_num > 0) {
        for (int cnt = 0; cnt < wifi_num; cnt++) {
            JsonObject wifi_i = wifi.createNestedObject();
            wifi_i["name"] = WiFi.SSID(cnt);;
            wifi_i["db"] = WiFi.RSSI(cnt);
            wifi_i["pwd"] = WiFi.encryptionType(cnt) == WIFI_AUTH_OPEN ? false : true;
        }
    }
    serializeJsonPretty(doc, dataJson);
    m_log("WifiSearch", numInfo.c_str(), dataJson.c_str(), NULL);
    return dataJson;
}

