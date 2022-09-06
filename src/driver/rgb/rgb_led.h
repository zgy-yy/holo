//
// Created by zgy on 2022/8/27.
//

#ifndef M_HOLO_RGB_LED_H
#define M_HOLO_RGB_LED_H
#include <FastLED.h>

#define RGB_LED_NUM 1
#define RGB_LED_PIN 27

#define LED_MODE_RGB 0
#define LED_MODE_HSV 1

class Pixel
{
private:
    CRGB rgb_buffers[RGB_LED_NUM];

public:
    void init();

    Pixel &setRGB(int r, int g, int b);

    Pixel &setHVS(uint8_t ih, uint8_t is, uint8_t iv);

    Pixel &fill_rainbow(int min_r, int max_r,
                        int min_g, int max_g,
                        int min_b, int max_b);

    Pixel &setBrightness(float duty);
};
#endif //M_HOLO_RGB_LED_H
