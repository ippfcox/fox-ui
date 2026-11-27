#pragma once

#include "ui/font/font_backend.hpp"

namespace ui
{
    class LvglFontBackend final : public ui::FontBackend
    {
        std::unique_ptr<FontResource> Create(const std::string &path, const FontDescriptor &descriptor) override;
    };
}