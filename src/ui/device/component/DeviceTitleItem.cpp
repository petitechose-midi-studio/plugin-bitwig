#include "DeviceTitleItem.hpp"
#include "../../theme/BitwigTheme.hpp"
#include "font/FontLoader.hpp"
#include "ui/font/FontLoader.hpp"
#include "ui/font/icon.hpp"
#include "util/TextUtils.hpp"

namespace Bitwig
{

    DeviceTitleItem::DeviceTitleItem(lv_obj_t *parent, IconSize iconSize)
        : parent_(parent), icon_size_(iconSize)
    {
        // Lazy init - don't create LVGL widgets here
    }

    DeviceTitleItem::~DeviceTitleItem()
    {
        // Don't delete anything - all LVGL objects are owned by parent hierarchy
    }

    void DeviceTitleItem::ensureCreated()
    {
        if (icon_ || !parent_) return;

        // State icon
        icon_ = lv_label_create(parent_);
        if (!icon_) return;

        // Folder icon (hidden by default)
        folder_icon_ = lv_label_create(parent_);
        if (folder_icon_)
        {
            lv_obj_add_flag(folder_icon_, LV_OBJ_FLAG_HIDDEN);
        }

        // Device name label
        label_ = lv_label_create(parent_);
        if (label_)
        {
            lv_obj_set_style_text_color(label_, lv_color_hex(Theme::Color::TEXT_LIGHT), 0);
            lv_obj_set_style_text_font(label_, bitwig_fonts.device_label, 0);
            lv_label_set_text(label_, pending_name_.c_str());
            lv_obj_set_style_text_opa(label_, enabled_ ? LV_OPA_COVER : LV_OPA_50, 0);
        }

        updateIcon();
        updateFolderIcon();
    }

    void DeviceTitleItem::setName(const std::string &name)
    {
        pending_name_ = TextUtils::sanitizeText(name.c_str()).c_str();

        ensureCreated();
        if (label_)
        {
            lv_label_set_text(label_, pending_name_.c_str());
        }
    }

    void DeviceTitleItem::setState(bool enabled)
    {
        if (enabled_ != enabled)
        {
            enabled_ = enabled;

            ensureCreated();
            updateIcon();

            if (label_)
            {
                lv_obj_set_style_text_opa(label_, enabled ? LV_OPA_COVER : LV_OPA_50, 0);
            }
        }
    }

    void DeviceTitleItem::setHasChildren(bool hasChildren)
    {
        if (has_children_ != hasChildren)
        {
            has_children_ = hasChildren;

            ensureCreated();
            if (folder_icon_)
            {
                if (hasChildren)
                {
                    lv_obj_clear_flag(folder_icon_, LV_OBJ_FLAG_HIDDEN);
                }
                else
                {
                    lv_obj_add_flag(folder_icon_, LV_OBJ_FLAG_HIDDEN);
                }
            }
        }
    }

    void DeviceTitleItem::updateIcon()
    {
        if (!icon_)
            return;

        const char *iconStr = enabled_ ? Icon::DEVICE_ON : Icon::DEVICE_OFF;
        Icon::Size size = (icon_size_ == IconSize::Small) ? Icon::S12 : Icon::S14;
        Icon::set(icon_, iconStr, size);

        lv_color_t color = lv_color_hex(enabled_
            ? Theme::Color::DEVICE_STATE_ENABLED
            : Theme::Color::DEVICE_STATE_DISABLED);
        lv_obj_set_style_text_color(icon_, color, 0);
    }

    void DeviceTitleItem::updateFolderIcon()
    {
        if (!folder_icon_)
            return;

        Icon::Size size = (icon_size_ == IconSize::Small) ? Icon::S12 : Icon::S14;
        Icon::set(folder_icon_, Icon::DIRECTORY, size);

        lv_obj_set_style_text_color(folder_icon_, lv_color_hex(Theme::Color::INACTIVE_LIGHTER), 0);
        lv_obj_set_style_text_opa(folder_icon_, LV_OPA_70, 0);
    }

    lv_coord_t DeviceTitleItem::getContentWidth() const
    {
        lv_coord_t width = 0;
        constexpr lv_coord_t gap = 6;

        if (icon_)
        {
            width += lv_obj_get_width(icon_);
        }

        if (folder_icon_ && !lv_obj_has_flag(folder_icon_, LV_OBJ_FLAG_HIDDEN))
        {
            width += gap + lv_obj_get_width(folder_icon_);
        }

        if (label_)
        {
            width += gap + lv_obj_get_width(label_);
        }

        return width;
    }

} // namespace Bitwig
