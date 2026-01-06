#include "TitleItem.hpp"

#include <oc/ui/lvgl/style/StyleBuilder.hpp>

namespace bitwig::ui {

namespace style = oc::ui::lvgl::style;

TitleItem::TitleItem(lv_obj_t* parent) {
    if (!parent) return;

    // Create container with flex row layout
    container_ = lv_obj_create(parent);
    lv_obj_set_size(container_, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    style::apply(container_).transparent();

    // Flex row with gap
    lv_obj_set_flex_flow(container_, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(container_, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(container_, 6, LV_STATE_DEFAULT);

    // Create children inside container
    icon_ = lv_label_create(container_);
    lv_obj_add_flag(icon_, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(icon_, "");

    // Use framework Label widget with auto-scroll for overflow text
    label_ = std::make_unique<oc::ui::lvgl::Label>(container_);
    label_->alignment(LV_TEXT_ALIGN_LEFT)
           .ownsLvglObjects(false);

    indicator_ = lv_label_create(container_);
    lv_obj_add_flag(indicator_, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(indicator_, "");
}

TitleItem::~TitleItem() {
    // label_ is unique_ptr with ownsLvglObjects(false) - container deletion handles LVGL cleanup
    if (container_) {
        lv_obj_delete(container_);
        container_ = nullptr;
        icon_ = nullptr;
        indicator_ = nullptr;
    }
}

void TitleItem::render(const TitleItemProps& props) { applyProps(props); }

void TitleItem::applyProps(const TitleItemProps& props) {
    if (!container_) return;

    if (!props.visible) {
        lv_obj_add_flag(container_, LV_OBJ_FLAG_HIDDEN);
        return;
    }

    lv_obj_clear_flag(container_, LV_OBJ_FLAG_HIDDEN);

    // Icon
    if (icon_) {
        if (props.icon && props.icon[0] != '\0') {
            lv_label_set_text(icon_, props.icon);
            style::apply(icon_).textColor(props.iconColor);
            lv_obj_set_style_text_opa(icon_, props.iconOpacity, LV_STATE_DEFAULT);
            if (props.iconFont) {
                lv_obj_set_style_text_font(icon_, props.iconFont, LV_STATE_DEFAULT);
            }
            lv_obj_clear_flag(icon_, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_add_flag(icon_, LV_OBJ_FLAG_HIDDEN);
        }
    }

    // Label
    if (label_) {
        label_->setText(props.text ? props.text : "");
        label_->color(props.textColor);
        lv_obj_set_style_text_opa(label_->getLabel(), props.textOpacity, LV_STATE_DEFAULT);
        if (props.textFont) {
            label_->font(props.textFont);
        }
        lv_obj_clear_flag(label_->getElement(), LV_OBJ_FLAG_HIDDEN);
    }

    // Indicator
    if (indicator_) {
        if (props.showIndicator && props.indicator && props.indicator[0] != '\0') {
            lv_label_set_text(indicator_, props.indicator);
            style::apply(indicator_).textColor(props.indicatorColor);
            if (props.iconFont) {
                lv_obj_set_style_text_font(indicator_, props.iconFont, LV_STATE_DEFAULT);
            }
            lv_obj_clear_flag(indicator_, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_add_flag(indicator_, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

lv_coord_t TitleItem::getContentWidth() const {
    if (!container_) return 0;
    lv_obj_update_layout(container_);
    return lv_obj_get_width(container_);
}

}  // namespace bitwig::ui
