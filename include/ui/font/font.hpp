#include <memory>

#include "font_descriptor.hpp"

namespace ui
{
    class FontData;

    class Font
    {
    public:
        Font() = default;

        bool Valid() const noexcept;
        int32_t Size() const noexcept;
        int32_t Weight() const noexcept;
        FontStyle Style() const noexcept;
        std::string Family() const noexcept;

    private:
        explicit Font(std::shared_ptr<FontData> data);

        std::shared_ptr<FontData> data_;
        friend class FontManager;
        friend class LvglFontAccess;
    };
}