#include <unordered_map>
#include <algorithm>
#include "ui/font/font_manager.hpp"
#include "font_data.hpp"

namespace ui
{
    struct FontManagerImpl
    {
        std::shared_ptr<FontBackend> backend;
        std::unordered_map<std::string, FontFamily> families;
        std::unordered_map<FontDescriptor, std::weak_ptr<FontData>, FontDescriptorHash> cache;
    };

    FontManager::FontManager(std::shared_ptr<FontBackend> backend)
        : impl_(std::make_unique<FontManagerImpl>())
    {
        impl_->backend = backend;
    }

    FontManager::~FontManager()
    {
    }

    FontManager::FontManager(FontManager &&rhs) noexcept
        : impl_(std::move(rhs.impl_))
    {
    }

    FontManager &FontManager::operator=(FontManager &&rhs) noexcept
    {
        impl_ = std::move(rhs.impl_);

        return *this;
    }

    bool FontManager::RegisterFamily(const FontFamily &family)
    {
        if (!impl_)
            return false;

        if (family.name.empty() || family.faces.empty())
            return false;

        if (impl_->families.contains(family.name))
            return false;

        impl_->families.emplace(family.name, family);

        return true;
    }

    bool FontManager::UnregisterFamily(const std::string &name)
    {
        if (!impl_)
            return false;

        if (impl_->families.erase(name) == 0)
            return false;

        std::erase_if(impl_->cache, [&](const auto &item) {
            return item.first.family == name;
        });

        return true;
    }

    bool FontManager::HasFamily(const std::string &name) const
    {
        if (!impl_)
            return false;

        return impl_->families.contains(name);
    }

    std::optional<Font> FontManager::GetFont(const FontDescriptor &descriptor)
    {
        if (!impl_ || !impl_->backend)
            return std::nullopt;

        if (descriptor.family.empty() ||
            descriptor.size <= 0 ||
            descriptor.weight < 1 ||
            descriptor.weight > 2000)
            return std::nullopt;

        if (!impl_->families.contains(descriptor.family))
            return std::nullopt;

        if (impl_->cache.contains(descriptor))
        {
            if (auto data = impl_->cache.at(descriptor).lock())
                return Font(std::move(data));

            impl_->cache.erase(descriptor);
        }

        const auto &family = impl_->families.at(descriptor.family);

        auto face_it = std::find_if(family.faces.begin(), family.faces.end(), [&](const FontFaceConfig &face) {
            return face.style == descriptor.style;
        });

        if (face_it == family.faces.end())
            return std::nullopt;

        auto resource = impl_->backend->Create(face_it->path, descriptor);

        auto data = std::make_shared<FontData>(descriptor, impl_->backend, std::move(resource));

        impl_->cache.emplace(descriptor, data);

        return Font(std::move(data));
    }
}