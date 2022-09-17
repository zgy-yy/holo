//
// Created by acer-zgy on 2022/9/16.
//

#include "heart.h"
#include "sys/mqtt/mqttClient.h"
#include "images/archerS_0000.h"
#include "images/archerS_0001.h"
#include "images/archerS_0002.h"
#include "images/archerS_0003.h"
#include "images/archerS_0004.h"
#include "images/archerS_0005.h"
#include "images/archerS_0006.h"
#include "images/archerS_0007.h"
#include "images/archerS_0008.h"
#include "images/archerR_0000.h"
#include "images/archerR_0001.h"
#include "images/archerR_0002.h"
#include "images/archerR_0003.h"
#include "images/archerR_0004.h"
#include "images/archerR_0005.h"
#include "images/archerR_0006.h"
#include "images/archerR_0007.h"
#include "images/archerR_0008.h"
#include "images/archerR_0009.h"
#include "images/heart_0000.h"
#include "images/heart_0001.h"
#include "images/heart_0002.h"
#include "images/heart_0003.h"
#include "images/heart_0004.h"
#include "images/heart_0005.h"
#include "images/heart_0006.h"
#include "images/heart_0007.h"
#include "images/heart_0008.h"
#include "images/heart_0009.h"
#include "images/heart_0010.h"
#include "images/heart_0011.h"
#include "images/heart_0012.h"
#include "images/heart_0013.h"
#include "images/heart_0014.h"
#include "images/heart_0015.h"
#include "images/heart_0016.h"
#include "images/heart_0017.h"
#include "images/heart_0018.h"
#include "images/heart_0019.h"

static lv_obj_t *heartbeat_Screen = NULL;
lv_obj_t *heartbeatImg = NULL;

const void *archerS_map[] = {&archerS_0000, &archerS_0001, &archerS_0002, &archerS_0003, &archerS_0004,
                             &archerS_0005, &archerS_0006, &archerS_0007, &archerS_0008};

const void *archerR_map[] = {&archerR_0000, &archerR_0001, &archerR_0002, &archerR_0003, &archerR_0004,
                             &archerR_0005, &archerR_0006, &archerR_0007, &archerR_0008, &archerR_0009};

const void *heart_map[] = {&heart_0000, &heart_0001, &heart_0002, &heart_0003, &heart_0004,
                           &heart_0005, &heart_0006, &heart_0007, &heart_0009, &heart_0008, &heart_0010, &heart_0011,
                           &heart_0012, &heart_0013, &heart_0014,
                           &heart_0015, &heart_0016, &heart_0017, &heart_0018, &heart_0019};


boolean isRec = false;//接收状态
unsigned int receivedTime = 0;//接收到持续时间
Heart::Heart() {
    this->app_name = "HEART";

    mqttClient.addReceiveCallback("/yy_love", [](byte *payload, unsigned int length) {
        payload[length] = '\0';
        showMe("HEART");
        isRec = true;
    });
}

void Gui() {
    heartbeat_Screen = lv_scr_act();
    static lv_style_t screenStyle;
    lv_style_init(&screenStyle);
    lv_style_set_bg_color(&screenStyle, lv_color_black());
    lv_obj_add_style(heartbeat_Screen, &screenStyle, LV_STATE_DEFAULT);
    heartbeatImg = lv_img_create(heartbeat_Screen); //创建heart图标
    lv_obj_align(heartbeatImg, LV_ALIGN_CENTER, 0, 0);
}

void playSend() {
    static int i = 0;
    lv_img_set_src(heartbeatImg, archerS_map[i]);
    i++;
    if (i == 9) {
        i = 0;
    }

}

void playRed() {
    static int i = 0;
    lv_img_set_src(heartbeatImg, archerR_map[i]);
    i++;
    if (i == 10) {
        i = 0;
    }
}

void playTogether() {
    static int i = 0;
    lv_img_set_src(heartbeatImg, heart_map[i]);
    i++;
    if (i == 20) {
        i = 0;
    }

}

void Heart::setup() {
    Gui();
}

int sendTime = 0;

void Heart::loop(int active) {
    Serial.println(sendTime);
    if (receivedTime == 100 * 2) {//6s
        receivedTime = 0;
        isRec = false;
    }
    if (sendTime > 100 * 2) {//3s
        sendTime = 0;
    }
    if (isRec) {
        receivedTime++;
    }

    if (active == 0 || active == 2) {
        //            发送
        receivedTime = 0;
        playSend();
        sendTime++;
    } else if (sendTime > 30 && receivedTime > 0) {
        playTogether();
    } else if (isRec && receivedTime > 0) {
        playRed();
    } else {
        lv_img_set_src(heartbeatImg, archerS_map[0]);
    }

    delay(30);
}

void Heart::exit() {
    lv_obj_clean(heartbeat_Screen);
    heartbeat_Screen = NULL;
    isRec = false;
    receivedTime = 0;
    sendTime = 0;
}
