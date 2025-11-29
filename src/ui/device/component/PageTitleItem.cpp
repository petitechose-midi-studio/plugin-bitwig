#include "PageTitleItem.hpp"
#include "../../theme/BitwigTheme.hpp"
#include "ui/font/FontLoader.hpp"
#include "util/TextUtils.hpp"

namespace Bitwig
{

    PageTitleItem::PageTitleItem(lv_obj_t *parent)
        : parent_(parent)
    {
        // Lazy init - don't create LVGL widget here
    }

    PageTitleItem::~PageTitleItem()
    {
        // Don't delete anything - LVGL parent handles cleanup
    }

    void PageTitleItem::ensureCreated()
    {
        if (label_ || !parent_) return;

        label_ = lv_label_create(parent_);
        if (!label_) return;

        lv_obj_set_style_text_color(label_, lv_color_hex(Theme::Color::TEXT_LIGHT), 0);
        lv_obj_set_style_text_font(label_, bitwig_fonts.page_label, 0);
        lv_label_set_text(label_, pending_name_.empty() ? "Page" : pending_name_.c_str());
    }

    void PageTitleItem::setName(const std::string &name)
    {
        pending_name_ = name.empty() ? "Page" : TextUtils::sanitizeText(name.c_str()).c_str();

        ensureCreated();
        if (label_)
        {
            lv_label_set_text(label_, pending_name_.c_str());
        }
    }

} // namespace Bitwig
