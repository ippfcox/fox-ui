#pragma once

#include <memory>
#include <string>
#include "ui/font/font_descriptor.hpp"
#include "ui/font/font_backend.hpp"

namespace ui
{
    class FontData
    {
    public:
        FontData(FontDescriptor descriptor,
            std::shared_ptr<FontBackend> backend,
            std::unique_ptr<FontResource> resource);
        ~FontData();

        FontData(const FontData &) = delete;
        FontData &operator=(const FontData &) = delete;

        const FontDescriptor &Descriptor() const noexcept;
        FontResource *Resource() const noexcept;

    private:
        FontDescriptor descriptor_;
        std::shared_ptr<FontBackend> backend_;
        std::unique_ptr<FontResource> resource_;
    };
}