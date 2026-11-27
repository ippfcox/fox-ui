#include "font_data.hpp"

namespace ui
{
    FontData::FontData(FontDescriptor descriptor,
        std::shared_ptr<FontBackend> backend,
        std::shared_ptr<FontResource> resource)
        : descriptor_(descriptor), backend_(backend), resource_(resource)
    {
    }

    FontData::~FontData()
    {
    }

    const FontDescriptor &FontData::Descriptor() const noexcept
    {
        return descriptor_;
    }
}