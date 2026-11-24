#include "spdlog/spdlog.h"
#include "lvgl.h"

static void init_font_engien()
{
    lv_freetype_init(256);
}

static lv_font_t *load_font(const std::string &font_path, uint32_t size, int32_t weight)
{
    lv_font_info_t info;
    lv_freetype_init_font_info(&info);
    info.name = font_path.c_str();
    info.size = size;
    info.render_mode = LV_FREETYPE_FONT_RENDER_MODE_BITMAP;
    info.weight = weight;
    info.kerning = LV_FONT_KERNING_NORMAL;

    return lv_freetype_font_create_with_info(&info);
}

static void unload_font(lv_font_t *font)
{
    if (font)
        lv_freetype_font_delete(font);
}

struct Context
{
    lv_obj_t *dd_font_name{nullptr};
    lv_obj_t *sl_font_size{nullptr};
    lv_obj_t *sl_font_weight{nullptr};
    lv_obj_t *lbl_test{nullptr};
    lv_font_t *font{nullptr};
};

int main()
{
    lv_init();

    lv_display_t *disp = lv_x11_window_create("Font Test", 400, 300);
    lv_x11_inputs_create(disp, nullptr);
    lv_obj_t *screen = lv_screen_active();
    lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_flex_main_place(screen, LV_FLEX_ALIGN_START, 0);
    lv_obj_set_style_flex_cross_place(screen, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_flex_track_place(screen, LV_FLEX_ALIGN_CENTER, 0);

    lv_obj_t *dd_font_name = lv_dropdown_create(screen);
    lv_dropdown_set_options(dd_font_name,
        "NotoColorEmoji-Regular.ttf\n"
        "NotoSansKR-VariableFont_wght.ttf\n"
        "Roboto-Italic-VariableFont_wdth,wght.ttf\n"
        "NotoSans-Italic-VariableFont_wdth,wght.ttf\n"
        "NotoSansSC-VariableFont_wght.ttf\n"
        "Roboto-VariableFont_wdth,wght.ttf\n"
        "NotoSansJP-VariableFont_wght.ttf\n"
        "NotoSans-VariableFont_wdth,wght.ttf\n");
    lv_obj_set_width(dd_font_name, LV_SIZE_CONTENT);

    lv_obj_t *sl_font_size = lv_slider_create(screen);
    lv_slider_set_min_value(sl_font_size, 0);
    lv_slider_set_max_value(sl_font_size, 256);
    lv_slider_set_start_value(sl_font_size, 24, false);

    lv_obj_t *sl_font_weight = lv_slider_create(screen);
    lv_slider_set_min_value(sl_font_weight, 0);
    lv_slider_set_max_value(sl_font_weight, 1500);
    lv_slider_set_start_value(sl_font_weight, 300, false);

    lv_obj_t *btn_apply = lv_button_create(screen);
    lv_obj_t *lbl_apply = lv_label_create(btn_apply);
    lv_label_set_text(lbl_apply, "Apply");

    lv_obj_t *lbl_test = lv_label_create(screen);
    lv_label_set_text(lbl_test, "Lazy fox");

    auto ctx = new Context;
    ctx->dd_font_name = dd_font_name;
    ctx->sl_font_size = sl_font_size;
    ctx->sl_font_weight = sl_font_weight;
    ctx->lbl_test = lbl_test;

    lv_obj_add_event_cb(btn_apply, [](lv_event_t *e) {
        auto ctx = static_cast<Context *>(lv_event_get_user_data(e));

        if (ctx->font)
        {
            unload_font(ctx->font);
            ctx->font = nullptr;
        }

        std::string font_name;
        font_name.resize(1024);
        lv_dropdown_get_selected_str(ctx->dd_font_name, font_name.data(), font_name.size());

        auto font_size = lv_slider_get_value(ctx->sl_font_size);
        auto font_weight = lv_slider_get_value(ctx->sl_font_weight);

        std::string font_path = "/home/jiangdongchao/workdir/opensource/fox-ui/src/material/font/" + font_name;
        ctx->font = load_font(font_path.c_str(), font_size, font_weight);


        lv_obj_set_style_text_font(ctx->lbl_test, ctx->font, 0); }, LV_EVENT_CLICKED, ctx);

    while (true)
    {
        lv_timer_handler();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}