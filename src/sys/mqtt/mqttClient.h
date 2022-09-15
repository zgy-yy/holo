//
// Created by acer-zgy on 2022/9/14.
//

#ifndef M_HOLO_MQTTCLIENT_H
#define M_HOLO_MQTTCLIENT_H

#include "PubSubClient.h"

typedef std::function<void(byte *payload, unsigned int length)> MqttCallFun;

 struct MqttcbFU {
    char *subscribeName;
    MqttCallFun fun;
} ;

class MqttClient {
private:
    PubSubClient *client;
public:
    void init();

//   保持心跳
    void loop();

    void reconnect();

    void addReceiveCallback(char *name, MqttCallFun fun);
};

extern MqttClient mqttClient;
#endif //M_HOLO_MQTTCLIENT_H
