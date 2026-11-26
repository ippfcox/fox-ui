#include "font_data.hpp"

namespace ui
{
    FontData::FontData(int32_t size, int32_t weight)
        : size_(size), weight_(weight)
    {
    }

    FontData::~FontData()
    {
    }

    int32_t FontData::Size() const noexcept
    {
        return size_;
    }

    int32_t FontData::Weight() const noexcept
    {
        return weight_;
    }
}