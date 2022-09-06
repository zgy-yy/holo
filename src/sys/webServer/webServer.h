//
// Created by zgy on 2022/9/2.
//

#ifndef M_HOLO_WEBSERVER_H
#define M_HOLO_WEBSERVER_H

#include <ESPAsyncWebServer.h>

#include "sys/network/network.h"

typedef std::function<void(AsyncWebServerRequest *request)> ArRequestHandlerFunction;

typedef struct _event {
    char* eventName;

    void
    (*eventHandler)(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data,
                    size_t len);
} MEvent;

class WebServer {
private:
    AsyncWebServer *httpServer;
    AsyncWebSocket *ws;
    int port = 8080;
    NetWork *netWork;

public:
    WebServer();

    WebServer(int port);

    void startHttpServer();

    void startWebSocket();

    void addSocketEvent(char* eventName,
                        void (*eventHandler)(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                                             void *arg, uint8_t *data,
                                             size_t len));

    void addHttpServer(const char *uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest);

    void serverEventsInit();
};

#endif //M_HOLO_WEBSERVER_H
