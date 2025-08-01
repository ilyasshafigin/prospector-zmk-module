#include <lvgl.h>

#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/kernel.h>

#include "widgets/battery_bar.h"
#include "widgets/caps_word_indicator.h"
#include "widgets/layer_roller.h"
#ifdef CONFIG_PROSPECTOR_MODIFIERS_WIDGET
#include "widgets/modifiers.h"
#endif

#include <fonts.h>
#include <sf_symbols.h>

#include <zmk/keymap.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static struct zmk_widget_layer_roller layer_roller_widget;
static struct zmk_widget_battery_bar battery_bar_widget;
static struct zmk_widget_caps_word_indicator caps_word_indicator_widget;
#ifdef CONFIG_PROSPECTOR_MODIFIERS_WIDGET
static struct zmk_widget_modifiers modifiers_widget;
#endif

lv_obj_t* zmk_display_status_screen() {
    lv_obj_t* screen;
    screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(screen, 255, LV_PART_MAIN);

#ifdef CONFIG_DT_HAS_ZMK_BEHAVIOR_CAPS_WORD_ENABLED
#ifdef CONFIG_PROSPECTOR_MODIFIERS_WIDGET
    zmk_widget_caps_word_indicator_init(&caps_word_indicator_widget, screen);
    lv_obj_align(zmk_widget_caps_word_indicator_obj(&caps_word_indicator_widget), LV_ALIGN_BOTTOM_RIGHT, -5, -48);
#else
    zmk_widget_caps_word_indicator_init(&caps_word_indicator_widget, screen);
    lv_obj_align(zmk_widget_caps_word_indicator_obj(&caps_word_indicator_widget), LV_ALIGN_RIGHT_MID, -10, 46);
#endif
#endif

    zmk_widget_battery_bar_init(&battery_bar_widget, screen);
    // lv_obj_set_width(zmk_widget_battery_bar_obj(&battery_bar_widget), lv_pct(100));
    lv_obj_set_size(zmk_widget_battery_bar_obj(&battery_bar_widget), lv_pct(100), 48);
    lv_obj_align(zmk_widget_battery_bar_obj(&battery_bar_widget), LV_ALIGN_BOTTOM_MID, 0, 0);

    zmk_widget_layer_roller_init(&layer_roller_widget, screen);
    lv_obj_set_size(zmk_widget_layer_roller_obj(&layer_roller_widget), 224, 140);
    lv_obj_align(zmk_widget_layer_roller_obj(&layer_roller_widget), LV_ALIGN_LEFT_MID, 0, -20);

#ifdef CONFIG_PROSPECTOR_MODIFIERS_WIDGET
    zmk_widget_modifiers_init(&modifiers_widget, screen);
    lv_obj_align(zmk_widget_modifiers_obj(&modifiers_widget), LV_ALIGN_TOP_RIGHT, -5, 12);
#endif

    return screen;
}
