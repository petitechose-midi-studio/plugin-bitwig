#pragma once

#include <Arduino.h>
#include <lvgl.h>
#include <memory>
#include "DeviceTitleItem.hpp"
#include "PageTitleItem.hpp"

namespace Bitwig
{

    /**
     * @brief Top bar showing device and page info
     *
     * Layout: [Device] [Page]
     * Uses DeviceTitleItem and PageTitleItem components.
     * Uses lazy initialization - LVGL widgets created on first use.
     */
    class DeviceStateBar
    {
    public:
        explicit DeviceStateBar(lv_obj_t *parent);
        ~DeviceStateBar();

        void setDeviceName(const String &device_name);
        void setDeviceState(bool enabled);
        void setDeviceHasChildren(bool hasChildren);
        void setPageName(const String &page_name);

    private:
        void ensureCreated();
        lv_obj_t* createCellWrapper(lv_obj_t* parent, lv_flex_align_t hAlign);

        lv_obj_t *parent_ = nullptr;
        lv_obj_t *container_ = nullptr;
        lv_obj_t *device_cell_ = nullptr;
        lv_obj_t *page_cell_ = nullptr;

        std::unique_ptr<DeviceTitleItem> device_item_;
        std::unique_ptr<PageTitleItem> page_item_;

        // Pending state
        String pending_device_name_ = "No Device";
        String pending_page_name_ = "";
        bool pending_device_enabled_ = false;
        bool pending_device_has_children_ = false;
    };

} // namespace Bitwig
