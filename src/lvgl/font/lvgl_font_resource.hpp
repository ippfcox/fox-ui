#pragma once

#include "ui/font/font_backend.hpp"
#include "lvgl/lvgl.h"

namespace ui
{
    class LvglFontResource : public ui::FontResource
    {
    public:
        explicit LvglFontResource(lv_font_t *font);
        ~LvglFontResource() override;

        LvglFontResource(const LvglFontResource &) = delete;
        LvglFontResource &operator=(const LvglFontResource &) = delete;

        lv_font_t *NativeFont() const noexcept;

    private:
        lv_font_t *font_{nullptr};
    };
}