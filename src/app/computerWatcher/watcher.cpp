//
// Created by acer-zgy on 2022/9/15.
//

#include "watcher.h"
#include "sys/mqtt/mqttClient.h"

LV_FONT_DECLARE(w_fot);


lv_obj_t *value_cpu;
lv_obj_t *value_mem;
lv_obj_t *bar_cpu;
lv_obj_t *bar_mem;

Watcher::Watcher() {
    this->app_name = "WATCHER";

    mqttClient.addReceiveCallback("/cpInfo", [](byte *payload, unsigned int length) {
        payload[length] = '\0';
        String res((char *) payload);
        float cpu = atof(res.substring(0, 4).c_str());
        float useMem = atof(res.substring(6, 10).c_str());
        float totalMem = atof(res.substring(12, 16).c_str());

        if (value_cpu) {
            lv_label_set_text(value_cpu, (res.substring(0, 4)+"%").c_str());
            lv_bar_set_value(bar_cpu, cpu, LV_ANIM_ON);
        }
        if (value_cpu) {
            lv_label_set_text(value_mem, (String(useMem / totalMem*100)+"%").c_str());
            lv_bar_set_value(bar_mem, useMem / totalMem*100, LV_ANIM_ON);
        }
        Serial.printf("%f,%f,%f\n", cpu, totalMem, useMem);

//
        Serial.printf("%s\n", payload);
    });
}

lv_obj_t *watcher_screen;

void app_gui() {
    watcher_screen = lv_scr_act();

    lv_obj_set_style_bg_color(watcher_screen,lv_color_black(),LV_PART_MAIN);

    static lv_style_t boxStyle;
    lv_style_init(&boxStyle);
    lv_style_set_bg_color(&boxStyle,lv_color_black());
    lv_style_set_pad_all(&boxStyle, 0);
    lv_style_set_border_width(&boxStyle, 0);
    lv_style_set_text_color(&boxStyle,lv_color_white());

    lv_obj_t *cpubox = lv_obj_create(watcher_screen);
    lv_obj_add_style(cpubox, &boxStyle, LV_PART_MAIN);
    lv_obj_set_height(cpubox, lv_pct(100));
    lv_obj_set_width(cpubox, lv_pct(50));

    static lv_style_t style_indic;
    lv_style_init(&style_indic);
    lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_bg_grad_color(&style_indic, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_bg_grad_dir(&style_indic, LV_GRAD_DIR_VER);

//    static lv_style_t textStyle;
//    lv_style_init(&textStyle);
//    lv_style_set_text_color(&textStyle,lv_color_white());
//cpu
    bar_cpu = lv_bar_create(cpubox);
    lv_obj_set_x(bar_cpu, 29);
    lv_obj_set_y(bar_cpu, 56);
    lv_obj_add_style(bar_cpu, &style_indic, LV_PART_INDICATOR);
    lv_obj_set_size(bar_cpu, 20, 160);

    lv_bar_set_range(bar_cpu, 0, 100);
    lv_bar_set_value(bar_cpu, 12, LV_ANIM_ON);

    lv_obj_t *label_cpu = lv_obj_create(cpubox);
    lv_obj_set_flex_flow(label_cpu, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_x(label_cpu, 56);
    lv_obj_set_y(label_cpu, 156);
    lv_obj_set_size(label_cpu, 60, 50);
    lv_obj_add_style(label_cpu, &boxStyle, LV_PART_MAIN);
    lv_obj_t *name_cpu = lv_label_create(label_cpu);
    lv_obj_set_style_text_font(name_cpu, &w_fot, LV_PART_MAIN);
    lv_label_set_text(name_cpu, "cpu");
    value_cpu = lv_label_create(label_cpu);
    lv_obj_set_style_text_font(value_cpu, &w_fot, LV_PART_MAIN);
    lv_label_set_text(value_cpu, "12.23%");

//memory
    lv_obj_t *membox = lv_obj_create(watcher_screen);
    lv_obj_add_style(membox, &boxStyle, LV_PART_MAIN);
    lv_obj_set_height(membox, lv_pct(100));
    lv_obj_set_width(membox, lv_pct(52));
    lv_obj_set_x(membox, lv_pct(46));

    bar_mem = lv_bar_create(membox);
    lv_obj_add_style(bar_mem, &style_indic, LV_PART_INDICATOR);
    lv_obj_set_size(bar_mem, 20, 160);
    lv_obj_set_x(bar_mem, 29);
    lv_obj_set_y(bar_mem, 56);
//    lv_obj_center(bar_mem);
    lv_bar_set_range(bar_mem, 0, 100);
    lv_bar_set_value(bar_mem, 56, LV_ANIM_ON);

    lv_obj_t *label_mem = lv_obj_create(membox);
    lv_obj_set_flex_flow(label_mem, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_x(label_mem, 56);
    lv_obj_set_y(label_mem, 156);
    lv_obj_set_size(label_mem, 68, 50);
    lv_obj_add_style(label_mem, &boxStyle, LV_PART_MAIN);
    lv_obj_t *name_mem = lv_label_create(label_mem);
    lv_obj_set_style_text_font(name_mem, &w_fot, LV_PART_MAIN);
    lv_label_set_text(name_mem, "memory");
    value_mem = lv_label_create(label_mem);
    lv_obj_set_style_text_font(value_mem, &w_fot, LV_PART_MAIN);
    lv_label_set_text(value_mem, "12.23%");

//    lv_obj_add_style()
}

void Watcher::setup() {
    app_gui();
}

void Watcher::loop(int active) {

}

void Watcher::exit() {
    lv_obj_clean(watcher_screen);
}
