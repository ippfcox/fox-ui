#include <memory>
#include <string>
#include "ui/font/font_descriptor.hpp"

namespace ui
{
    class FontBackend;
    class FontResource;

    class FontData
    {
    public:
        FontData(FontDescriptor descriptor,
            std::shared_ptr<FontBackend> backend,
            std::shared_ptr<FontResource> resource);
        ~FontData();

        FontData(const FontData &) = delete;
        FontData &operator=(FontData &) = delete;

        const FontDescriptor &Descriptor() const noexcept;

    private:
        FontDescriptor descriptor_;
        std::shared_ptr<FontBackend> backend_;
        std::shared_ptr<FontResource> resource_;
    };
}