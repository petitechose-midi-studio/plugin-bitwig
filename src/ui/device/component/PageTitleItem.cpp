#include "PageTitleItem.hpp"
#include "../../theme/BitwigTheme.hpp"
#include "ui/font/FontLoader.hpp"
#include "util/TextUtils.hpp"

namespace Bitwig
{

    PageTitleItem::PageTitleItem(lv_obj_t *parent)
    {
        // Create label directly in parent - owned by LVGL parent hierarchy
        label_ = lv_label_create(parent);
        lv_obj_set_style_text_color(label_, lv_color_hex(Theme::Color::TEXT_LIGHT), 0);
        lv_obj_set_style_text_font(label_, bitwig_fonts.page_label, 0);
        lv_label_set_text(label_, "");
    }

    PageTitleItem::~PageTitleItem()
    {
        // Don't delete anything - LVGL parent handles cleanup
    }

    void PageTitleItem::setName(const std::string &name)
    {
        if (label_)
        {
            if (name.empty())
            {
                lv_label_set_text(label_, "Page");
            }
            else
            {
                std::string clean = TextUtils::sanitizeText(name.c_str()).c_str();
                lv_label_set_text(label_, clean.c_str());
            }
        }
    }

} // namespace Bitwig
