#pragma once

#include "ui/font/font.hpp"
#include "lvgl_font_resource.hpp"

namespace ui
{
    class LvglFontAccess
    {
    public:
        static lv_font_t *NavtiveFont(const Font &font) noexcept;
    };
}