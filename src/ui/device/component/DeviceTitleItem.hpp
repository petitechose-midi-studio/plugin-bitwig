#pragma once

#include <lvgl.h>
#include <cstdint>

namespace Bitwig
{

struct DeviceTitleItemProps
{
    const char *name = "";
    bool enabled = false;
    bool hasChildren = false;
};

class DeviceTitleItem
{
public:
    enum class IconSize
    {
        Small = 12,
        Medium = 14
    };

    explicit DeviceTitleItem(lv_obj_t *parent, IconSize iconSize = IconSize::Medium);
    ~DeviceTitleItem();

    void render(const DeviceTitleItemProps &props);
    lv_coord_t getContentWidth() const;

private:
    void ensureCreated();
    void updateIcon(bool enabled);
    void updateFolderIcon(bool hasChildren);

    lv_obj_t *parent_ = nullptr;
    lv_obj_t *icon_ = nullptr;
    lv_obj_t *folder_icon_ = nullptr;
    lv_obj_t *label_ = nullptr;
    IconSize icon_size_;
};

} // namespace Bitwig
