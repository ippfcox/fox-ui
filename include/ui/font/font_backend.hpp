#pragma once

#include <memory>
#include "font_descriptor.hpp"

namespace ui
{
    class FontResource
    {
    public:
        virtual ~FontResource() = default;
    };

    class FontBackend
    {
    public:
        virtual ~FontBackend() = default;

        virtual std::unique_ptr<FontResource> Create(const std::string &path, const FontDescriptor &descriptor) = 0;
    };
}