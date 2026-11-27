#pragma once

#include <string>

namespace ui
{
    enum class FontStyle
    {
        None,
        Normal,
        Italic,
    };

    struct FontDescriptor
    {
        std::string family;
        int32_t size{16};
        int32_t weight{400};
        FontStyle style{FontStyle::Normal};
        // FontRenderMode render_mode;
        // FontKerning kerning;
        bool operator==(const FontDescriptor &rhs) const noexcept
        {
            return family == rhs.family &&
                   size == rhs.size &&
                   weight == rhs.weight &&
                   style == rhs.style;
        }
    };

    struct FontDescriptorHash
    {
        size_t operator()(const FontDescriptor &d) const noexcept
        {
            size_t seed = std::hash<std::string>{}(d.family);
            auto combine = [&seed](size_t value) { seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2); };

            combine(std::hash<int32_t>{}(d.size));
            combine(std::hash<int32_t>{}(d.weight));
            combine(std::hash<int>{}(static_cast<int>(d.style)));

            return seed;
        }
    };
}