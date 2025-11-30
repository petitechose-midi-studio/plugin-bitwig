#include "DeviceTitleItem.hpp"
#include "ui/device/DeviceTypeHelper.hpp"
#include "ui/theme/BitwigTheme.hpp"
#include "ui/theme/StyleHelpers.hpp"
#include "ui/font/FontLoader.hpp"
#include "ui/font/icon.hpp"

using namespace Theme;

namespace Bitwig {

DeviceTitleItem::DeviceTitleItem(lv_obj_t *parent, IconSize iconSize)
    : parent_(parent), icon_size_(iconSize) {}

DeviceTitleItem::~DeviceTitleItem() {
    if (type_icon_) lv_obj_delete(type_icon_);
    if (state_icon_) lv_obj_delete(state_icon_);
    if (folder_icon_) lv_obj_delete(folder_icon_);
    if (label_) lv_obj_delete(label_);
}

void DeviceTitleItem::ensureCreated() {
    if (type_icon_ || !parent_)
        return;

    // Order: type_icon -> state_icon -> folder_icon -> label
    type_icon_ = lv_label_create(parent_);
    if (!type_icon_)
        return;

    state_icon_ = lv_label_create(parent_);

    folder_icon_ = lv_label_create(parent_);
    if (folder_icon_)
        lv_obj_add_flag(folder_icon_, LV_OBJ_FLAG_HIDDEN);

    label_ = lv_label_create(parent_);
    if (label_) {
        Style::setTextColor(label_, Color::TEXT_LIGHT);
        lv_obj_set_style_text_font(label_, bitwig_fonts.device_label, 0);
    }
}

void DeviceTitleItem::render(const DeviceTitleItemProps &props) {
    ensureCreated();

    if (label_) {
        lv_label_set_text(label_, props.name ? props.name : "");
        lv_obj_set_style_text_opa(label_, props.enabled ? Opacity::FULL : Opacity::DIMMED, 0);
    }

    updateTypeIcon(props.deviceType);
    updateStateIcon(props.enabled);
    updateFolderIcon(props.hasChildren);
}

void DeviceTitleItem::updateTypeIcon(uint8_t deviceType) {
    if (!type_icon_)
        return;

    auto info = DeviceType::get(deviceType);
    if (!info.visible) {
        lv_obj_add_flag(type_icon_, LV_OBJ_FLAG_HIDDEN);
        return;
    }

    Icon::Size size = (icon_size_ == IconSize::Small) ? Icon::S : Icon::M;
    Icon::set(type_icon_, info.icon, size);
    Style::setTextColor(type_icon_, info.color);
    lv_obj_clear_flag(type_icon_, LV_OBJ_FLAG_HIDDEN);
}

void DeviceTitleItem::updateStateIcon(bool enabled) {
    if (!state_icon_)
        return;

    const char *iconStr = enabled ? Icon::DEVICE_ON : Icon::DEVICE_OFF;
    Icon::Size size = (icon_size_ == IconSize::Small) ? Icon::S : Icon::M;
    Icon::set(state_icon_, iconStr, size);

    uint32_t color = enabled ? Color::DEVICE_STATE_ENABLED : Color::DEVICE_STATE_DISABLED;
    Style::setTextColor(state_icon_, color);
}

void DeviceTitleItem::updateFolderIcon(bool hasChildren) {
    if (!folder_icon_)
        return;

    Icon::Size size = (icon_size_ == IconSize::Small) ? Icon::S : Icon::M;
    Icon::set(folder_icon_, Icon::DIRECTORY, size);

    Style::setTextColor(folder_icon_, Color::INACTIVE_LIGHTER);
    lv_obj_set_style_text_opa(folder_icon_, Opacity::SUBTLE, 0);

    if (hasChildren)
        lv_obj_clear_flag(folder_icon_, LV_OBJ_FLAG_HIDDEN);
    else
        lv_obj_add_flag(folder_icon_, LV_OBJ_FLAG_HIDDEN);
}

lv_coord_t DeviceTitleItem::getContentWidth() const {
    lv_coord_t width = 0;

    if (type_icon_ && !lv_obj_has_flag(type_icon_, LV_OBJ_FLAG_HIDDEN))
        width += lv_obj_get_width(type_icon_);

    if (state_icon_) {
        if (width > 0) width += Layout::GAP_SM;
        width += lv_obj_get_width(state_icon_);
    }

    if (folder_icon_ && !lv_obj_has_flag(folder_icon_, LV_OBJ_FLAG_HIDDEN))
        width += Layout::GAP_MD + lv_obj_get_width(folder_icon_);

    if (label_)
        width += Layout::GAP_MD + lv_obj_get_width(label_);

    return width;
}

}  // namespace Bitwig
