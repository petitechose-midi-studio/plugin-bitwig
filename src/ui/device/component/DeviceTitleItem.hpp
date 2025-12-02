#pragma once

#include <cstdint>

#include <lvgl.h>

namespace Bitwig {

struct DeviceTitleItemProps {
    const char *name = "";
    uint8_t deviceType = 0;  // 0=Unknown, 1=Audio, 2=Instrument, 3=Note
    bool enabled = false;
    bool hasChildren = false;
};

class DeviceTitleItem {
public:
    enum class IconSize { SMALL = 12, MEDIUM = 14 };

    explicit DeviceTitleItem(lv_obj_t *parent, IconSize iconSize = IconSize::MEDIUM);
    ~DeviceTitleItem();

    void render(const DeviceTitleItemProps &props);
    lv_coord_t getContentWidth() const;
private:
    void updateTypeIcon(uint8_t deviceType);
    void updateStateIcon(bool enabled);
    void updateFolderIcon(bool hasChildren);

    lv_obj_t *parent_ = nullptr;
    lv_obj_t *type_icon_ = nullptr;   // Device type: Audio/Instrument/Note
    lv_obj_t *state_icon_ = nullptr;  // Device state: ON/OFF
    lv_obj_t *folder_icon_ = nullptr;
    lv_obj_t *label_ = nullptr;
    IconSize icon_size_;
};

}  // namespace Bitwig
