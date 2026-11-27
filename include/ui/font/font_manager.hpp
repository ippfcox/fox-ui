#include <optional>
#include "font_family.hpp"
#include "font_descriptor.hpp"
#include "font.hpp"
#include "font_backend.hpp"

namespace ui
{
    class FontManagerImpl;

    class FontManager
    {
    public:
        explicit FontManager(std::shared_ptr<FontBackend> backend);
        ~FontManager();

        FontManager(const FontManager &) = delete;
        FontManager &operator=(const FontManager &) = delete;

        FontManager(FontManager &&rhs) noexcept;
        FontManager &operator=(FontManager &&rhs) noexcept;

        bool RegisterFamily(const FontFamily &family);
        bool UnregisterFamily(const std::string &name);
        bool HasFamily(const std::string &name) const;

        std::optional<Font> GetFont(const FontDescriptor &descriptor);
        // bool SetFallbacks(const std::string &family, const std::vector<std::string> &fallbacks);

    private:
        std::unique_ptr<FontManagerImpl> impl_;
    };
}