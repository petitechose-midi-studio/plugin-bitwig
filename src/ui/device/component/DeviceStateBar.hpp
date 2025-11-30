#pragma once

#include <lvgl.h>
#include <memory>
#include <cstdint>
#include "DeviceTitleItem.hpp"
#include "widget/TitleItem.hpp"

namespace Bitwig
{

struct DeviceStateBarProps
{
    const char *deviceName = "";
    uint8_t deviceType = 0;  // 0=Unknown, 1=Audio, 2=Instrument, 3=Note
    bool deviceEnabled = false;
    bool deviceHasChildren = false;
    const char *pageName = "";
};

class DeviceStateBar
{
public:
    explicit DeviceStateBar(lv_obj_t *parent);
    ~DeviceStateBar();

    void render(const DeviceStateBarProps &props);

private:
    void ensureCreated();
    lv_obj_t *createCellWrapper(lv_obj_t *parent, lv_flex_align_t hAlign);

    lv_obj_t *parent_ = nullptr;
    lv_obj_t *container_ = nullptr;
    lv_obj_t *device_cell_ = nullptr;
    lv_obj_t *page_cell_ = nullptr;

    std::unique_ptr<DeviceTitleItem> device_item_;
    std::unique_ptr<UI::TitleItem> page_item_;
};

} // namespace Bitwig
