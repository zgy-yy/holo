//
// Created by zgy on 2022/9/2.
//

#include "webServer.h"
#include <Arduino.h>


static uint8_t usedlen = 0;
static uint8_t size = 2;
MEvent *events = (MEvent *) malloc(sizeof(MEvent) * size);

void wsOnEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data,
               size_t len) {
    if (type == WS_EVT_CONNECT) {
        Serial.print("ws客户连接\n");
    } else if (type == WS_EVT_DATA) {
        AwsFrameInfo *info = (AwsFrameInfo *) arg;
        Serial.printf("ws_url[%s] clientID[%u] frame[%u] %s[%llu - %llu]-> ", server->url(), client->id(), info->num,
                      (info->message_opcode == WS_TEXT) ? "text" : "binary", info->index, info->index + len);
        data[len] = 0;
        Serial.printf("data:%s\n", (char *) data);
        for (int i = 0; i < usedlen; i++) {
            if (strcmp((char *) data, events[i].eventName) == 0) {
                events[i].eventHandler(server, client, type, arg, data, len);
            }
        }
    }
}


WebServer::WebServer() {
    httpServer = new AsyncWebServer(port);
    ws = new AsyncWebSocket("/ws");

    netWork = getNetWorkIns();
    //    注册wifi 事件
    netWork->registerWifiEvent(this->ws);
    netWork->open_ap();
//
//    netWork->connectWifi("holo", "12345678");
//    netWork->connectWifi("gy的iPhone", "12345678");
    netWork->connectWifi("CQUPT", "");
    startHttpServer();
    startWebSocket();
}

WebServer::WebServer(int port) {
    this->port = port;
    WebServer();
}

void WebServer::startHttpServer() {
    serverEventsInit();
    httpServer->begin();
}

void WebServer::startWebSocket() {
    ws->onEvent(wsOnEvent);
    httpServer->addHandler(ws);
}

void WebServer::addSocketEvent(char *eventName,
                               void (*eventHandler)(AsyncWebSocket *server, AsyncWebSocketClient *client,
                                                    AwsEventType type, void *arg, uint8_t *data,
                                                    size_t len)) {
    MEvent newEvent;
    newEvent.eventName = eventName;
    newEvent.eventHandler = eventHandler;

    if (usedlen == size) {
        MEvent *oldEvents = events;
        size += 3;
        events = (MEvent *) malloc(sizeof(MEvent) * size);

        for (uint8_t i = 0; i < usedlen; ++i) {
            events[i] = oldEvents[i];
        }
        free(oldEvents);
    }
    events[usedlen] = newEvent;
    usedlen += 1;
}

//

void WebServer::serverEventsInit() {
    //httpServer;
    NetWork *wifi = netWork;

    httpServer->on("/wifi/status", HTTP_GET, [wifi](AsyncWebServerRequest *request) {
        String status = wifi->getWifiStatus();
        request->send(200, "text/plain", status);
    });

    httpServer->on("/wifi/connect", HTTP_GET, [wifi](AsyncWebServerRequest *request) {
        request->getParam(0);
        int params = request->params();
        AsyncWebParameter* name;
        AsyncWebParameter* pwd;
        if(request->hasParam("ssid"))
            name = request->getParam("ssid");
        if(request->hasParam("pwd"))
            pwd = request->getParam("pwd");
//        Serial.printf("%s,%s\n",name->value().c_str(),pwd->value().c_str());
        wifi->connectWifi(name->value().c_str(),pwd->value().c_str());
        request->send(200, "text/plain", "");
    });


    //websocket Server;
    /*
     * wifi 相关事件
     */
    addSocketEvent("scanWifi",
                   [](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data,
                      size_t len) {
                       NetWork *net = getNetWorkIns();
                       net->scan_wifi();
                   });
    addSocketEvent("disconnectWifi",
                   [](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data,
                      size_t len) {
                       NetWork *net = getNetWorkIns();
                       boolean ok = net->disconnectWifi();
//                       server->text(client->id(), String(ok));
                   });
}
