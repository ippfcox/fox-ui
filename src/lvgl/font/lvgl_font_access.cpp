#include "lvgl_font_access.hpp"
#include "../../font/font_data.hpp"

namespace ui
{
    lv_font_t *LvglFontAccess::NavtiveFont(const Font &font) noexcept
    {
        if (!font.data_)
            return nullptr;

        auto *resource = dynamic_cast<LvglFontResource *>(font.data_->Resource());

        return resource ? resource->NativeFont() : nullptr;
    }
}