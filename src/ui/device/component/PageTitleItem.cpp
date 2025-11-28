#include "PageTitleItem.hpp"
#include "../../theme/BitwigTheme.hpp"
#include "ui/font/FontLoader.hpp"
#include "util/TextUtils.hpp"

namespace Bitwig
{

    PageTitleItem::PageTitleItem(lv_obj_t *parent)
    {
        label_ = std::make_unique<Label>(parent);
        label_->setFlexGrow(false);  // Don't expand, allow right-alignment
        label_->setAutoScroll(false);  // Page names are usually short
        label_->setColor(lv_color_hex(Theme::Color::TEXT_LIGHT));
        label_->setFont(bitwig_fonts.page_label);
    }

    PageTitleItem::~PageTitleItem()
    {
        // unique_ptr handles cleanup
    }

    void PageTitleItem::setName(const std::string &name)
    {
        if (label_)
        {
            if (name.empty())
            {
                label_->setText("Page");
            }
            else
            {
                std::string clean = TextUtils::sanitizeText(name.c_str()).c_str();
                label_->setText(clean);
            }
        }
    }

} // namespace Bitwig
