//
// Created by zgy on 2022/8/28.
//

#include "display.h"
#include <TFT_eSPI.h>

#define LCD_BL_PWM_CHANNEL 0
#define LCD_BL_PIN 5
static lv_disp_draw_buf_t draw_buf;    //定义显示器变量
static lv_color_t buf[TFT_WIDTH * 10]; //定义刷新缓存
TFT_eSPI *tft = new TFT_eSPI(240, 240);


/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    tft->setAddrWindow(area->x1, area->y1, w, h);             //设置填充区域
    tft->startWrite();                                        //使能写功能
    tft->pushColors((uint16_t *) &color_p->full, w * h, true); //写入颜色缓存和缓存大小
    tft->endWrite();                                          //关闭写功能

    lv_disp_flush_ready(disp); //调用区域填充颜色函数
}

void Display::init(uint8_t rotation, uint8_t backLight) {
    ledcSetup(LCD_BL_PWM_CHANNEL, 5000, 8);
    ledcAttachPin(LCD_BL_PIN, LCD_BL_PWM_CHANNEL);

    lv_init();
    //lv_log_register_print_cb();
    tft->begin(); /* TFT init */
    tft->fillScreen(TFT_WHITE);
    tft->writecommand(ST7789_DISPON); // Display on
    tft->setRotation(rotation);              /* mirror 修改反转，如果加上分光棱镜需要改为4镜像*/
    setBackLight(backLight / 100.0);  // 设置亮度
    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, TFT_WIDTH * 10);

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = TFT_WIDTH;
    disp_drv.ver_res = TFT_HEIGHT;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);
}

//void IRAM_ATTR onTimer() {
//
//}

void Display::routine(void (*fun)()) {
//    delay(5);
    //  lv_timer_handler(); /* let the GUI do its work */
//    hw_timer_t *timer = timerBegin(0, 80, true);
//    timerAttachInterrupt(timer, &onTimer, true);
//    timerAlarmWrite(timer, 1000000, true);
//    timerAlarmEnable(timer);
    long last_time = millis();
    fun();
//    delay(50);
    lv_task_handler();
    lv_tick_inc(int(millis() - last_time));
}

void Display::setBackLight(float duty) {
    duty = constrain(duty, 0, 1);
    duty = 1 - duty;
    ledcWrite(LCD_BL_PWM_CHANNEL, (int) (duty * 255));
}
