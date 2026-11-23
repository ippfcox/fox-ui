#include "spdlog/spdlog.h"
#include "lvgl.h"

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

    lv_font_manager_t *fm = lv_font_manager_create(8);
    if (!fm)
    {
        SPDLOG_ERROR("lv_font_manager_create failed");
        return -1;
    }

    
}