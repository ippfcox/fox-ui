#include "spdlog/spdlog.h"
#include "lvgl.h"

int main()
{
    lv_init();

    lv_display_t *disp = lv_x11_window_create("Helloworld2", 800, 600);
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

    lv_obj_t *label = lv_label_create(screen);
    lv_label_set_text(label, "Hello World");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    while (true)
    {
        lv_timer_handler();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}