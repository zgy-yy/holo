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

DynamicJsonDocument doc(2048);

void NetWork::registerWifiEvent(AsyncWebSocket *ws) {
//连接到热点获取ip 事件
    WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
        m_log("连接到 wifi", WiFi.SSID().c_str(), IPAddress(info.connected.ssid).toString().c_str(), NULL);
    }, WiFiEvent_t::SYSTEM_EVENT_STA_GOT_IP);
    //扫描热点结束
    WiFi.onEvent([this, ws](WiFiEvent_t event, WiFiEventInfo_t info) {
        String outJson;
        String wifiList = this->getSearchedWifi();
        Serial.println("搜索结束");
        doc["type"] = 0;
        doc["name"] = "search";
        doc["state"] = "ok";
        doc["data"] = wifiList.c_str();
        serializeJson(doc, outJson);
        doc.clear();
        ws->textAll(outJson.c_str());
    }, WiFiEvent_t::SYSTEM_EVENT_SCAN_DONE);

//断开连接事件
    WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
        m_log("断开 wifi", WiFi.SSID().c_str(), NULL);
    }, WiFiEvent_t::SYSTEM_EVENT_STA_DISCONNECTED);
}


void NetWork::scan_wifi() {
    WiFi.scanNetworks(true, true);
    Serial.println("搜索 wifi...");
}


void NetWork::connectWifi(const char *ssid, const char *password) {
//    WiFi.enableSTA(true);
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
    doc.clear();
    return jsonData;
}

String NetWork::getSearchedWifi() {
    String dataJson;
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
    dataJson = "";
    serializeJson(doc, dataJson);
    doc.clear();
    return dataJson;
}



