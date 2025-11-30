#include "DeviceTitleItem.hpp"
#include "../../theme/BitwigTheme.hpp"
#include "ui/font/FontLoader.hpp"
#include "ui/font/icon.hpp"

namespace Bitwig
{

DeviceTitleItem::DeviceTitleItem(lv_obj_t *parent, IconSize iconSize)
    : parent_(parent), icon_size_(iconSize)
{
}

DeviceTitleItem::~DeviceTitleItem()
{
    // Delete LVGL objects (children of parent)
    if (icon_)
        lv_obj_delete(icon_);
    if (folder_icon_)
        lv_obj_delete(folder_icon_);
    if (label_)
        lv_obj_delete(label_);
}

void DeviceTitleItem::ensureCreated()
{
    if (icon_ || !parent_)
        return;

    icon_ = lv_label_create(parent_);
    if (!icon_)
        return;

    folder_icon_ = lv_label_create(parent_);
    if (folder_icon_)
        lv_obj_add_flag(folder_icon_, LV_OBJ_FLAG_HIDDEN);

    label_ = lv_label_create(parent_);
    if (label_)
    {
        lv_obj_set_style_text_color(label_, lv_color_hex(Theme::Color::TEXT_LIGHT), 0);
        lv_obj_set_style_text_font(label_, bitwig_fonts.device_label, 0);
    }
}

void DeviceTitleItem::render(const DeviceTitleItemProps &props)
{
    ensureCreated();

    if (label_)
    {
        lv_label_set_text(label_, props.name ? props.name : "");
        lv_obj_set_style_text_opa(label_, props.enabled ? LV_OPA_COVER : LV_OPA_50, 0);
    }

    updateIcon(props.enabled);
    updateFolderIcon(props.hasChildren);
}

void DeviceTitleItem::updateIcon(bool enabled)
{
    if (!icon_)
        return;

    const char *iconStr = enabled ? Icon::DEVICE_ON : Icon::DEVICE_OFF;
    Icon::Size size = (icon_size_ == IconSize::Small) ? Icon::S12 : Icon::S14;
    Icon::set(icon_, iconStr, size);

    lv_color_t color = lv_color_hex(enabled ? Theme::Color::DEVICE_STATE_ENABLED
                                            : Theme::Color::DEVICE_STATE_DISABLED);
    lv_obj_set_style_text_color(icon_, color, 0);
}

void DeviceTitleItem::updateFolderIcon(bool hasChildren)
{
    if (!folder_icon_)
        return;

    Icon::Size size = (icon_size_ == IconSize::Small) ? Icon::S12 : Icon::S14;
    Icon::set(folder_icon_, Icon::DIRECTORY, size);

    lv_obj_set_style_text_color(folder_icon_, lv_color_hex(Theme::Color::INACTIVE_LIGHTER), 0);
    lv_obj_set_style_text_opa(folder_icon_, LV_OPA_70, 0);

    if (hasChildren)
        lv_obj_clear_flag(folder_icon_, LV_OBJ_FLAG_HIDDEN);
    else
        lv_obj_add_flag(folder_icon_, LV_OBJ_FLAG_HIDDEN);
}

lv_coord_t DeviceTitleItem::getContentWidth() const
{
    lv_coord_t width = 0;
    constexpr lv_coord_t gap = 6;

    if (icon_)
        width += lv_obj_get_width(icon_);

    if (folder_icon_ && !lv_obj_has_flag(folder_icon_, LV_OBJ_FLAG_HIDDEN))
        width += gap + lv_obj_get_width(folder_icon_);

    if (label_)
        width += gap + lv_obj_get_width(label_);

    return width;
}

} // namespace Bitwig
