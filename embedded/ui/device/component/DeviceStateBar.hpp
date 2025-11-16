#pragma once

#include <Arduino.h>
#include <lvgl.h>

namespace Plugin::Bitwig {

class DeviceStateBar {
public:
    enum class DeviceStatus { ENABLED, DISABLED };

    explicit DeviceStateBar(lv_obj_t* parent);
    ~DeviceStateBar();

    void setTrackName(const String& track_name);
    void setTrackColor(uint32_t color);
    void setDeviceName(const String& device_name);
    void setDeviceState(uint8_t state);
    void setPageName(const String& page_name);

private:
    void createTrackSection();
    void createDeviceSection();
    void createPageSection();

    lv_coord_t getTrackAvailableWidth() const;
    lv_coord_t getDeviceAvailableWidth() const;

    lv_obj_t* container_ = nullptr;
    lv_obj_t* track_container_ = nullptr;
    lv_obj_t* track_color_indicator_ = nullptr;
    lv_obj_t* track_label_ = nullptr;
    lv_obj_t* device_container_ = nullptr;
    lv_obj_t* device_icon_ = nullptr;
    lv_obj_t* device_label_ = nullptr;
    lv_obj_t* page_label_ = nullptr;

    String track_name_;
    uint32_t track_color_ = 0xFFFFFF;
    String device_name_;
    DeviceStatus device_status_;
};

} // namespace Plugin::Bitwig
