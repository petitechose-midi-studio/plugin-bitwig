#include "DeviceTitleItem.hpp"
#include "../../theme/BitwigTheme.hpp"
#include "font/FontLoader.hpp"
#include "ui/font/FontLoader.hpp"
#include "ui/font/icon.hpp"
#include "util/TextUtils.hpp"

namespace Bitwig
{

    DeviceTitleItem::DeviceTitleItem(lv_obj_t *parent, IconSize iconSize)
        : icon_size_(iconSize)
    {
        // Container - content-sized for flexible positioning by parent
        container_ = lv_obj_create(parent);
        lv_obj_set_size(container_, LV_SIZE_CONTENT, LV_PCT(100));
        lv_obj_set_style_bg_opa(container_, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_width(container_, 0, 0);
        lv_obj_set_style_pad_all(container_, 0, 0);
        lv_obj_set_style_pad_gap(container_, 4, 0);
        lv_obj_clear_flag(container_, LV_OBJ_FLAG_SCROLLABLE);

        lv_obj_set_flex_flow(container_, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(container_,
                              LV_FLEX_ALIGN_START,
                              LV_FLEX_ALIGN_CENTER,
                              LV_FLEX_ALIGN_CENTER);

        // State icon
        icon_ = lv_label_create(container_);
        updateIcon();

        // Device name label - no flex-grow, content-sized
        label_ = std::make_unique<Label>(container_);
        label_->setFlexGrow(false);
        label_->setColor(lv_color_hex(Theme::Color::TEXT_LIGHT));
        label_->setFont(bitwig_fonts.device_label);
    }

    DeviceTitleItem::~DeviceTitleItem()
    {
        if (container_)
        {
            lv_obj_delete(container_);
        }
    }

    void DeviceTitleItem::setName(const std::string &name)
    {
        if (label_)
        {
            std::string clean = TextUtils::sanitizeText(name.c_str()).c_str();
            label_->setText(clean);
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
                lv_obj_set_style_text_opa(label_->getLabel(), enabled ? LV_OPA_COVER : LV_OPA_50, 0);
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

} // namespace Bitwig
