#include "spdlog/spdlog.h"
#include "lvgl.h"

static lv_font_manager_t *g_font_manager{nullptr};
static lv_font_t *g_font_24{nullptr};
static lv_font_t *g_font_36{nullptr};
static lv_font_t *g_font_48{nullptr};

static void init_font_manager()
{
    g_font_manager = lv_font_manager_create(8);
    if (!g_font_manager)
    {
        SPDLOG_ERROR("lv_font_manager_create failed");
        return;
    }

    // static std::string font_path = "/home/jiangdongchao/workdir/opensource/fox-ui/src/material/font/NotoSansSC-VariableFont_wght.ttf";
    // static std::string font_path = "/home/jiangdongchao/workdir/opensource/fox-ui/src/material/font/NotoSans-VariableFont_wdth,wght.ttf";
    static std::string font_path = "/home/jiangdongchao/workdir/opensource/fox-ui/src/material/font/NotoSans-Italic-VariableFont_wdth,wght.ttf";

    auto ret = lv_font_manager_add_src_static(g_font_manager, "NotoSansSC", font_path.c_str(), &lv_freetype_font_class);
    if (!ret)
    {
        SPDLOG_ERROR("failed to register NotoSansSC");
        return;
    }

    g_font_24 = lv_font_manager_create_font(g_font_manager, "NotoSansSC", LV_FREETYPE_FONT_RENDER_MODE_BITMAP, 24, LV_FREETYPE_FONT_STYLE_NORMAL, LV_FONT_KERNING_NONE);
    g_font_36 = lv_font_manager_create_font(g_font_manager, "NotoSansSC", LV_FREETYPE_FONT_RENDER_MODE_BITMAP, 36, LV_FREETYPE_FONT_STYLE_NORMAL, LV_FONT_KERNING_NONE);
    g_font_48 = lv_font_manager_create_font(g_font_manager, "NotoSansSC", LV_FREETYPE_FONT_RENDER_MODE_BITMAP, 48, LV_FREETYPE_FONT_STYLE_NORMAL, LV_FONT_KERNING_NONE);

    if (!g_font_24 || !g_font_36 || !g_font_48)
    {
        SPDLOG_ERROR("lv_font_manager_create_font failed");
        return;
    }
}

static lv_obj_t *create_font_label(lv_obj_t *parent, const char *text, const lv_font_t *font, lv_coord_t y)
{
    lv_obj_t *label = lv_label_create(parent);

    lv_label_set_text(label, text);
    lv_obj_set_style_text_font(label, font, LV_PART_MAIN);

    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 30, y);

    return label;
}

int main()
{
    lv_init();

    lv_display_t *disp = lv_x11_window_create("HelloWorld3", 400, 300);
    if (!disp)
    {
        SPDLOG_ERROR("lv_x11_window_create failed");
        return -1;
    }

    lv_x11_inputs_create(disp, nullptr);

    lv_obj_t *screen = lv_screen_active();
    if (!screen)
    {
        SPDLOG_ERROR("lv_screen_active failed");
        return -1;
    }

    init_font_manager();

    create_font_label(screen, "Noto Sans SC - 24px", g_font_24, 30);
    create_font_label(screen, "Noto Sans SC - 36px", g_font_36, 100);
    create_font_label(screen, "Noto Sans SC - 48px", g_font_48, 190);

    while (true)
    {
        lv_timer_handler();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}