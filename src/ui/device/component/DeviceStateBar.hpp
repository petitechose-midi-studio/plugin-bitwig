#pragma once

#include <Arduino.h>
#include <lvgl.h>
#include <memory>
#include "TrackTitleItem.hpp"
#include "DeviceTitleItem.hpp"
#include "PageTitleItem.hpp"

namespace Bitwig
{

    /**
     * @brief Top bar showing current track, device, and page info
     *
     * Layout: [Device] [Track] [Page]
     * Uses TrackTitleItem, DeviceTitleItem, PageTitleItem components.
     */
    class DeviceStateBar
    {
    public:
        explicit DeviceStateBar(lv_obj_t *parent);
        ~DeviceStateBar();

        void setTrackName(const String &track_name);
        void setTrackColor(uint32_t color);
        void setTrackType(uint8_t trackType);
        void setDeviceName(const String &device_name);
        void setDeviceState(bool enabled);
        void setDeviceHasChildren(bool hasChildren);
        void setPageName(const String &page_name);

    private:
        lv_obj_t* createCellWrapper(lv_obj_t* parent, lv_flex_align_t hAlign);
        void updateDeviceCellAlignment();

        lv_obj_t *container_ = nullptr;
        lv_obj_t *track_cell_ = nullptr;
        lv_obj_t *device_cell_ = nullptr;
        lv_obj_t *page_cell_ = nullptr;

        std::unique_ptr<TrackTitleItem> track_item_;
        std::unique_ptr<DeviceTitleItem> device_item_;
        std::unique_ptr<PageTitleItem> page_item_;
    };

} // namespace Bitwig
