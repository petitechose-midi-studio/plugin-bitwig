#include "BackButton.hpp"

#include <oc/ui/lvgl/style/StyleBuilder.hpp>

#include "ui/font/BitwigFonts.hpp"
#include "ui/font/icon.hpp"
#include "ui/theme/BitwigTheme.hpp"

namespace bitwig {

using namespace oc::ui::lvgl;
using namespace theme;
namespace style = oc::ui::lvgl::style;

BackButton::BackButton(lv_obj_t* parent) {
    label_ = lv_label_create(parent);
    lv_label_set_text(label_, Icon::UI_ARROW_LEFT);

    if (bitwig_fonts.icons_14) {
        lv_obj_set_style_text_font(label_, bitwig_fonts.icons_14, LV_STATE_DEFAULT);
    }

    style::apply(label_).textColor(Color::TEXT_LIGHT);

    // Hidden by default
    lv_obj_add_flag(label_, LV_OBJ_FLAG_HIDDEN);
    visible_ = false;
}

BackButton::~BackButton() {
    if (label_) {
        lv_obj_delete(label_);
        label_ = nullptr;
    }
}

void BackButton::setHighlighted(bool highlighted) {
    if (!label_) return;
    style::apply(label_).textColor(highlighted ? Color::TEXT_PRIMARY : Color::TEXT_LIGHT);
}

void BackButton::show() {
    if (!label_) return;
    lv_obj_clear_flag(label_, LV_OBJ_FLAG_HIDDEN);
    visible_ = true;
}

void BackButton::hide() {
    if (!label_) return;
    lv_obj_add_flag(label_, LV_OBJ_FLAG_HIDDEN);
    visible_ = false;
}

bool BackButton::isVisible() const {
    return visible_;
}

}  // namespace bitwig
