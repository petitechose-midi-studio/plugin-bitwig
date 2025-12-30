#pragma once

#include "DeviceTitleItem.hpp"

#include <cstdint>
#include <memory>

#include <lvgl.h>

#include <oc/ui/lvgl/IWidget.hpp>

#include "ui/widget/TitleItem.hpp"

namespace bitwig {

struct DeviceStateBarProps {
    const char *deviceName = "";
    uint8_t deviceType = 0;  // 0=Unknown, 1=Audio, 2=Instrument, 3=Note
    bool deviceEnabled = false;
    bool deviceHasChildren = false;
    const char *pageName = "";
};

/**
 * @brief State bar showing current device and page information
 *
 * Displays device title (type, enabled state) and current page name.
 * Implements IWidget for consistent element access.
 */
class DeviceStateBar : public oc::ui::lvgl::IWidget {
public:
    explicit DeviceStateBar(lv_obj_t *parent);
    ~DeviceStateBar() override;

    DeviceStateBar(const DeviceStateBar&) = delete;
    DeviceStateBar& operator=(const DeviceStateBar&) = delete;

    void render(const DeviceStateBarProps &props);

    // IWidget
    lv_obj_t* getElement() const override { return container_; }

private:
    lv_obj_t *createCellWrapper(lv_obj_t *parent, lv_flex_align_t hAlign);

    lv_obj_t *container_ = nullptr;
    lv_obj_t *device_cell_ = nullptr;
    lv_obj_t *page_cell_ = nullptr;

    std::unique_ptr<DeviceTitleItem> device_item_;
    std::unique_ptr<TitleItem> page_item_;
};

}  // namespace bitwig
