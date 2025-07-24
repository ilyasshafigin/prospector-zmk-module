#include "modifiers.h"

#include <zmk/display.h>
#include <zmk/event_manager.h>
#include <zmk/events/keycode_state_changed.h>
#include <zmk/hid.h>
#include <zmk/keys.h>

#include <fonts.h>
#include <sf_symbols.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#define MODIFIER_ACTIVE_COLOR lv_color_hex(0x80e0a7)
#define MODIFIER_INACTIVE_COLOR lv_color_hex(0x101010)

static bool initialized = false;

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

struct modifiers_state {
    zmk_mod_flags_t current_mods;
};

static void modifiers_update_visuals(struct zmk_widget_modifiers* widget,
                                     struct modifiers_state state) {
    if (!initialized) {
        return;
    }

    zmk_mod_flags_t mods = state.current_mods;

    if (mods & (MOD_LSFT | MOD_RSFT)) {
        lv_obj_set_style_text_color(widget->shift_label, MODIFIER_ACTIVE_COLOR, LV_PART_MAIN);
    } else {
        lv_obj_set_style_text_color(widget->shift_label, MODIFIER_INACTIVE_COLOR, LV_PART_MAIN);
    }

    if (mods & (MOD_LCTL | MOD_RCTL)) {
        lv_obj_set_style_text_color(widget->ctrl_label, MODIFIER_ACTIVE_COLOR, LV_PART_MAIN);
    } else {
        lv_obj_set_style_text_color(widget->ctrl_label, MODIFIER_INACTIVE_COLOR, LV_PART_MAIN);
    }

    if (mods & (MOD_LALT | MOD_RALT)) {
        lv_obj_set_style_text_color(widget->opt_label, MODIFIER_ACTIVE_COLOR, LV_PART_MAIN);
    } else {
        lv_obj_set_style_text_color(widget->opt_label, MODIFIER_INACTIVE_COLOR, LV_PART_MAIN);
    }

    if (mods & (MOD_LGUI | MOD_RGUI)) {
        lv_obj_set_style_text_color(widget->cmd_label, MODIFIER_ACTIVE_COLOR, LV_PART_MAIN);
    } else {
        lv_obj_set_style_text_color(widget->cmd_label, MODIFIER_INACTIVE_COLOR, LV_PART_MAIN);
    }
}

static void modifiers_update_cb(struct modifiers_state state) {
    struct zmk_widget_modifiers* widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
        modifiers_update_visuals(widget, state);
    }
}

static struct modifiers_state modifiers_get_state(const zmk_event_t* eh) {
    zmk_mod_flags_t current_mods = zmk_hid_get_explicit_mods();
    return (struct modifiers_state) { .current_mods = current_mods };
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_modifiers, struct modifiers_state,
                            modifiers_update_cb, modifiers_get_state);
ZMK_SUBSCRIPTION(widget_modifiers, zmk_keycode_state_changed);

int zmk_widget_modifiers_init(struct zmk_widget_modifiers* widget, lv_obj_t* parent) {
    widget->obj = lv_obj_create(parent);
    lv_obj_remove_style_all(widget->obj);
    lv_obj_set_size(widget->obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_layout(widget->obj, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(widget->obj, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_column(widget->obj, 5, LV_PART_MAIN);

    const lv_font_t* mod_font = &SF_Compact_Text_Bold_32;

    widget->shift_label = lv_label_create(widget->obj);
    lv_label_set_text(widget->shift_label, SF_SYMBOL_SHIFT);
    lv_obj_set_style_text_font(widget->shift_label, mod_font, LV_PART_MAIN);
    lv_obj_set_style_text_color(widget->shift_label, MODIFIER_INACTIVE_COLOR, LV_PART_MAIN);

    widget->ctrl_label = lv_label_create(widget->obj);
    lv_label_set_text(widget->ctrl_label, SF_SYMBOL_CONTROL);
    lv_obj_set_style_text_font(widget->ctrl_label, mod_font, LV_PART_MAIN);
    lv_obj_set_style_text_color(widget->ctrl_label, MODIFIER_INACTIVE_COLOR, LV_PART_MAIN);

    widget->opt_label = lv_label_create(widget->obj);
    lv_label_set_text(widget->opt_label, SF_SYMBOL_OPTION);
    lv_obj_set_style_text_font(widget->opt_label, mod_font, LV_PART_MAIN);
    lv_obj_set_style_text_color(widget->opt_label, MODIFIER_INACTIVE_COLOR, LV_PART_MAIN);

    widget->cmd_label = lv_label_create(widget->obj);
    lv_label_set_text(widget->cmd_label, SF_SYMBOL_COMMAND);
    lv_obj_set_style_text_font(widget->cmd_label, mod_font, LV_PART_MAIN);
    lv_obj_set_style_text_color(widget->cmd_label, MODIFIER_INACTIVE_COLOR, LV_PART_MAIN);

    sys_slist_append(&widgets, &widget->node);

    widget_modifiers_init();
    modifiers_update_cb(modifiers_get_state(NULL));

    initialized = true;

    return 0;
}

lv_obj_t* zmk_widget_modifiers_obj(struct zmk_widget_modifiers* widget) {
    return widget->obj;
}
