#include "ui/font/font.hpp"
#include "font_data.hpp"

namespace ui
{
    Font::Font(std::shared_ptr<FontData> data)
        : data_(data)
    {
    }

    bool Font::Valid() const noexcept
    {
        return static_cast<bool>(data_);
    }

    int32_t Font::Size() const noexcept
    {
        return data_ ? data_->Descriptor().size : 0;
    }

    int32_t Font::Weight() const noexcept
    {
        return data_ ? data_->Descriptor().weight : 0;
    }
}