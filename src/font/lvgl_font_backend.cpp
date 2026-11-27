#include "lvgl_font_backend.hpp"
#include "lvgl_font_resource.hpp"

namespace ui
{
    std::unique_ptr<FontResource> FontBackend::Create(const std::string &path, const FontDescriptor &descriptor)
    {
        lv_font_info_t info;
        lv_freetype_init_font_info(&info);

        info.name = path.c_str();
        info.size = static_cast<uint32_t>(descriptor.size);
        info.weight = descriptor.weight;

        info.render_mode = LV_FREETYPE_FONT_RENDER_MODE_BITMAP;

        info.style = descriptor.style == FontStyle::Italic
                         ? LV_FREETYPE_FONT_STYLE_ITALIC
                         : LV_FREETYPE_FONT_STYLE_NORMAL;

        lv_font_t *font = lv_freetype_font_create_with_info(&info);
        if (!font)
            return nullptr;

        return std::make_unique<LvglFontResource>(font);
    }
}