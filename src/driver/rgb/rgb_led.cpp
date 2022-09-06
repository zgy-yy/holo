//
// Created by zgy on 2022/8/27.
//

#include "rgb_led.h"

void Pixel::init()
{
    FastLED.addLeds<WS2812, RGB_LED_PIN, GRB>(rgb_buffers, RGB_LED_NUM);
    FastLED.setBrightness(200);
}

Pixel &Pixel::setRGB(int r, int g, int b)
{
    rgb_buffers[0] = CRGB(r, g, b);
    FastLED.show();

    return *this;
}

Pixel &Pixel::setHVS(uint8_t ih, uint8_t is, uint8_t iv)
{
    rgb_buffers[0].setHSV(ih, is, iv);
    FastLED.show();

    return *this;
}

Pixel &Pixel::fill_rainbow(int min_r, int max_r,
                           int min_g, int max_g,
                           int min_b, int max_b)
{
    fill_gradient(rgb_buffers, 0, CHSV(50, 255, 255), 29, CHSV(150, 255, 255), SHORTEST_HUES);
    FastLED.show();

    return *this;
}

Pixel &Pixel::setBrightness(float duty)
{
    duty = constrain(duty, 0, 1);
    FastLED.setBrightness((uint8_t)(255 * duty));
    FastLED.show();

    return *this;
}