#include "lvgl_font_resource.hpp"

namespace ui
{
    LvglFontResource::LvglFontResource(lv_font_t *font)
        : font_(font)
    {
    }

    LvglFontResource::~LvglFontResource()
    {
    }

    lv_font_t *LvglFontResource::NativeFont() const noexcept
    {
        return font_;
    }
}