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
        // Create children directly in parent (no intermediate container)
        // Parent should be a flex row container

        // State icon
        icon_ = lv_label_create(parent_);
        updateIcon();

        // Folder icon (hidden by default)
        folder_icon_ = lv_label_create(parent_);
        lv_obj_add_flag(folder_icon_, LV_OBJ_FLAG_HIDDEN);
        updateFolderIcon();

        // Device name label - raw lv_label, owned by LVGL parent
        label_ = lv_label_create(parent_);
        lv_obj_set_style_text_color(label_, lv_color_hex(Theme::Color::TEXT_LIGHT), 0);
        lv_obj_set_style_text_font(label_, bitwig_fonts.device_label, 0);
        lv_label_set_text(label_, "");
    }

    DeviceTitleItem::~DeviceTitleItem()
    {
        // Don't delete anything - all LVGL objects are owned by parent hierarchy
        // When parent is deleted, LVGL recursively deletes all children
    }

    void DeviceTitleItem::setName(const std::string &name)
    {
        if (label_)
        {
            std::string clean = TextUtils::sanitizeText(name.c_str()).c_str();
            lv_label_set_text(label_, clean.c_str());
        }
    }

    void DeviceTitleItem::setState(bool enabled)
    {
        if (enabled_ != enabled)
        {
            enabled_ = enabled;
            updateIcon();

            // Update label opacity based on state
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

        // Folder icon with reduced opacity
        lv_obj_set_style_text_color(folder_icon_, lv_color_hex(Theme::Color::INACTIVE_LIGHTER), 0);
        lv_obj_set_style_text_opa(folder_icon_, LV_OPA_70, 0);
    }

    lv_coord_t DeviceTitleItem::getContentWidth() const
    {
        lv_coord_t width = 0;
        constexpr lv_coord_t gap = 6;  // Same as parent's flex gap

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
