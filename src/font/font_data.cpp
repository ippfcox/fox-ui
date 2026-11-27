#include "font_data.hpp"

namespace ui
{
    FontData::FontData(FontDescriptor descriptor,
        std::shared_ptr<FontBackend> backend,
        std::unique_ptr<FontResource> resource)
        : descriptor_(descriptor), backend_(backend), resource_(std::move(resource))
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