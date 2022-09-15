//
// Created by acer-zgy on 2022/9/14.
//

#include <WiFiClient.h>
#include <WiFi.h>
#include "mqttClient.h"

MqttClient mqttClient;

WiFiClient espClient;
const char *mqtt_server = "zgy.ink";
char *client_id = "zgy_yy-12345";

MqttcbFU mqttFun[10];
int usedLen = 0;

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);   // 打印主题信息
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]); // 打印主题内容
    }
    Serial.println();
    for (int i = 0; i < usedLen; i++) {
//        Serial.println(i);
        if (strcmp((char *) topic, mqttFun[i].subscribeName) == 0) {
            mqttFun[i].fun(payload, length);
        }
    }
}


void MqttClient::init() {
    client = new PubSubClient(mqtt_server, 1883, espClient);
    client->setCallback(callback);
    for (int i = 0; i < usedLen; ++i) {
        client->subscribe(mqttFun[i].subscribeName);
    }
}


void MqttClient::reconnect() {
    if (WiFi.status() == WL_CONNECTED) {
        if (!client->connected()) {
            Serial.print("Attempting MQTT connection...\n");
            // Attempt to connect
            if (client->connect(client_id)) {
                Serial.println("mqtt connected\n");
                // 连接成功时订阅主题
                for (int i = 0; i < usedLen; ++i) {
                    client->subscribe(mqttFun[i].subscribeName);
                }
            } else {
                Serial.print("failed, rc=");
                Serial.print(client->state());
                Serial.println(" try again in next seconds\n");
                // Wait 5 seconds before retrying
            }
        }
    }
}

void MqttClient::loop() {
    client->loop();
    this->reconnect();
}

//MqttCallBackF *events = (MqttCallBackF *) malloc(sizeof(MqttCallBackF) * totalSize);


void MqttClient::addReceiveCallback(char *name, MqttCallFun fun) {
    MqttcbFU newFun;
    newFun.subscribeName = name;
    newFun.fun = fun;


    mqttFun[usedLen++] = newFun;

    if (client) {
        client->subscribe(name);
    }

}
