
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_MULTIPLE_MASTERS_H

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

static constexpr FT_ULong TAG_WGHT = FT_MAKE_TAG('w', 'g', 'h', 't');
static constexpr FT_ULong TAG_WDTH = FT_MAKE_TAG('w', 'd', 't', 'h');

static float toFloat(FT_Fixed value)
{
    return static_cast<float>(value) / 65536.0f;
}

static FT_Fixed toFixed(float value)
{
    return static_cast<FT_Fixed>(value * 65536.0f);
}

static uint64_t bitmapHash(const FT_Bitmap &bitmap)
{
    uint64_t hash = 14695981039346656037ULL;

    for (unsigned int y = 0; y < bitmap.rows; ++y)
    {
        const unsigned char *row =
            bitmap.buffer + y * bitmap.pitch;

        for (unsigned int x = 0; x < bitmap.width; ++x)
        {
            hash ^= row[x];
            hash *= 1099511628211ULL;
        }
    }

    return hash;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " font.ttf\n";
        return 1;
    }

    FT_Library library = nullptr;
    FT_Face face = nullptr;

    if (FT_Init_FreeType(&library))
    {
        std::cerr << "FT_Init_FreeType failed\n";
        return 1;
    }

    if (FT_New_Face(library, argv[1], 0, &face))
    {
        std::cerr << "FT_New_Face failed\n";
        FT_Done_FreeType(library);
        return 1;
    }

    FT_MM_Var *mmVar = nullptr;

    if (FT_Get_MM_Var(face, &mmVar))
    {
        std::cerr << "This font has no variable axes\n";
        FT_Done_Face(face);
        FT_Done_FreeType(library);
        return 1;
    }

    std::vector<FT_Fixed> coords(mmVar->num_axis);

    std::cout << "Variable axes: " << mmVar->num_axis << "\n";

    for (FT_UInt i = 0; i < mmVar->num_axis; ++i)
    {
        const auto &axis = mmVar->axis[i];

        coords[i] = axis.def;

        std::cout << "Axis: "
                  << static_cast<char>(axis.tag >> 24)
                  << static_cast<char>(axis.tag >> 16)
                  << static_cast<char>(axis.tag >> 8)
                  << static_cast<char>(axis.tag)
                  << "  min=" << toFloat(axis.minimum)
                  << "  default=" << toFloat(axis.def)
                  << "  max=" << toFloat(axis.maximum)
                  << "\n";
    }

    // 设置测试字号
    FT_Set_Pixel_Sizes(face, 0, 48);

    auto render = [&]() {
        if (FT_Load_Char(face, 'A', FT_LOAD_RENDER))
        {
            std::cerr << "FT_Load_Char failed\n";
            return;
        }

        const auto &slot = *face->glyph;

        std::cout << "  width=" << slot.bitmap.width
                  << " height=" << slot.bitmap.rows
                  << " advance=" << slot.advance.x / 64.0f
                  << " hash=" << bitmapHash(slot.bitmap)
                  << "\n";
    };

    auto testAxis = [&](FT_ULong tag,
                        const std::vector<float> &values) {
        int index = -1;

        for (FT_UInt i = 0; i < mmVar->num_axis; ++i)
        {
            if (mmVar->axis[i].tag == tag)
            {
                index = static_cast<int>(i);
                break;
            }
        }

        if (index < 0)
        {
            std::cout << "Axis not found: " << tag << "\n";
            return;
        }

        const auto &axis = mmVar->axis[index];

        for (float value : values)
        {
            value = std::max(
                toFloat(axis.minimum),
                std::min(value, toFloat(axis.maximum)));

            // 每次都从默认值开始，只修改目标轴
            for (FT_UInt i = 0; i < mmVar->num_axis; ++i)
            {
                coords[i] = mmVar->axis[i].def;
            }

            coords[index] = toFixed(value);

            if (FT_Set_Var_Design_Coordinates(
                    face, mmVar->num_axis, coords.data()))
            {
                std::cerr << "FT_Set_Var_Design_Coordinates failed\n";
                continue;
            }

            std::cout << "Value = " << value << "\n";
            render();
        }
    };

    std::cout << "\nTesting wght:\n";
    testAxis(TAG_WGHT, {100, 400, 700, 900});

    std::cout << "\nTesting wdth:\n";
    testAxis(TAG_WDTH, {75, 87.5f, 100, 112.5f, 125});

    FT_Done_MM_Var(library, mmVar);
    FT_Done_Face(face);
    FT_Done_FreeType(library);

    return 0;
}
