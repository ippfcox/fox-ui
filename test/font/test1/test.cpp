#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <vector>

#include "ui/font/font_manager.hpp"

namespace
{
    // 通过引用计数追踪 FontResource 的存活数量，用于验证释放时机
    class MockFontResource : public ui::FontResource
    {
    public:
        explicit MockFontResource(int &live_count) : live_count_(live_count)
        {
            ++live_count_;
        }

        ~MockFontResource() override
        {
            --live_count_;
        }

    private:
        int &live_count_;
    };

    class MockFontBackend : public ui::FontBackend
    {
    public:
        std::unique_ptr<ui::FontResource> Create(const std::string &path,
            const ui::FontDescriptor &descriptor) override
        {
            ++create_count_;
            paths_.push_back(path);
            return std::make_unique<MockFontResource>(live_count_);
        }

        int create_count_{0};
        int live_count_{0};
        std::vector<std::string> paths_;
    };

    ui::FontFamily MakeFamily(const std::string &name, const std::string &path)
    {
        ui::FontFamily family;
        family.name = name;
        family.faces.push_back({ui::FontStyle::Normal, path});
        return family;
    }

    ui::FontDescriptor MakeDesc(const std::string &family, int32_t size = 24, int32_t weight = 400)
    {
        ui::FontDescriptor desc;
        desc.family = family;
        desc.size = size;
        desc.weight = weight;
        return desc;
    }
}

class FontManagerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        backend_ = std::make_shared<MockFontBackend>();
        manager_ = std::make_unique<ui::FontManager>(backend_);
    }

    std::shared_ptr<MockFontBackend> backend_;
    std::unique_ptr<ui::FontManager> manager_;
};

// 测试项1：注册相同 Family 两次，第二次失败
TEST_F(FontManagerTest, RegisterSameFamilyTwiceFails)
{
    ASSERT_TRUE(manager_->RegisterFamily(MakeFamily("Noto", "/fonts/a.ttf")));
    EXPECT_FALSE(manager_->RegisterFamily(MakeFamily("Noto", "/fonts/b.ttf")));
}

// 测试项2：请求不存在的 Family，返回空
TEST_F(FontManagerTest, GetFontUnknownFamilyReturnsEmpty)
{
    auto font = manager_->GetFont(MakeDesc("NoSuchFamily"));
    EXPECT_FALSE(font.has_value());
    EXPECT_EQ(backend_->create_count_, 0);
}

// 测试项3：连续请求相同 Descriptor，只创建一次
TEST_F(FontManagerTest, SameDescriptorCreatesOnlyOnce)
{
    ASSERT_TRUE(manager_->RegisterFamily(MakeFamily("Noto", "/fonts/a.ttf")));

    auto f1 = manager_->GetFont(MakeDesc("Noto"));
    auto f2 = manager_->GetFont(MakeDesc("Noto"));

    ASSERT_TRUE(f1.has_value());
    ASSERT_TRUE(f2.has_value());
    EXPECT_EQ(backend_->create_count_, 1);
}

// 测试项4：请求不同 weight，创建不同实例
TEST_F(FontManagerTest, DifferentWeightCreatesSeparateInstances)
{
    ASSERT_TRUE(manager_->RegisterFamily(MakeFamily("Noto", "/fonts/a.ttf")));

    auto f1 = manager_->GetFont(MakeDesc("Noto", 24, 400));
    auto f2 = manager_->GetFont(MakeDesc("Noto", 24, 700));

    ASSERT_TRUE(f1.has_value());
    ASSERT_TRUE(f2.has_value());
    EXPECT_EQ(backend_->create_count_, 2);
    EXPECT_EQ(f1->Weight(), 400);
    EXPECT_EQ(f2->Weight(), 700);
}

// 测试项5：复制 Font 句柄，不创建新实例
TEST_F(FontManagerTest, CopyFontHandleDoesNotCreateNewInstance)
{
    ASSERT_TRUE(manager_->RegisterFamily(MakeFamily("Noto", "/fonts/a.ttf")));

    auto f1 = manager_->GetFont(MakeDesc("Noto"));
    ASSERT_TRUE(f1.has_value());

    ui::Font copied = *f1; // 复制句柄

    EXPECT_TRUE(copied.Valid());
    EXPECT_EQ(backend_->create_count_, 1);
    EXPECT_EQ(backend_->live_count_, 1);
}

// 测试项6：销毁其中一个 Font，资源仍然有效
TEST_F(FontManagerTest, DestroyingOneFontKeepsResourceValid)
{
    ASSERT_TRUE(manager_->RegisterFamily(MakeFamily("Noto", "/fonts/a.ttf")));

    auto f1 = manager_->GetFont(MakeDesc("Noto"));
    ASSERT_TRUE(f1.has_value());
    ui::Font copied = *f1;

    f1 = ui::Font{}; // 销毁其中一个句柄

    EXPECT_TRUE(copied.Valid());
    EXPECT_EQ(copied.Size(), 24);
    EXPECT_EQ(backend_->live_count_, 1);
}

// 测试项7：销毁最后一个 Font，FontData 和 Resource 释放
TEST_F(FontManagerTest, DestroyingLastFontReleasesResource)
{
    ASSERT_TRUE(manager_->RegisterFamily(MakeFamily("Noto", "/fonts/a.ttf")));

    {
        auto f1 = manager_->GetFont(MakeDesc("Noto"));
        ASSERT_TRUE(f1.has_value());
        EXPECT_EQ(backend_->live_count_, 1);
    } // f1 在此析构，manager 内部缓存是 weak_ptr，不延长生命周期

    EXPECT_EQ(backend_->live_count_, 0);
}

// 测试项8：注销 Family 后，已有 Font 仍然有效
TEST_F(FontManagerTest, ExistingFontValidAfterUnregisterFamily)
{
    ASSERT_TRUE(manager_->RegisterFamily(MakeFamily("Noto", "/fonts/a.ttf")));

    auto f1 = manager_->GetFont(MakeDesc("Noto"));
    ASSERT_TRUE(f1.has_value());

    EXPECT_TRUE(manager_->UnregisterFamily("Noto"));

    EXPECT_TRUE(f1->Valid());
    EXPECT_EQ(f1->Size(), 24);
    EXPECT_EQ(backend_->live_count_, 1);
}

// 测试项9：注销后重新注册同名 Family，使用新字体路径
TEST_F(FontManagerTest, ReRegisterFamilyUsesNewPath)
{
    ASSERT_TRUE(manager_->RegisterFamily(MakeFamily("Noto", "/fonts/a.ttf")));

    auto f1 = manager_->GetFont(MakeDesc("Noto"));
    ASSERT_TRUE(f1.has_value());

    ASSERT_TRUE(manager_->UnregisterFamily("Noto"));
    ASSERT_TRUE(manager_->RegisterFamily(MakeFamily("Noto", "/fonts/b.ttf")));

    auto f2 = manager_->GetFont(MakeDesc("Noto"));
    ASSERT_TRUE(f2.has_value());

    ASSERT_EQ(backend_->paths_.size(), 2u);
    EXPECT_EQ(backend_->paths_[0], "/fonts/a.ttf");
    EXPECT_EQ(backend_->paths_[1], "/fonts/b.ttf");
}
