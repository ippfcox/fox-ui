#include "ui/font/font_manager.hpp"

class MockFontResource : public ui::FontResource
{
};

class MockFontBackend : public ui::FontBackend
{
public:
    virtual std::unique_ptr<ui::FontResource> Create(const std::string &path, const ui::FontDescriptor &descriptor) override
    {
        ++create_count_;

        return std::make_unique<MockFontResource>();
    }

    int create_count_{0};
};

int main()
{
    auto fm = std::make_shared<ui::FontManager>(std::make_shared<MockFontBackend>());
}