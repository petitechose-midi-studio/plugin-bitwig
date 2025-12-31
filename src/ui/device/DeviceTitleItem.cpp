#include "DeviceTitleItem.hpp"

#include <oc/ui/lvgl/style/StyleBuilder.hpp>

#include "ui/device/DeviceTypeHelper.hpp"
#include "ui/font/BitwigFonts.hpp"
#include "ui/font/icon.hpp"
#include "ui/theme/BitwigTheme.hpp"

using namespace bitwig::theme;
namespace style = oc::ui::lvgl::style;

namespace bitwig {

DeviceTitleItem::DeviceTitleItem(lv_obj_t *parent, IconSize iconSize)
    : icon_size_(iconSize) {
    if (!parent) return;

    // Create container that holds all child elements
    container_ = lv_obj_create(parent);
    if (!container_) return;

    // Style container as transparent flex row
    lv_obj_set_size(container_, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(container_, LV_OPA_TRANSP, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(container_, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(container_, 0, LV_STATE_DEFAULT);
    lv_obj_set_flex_flow(container_, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(container_, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(container_, 4, LV_STATE_DEFAULT);
    lv_obj_clear_flag(container_, LV_OBJ_FLAG_SCROLLABLE);

    // Order: type_icon -> state_icon -> folder_icon -> label
    type_icon_ = lv_label_create(container_);
    if (!type_icon_) return;

    state_icon_ = lv_label_create(container_);

    folder_icon_ = lv_label_create(container_);
    if (folder_icon_) lv_obj_add_flag(folder_icon_, LV_OBJ_FLAG_HIDDEN);

    // Use framework Label widget with auto-scroll for overflow text
    label_ = std::make_unique<oc::ui::lvgl::Label>(container_);
    label_->alignment(LV_TEXT_ALIGN_LEFT)
           .color(Color::TEXT_LIGHT)
           .font(bitwig_fonts.device_label)
           .ownsLvglObjects(false);
}

DeviceTitleItem::~DeviceTitleItem() {
    // Delete container - this also deletes all children
    // label_ unique_ptr is cleaned up automatically (ownsLvglObjects=false means LVGL handles it)
    if (container_) {
        lv_obj_delete(container_);
        container_ = nullptr;
    }
}

void DeviceTitleItem::render(const DeviceTitleItemProps &props) {
    if (label_) {
        label_->setText(props.name ? props.name : "");
        lv_obj_set_style_text_opa(label_->getLabel(), props.enabled ? Opacity::FULL : Opacity::DIMMED,
                                  LV_STATE_DEFAULT);
    }

    updateTypeIcon(props.deviceType);
    updateStateIcon(props.enabled);
    updateFolderIcon(props.hasChildren);
}

void DeviceTitleItem::updateTypeIcon(DeviceType deviceType) {
    if (!type_icon_) return;

    auto info = DeviceTypeHelper::get(deviceType);
    if (!info.visible) {
        lv_obj_add_flag(type_icon_, LV_OBJ_FLAG_HIDDEN);
        return;
    }

    Icon::Size size = (icon_size_ == IconSize::SMALL) ? Icon::Size::S : Icon::Size::M;
    Icon::set(type_icon_, info.icon, size);
    style::apply(type_icon_).textColor(info.color);
    lv_obj_clear_flag(type_icon_, LV_OBJ_FLAG_HIDDEN);
}

void DeviceTitleItem::updateStateIcon(bool enabled) {
    if (!state_icon_) return;

    const char *icon_str = enabled ? Icon::DEVICE_ON : Icon::DEVICE_OFF;
    Icon::Size size = (icon_size_ == IconSize::SMALL) ? Icon::Size::S : Icon::Size::M;
    Icon::set(state_icon_, icon_str, size);

    uint32_t color = enabled ? Color::DEVICE_STATE_ENABLED : Color::DEVICE_STATE_DISABLED;
    style::apply(state_icon_).textColor(color);
}

void DeviceTitleItem::updateFolderIcon(bool hasChildren) {
    if (!folder_icon_) return;

    Icon::Size size = (icon_size_ == IconSize::SMALL) ? Icon::Size::S : Icon::Size::M;
    Icon::set(folder_icon_, Icon::BROWSER_DIRECTORY, size);

    style::apply(folder_icon_).textColor(Color::INACTIVE_LIGHTER);
    lv_obj_set_style_text_opa(folder_icon_, Opacity::SUBTLE, LV_STATE_DEFAULT);

    if (hasChildren) lv_obj_clear_flag(folder_icon_, LV_OBJ_FLAG_HIDDEN);
    else lv_obj_add_flag(folder_icon_, LV_OBJ_FLAG_HIDDEN);
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

    if (label_) width += Layout::GAP_MD + lv_obj_get_width(label_->getElement());

    return width;
}

void DeviceTitleItem::show() {
    if (container_) {
        lv_obj_clear_flag(container_, LV_OBJ_FLAG_HIDDEN);
    }
    visible_ = true;
}

void DeviceTitleItem::hide() {
    if (container_) {
        lv_obj_add_flag(container_, LV_OBJ_FLAG_HIDDEN);
    }
    visible_ = false;
}

void DeviceTitleItem::setHighlighted(bool highlighted) {
    highlighted_ = highlighted;
    if (label_) {
        uint32_t labelColor = highlighted ? Color::TEXT_PRIMARY : Color::TEXT_LIGHT;
        label_->color(labelColor);
    }
}

}  // namespace bitwig
