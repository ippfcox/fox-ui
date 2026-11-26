#include <string>
#include <vector>
#include "font_descriptor.hpp"

namespace ui
{
    struct FontFaceConfig
    {
        FontStyle style;
        std::string path;
    };

    struct FontFamily
    {
        std::string name;
        std::vector<FontFaceConfig> faces;
        std::vector<std::string> fallbacks;
    };
}