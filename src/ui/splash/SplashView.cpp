#include "SplashView.hpp"

#include <oc/ui/lvgl/style/StyleBuilder.hpp>

#include "ui/font/BitwigFonts.hpp"
#include "ui/theme/BitwigTheme.hpp"

LV_IMG_DECLARE(Bitwig_Logo);

namespace bitwig {

using namespace bitwig::theme;
namespace style = oc::ui::lvgl::style;

SplashView::SplashView(lv_obj_t *zone) : zone_(zone) {}

SplashView::~SplashView() { destroyUI(); }

void SplashView::onActivate() {
    if (!container_) { createUI(); }

    if (container_) {
        lv_obj_clear_flag(container_, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_style_opa(container_, Opacity::FULL,
                             LV_STATE_DEFAULT);  // Reset opacity after fadeOut
    }
}

void SplashView::onDeactivate() {
    if (container_) { lv_obj_add_flag(container_, LV_OBJ_FLAG_HIDDEN); }
}

void SplashView::setText(const char *message) {
    if (!container_) { createUI(); }
    if (label_) { label_->setText(message ? message : ""); }
}

void SplashView::createUI() {
    container_ = lv_obj_create(zone_);
    style::apply(container_).fullSize().pad(0).bgColor(Color::BACKGROUND_BASE);
    lv_obj_set_style_radius(container_, 0, LV_STATE_DEFAULT);

    lv_obj_add_flag(container_, LV_OBJ_FLAG_FLOATING);
    lv_obj_set_pos(container_, 0, 0);

    lv_obj_set_flex_flow(container_, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container_, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER);

    logo_ = lv_image_create(container_);
    lv_image_set_src(logo_, &Bitwig_Logo);
    lv_obj_set_style_img_opa(logo_, Opacity::FULL, LV_STATE_DEFAULT);

    // Use framework Label widget with auto-scroll for overflow text
    label_ = std::make_unique<oc::ui::lvgl::Label>(container_);
    label_->alignment(LV_TEXT_ALIGN_CENTER)
           .color(Color::TEXT_PRIMARY)
           .ownsLvglObjects(false);

    if (bitwig_fonts.device_label) {
        label_->font(bitwig_fonts.device_label);
    }

    lv_obj_set_style_pad_top(label_->getElement(), 15, LV_STATE_DEFAULT);

    lv_obj_add_flag(container_, LV_OBJ_FLAG_HIDDEN);
}

void SplashView::destroyUI() {
    // label_ is unique_ptr with ownsLvglObjects(false) - container deletion handles LVGL cleanup
    label_.reset();
    if (container_) {
        lv_obj_delete(container_);
        container_ = nullptr;
    }
}

void SplashView::fadeOut(uint32_t durationMs, std::function<void()> onComplete) {
    if (!container_) {
        if (onComplete) onComplete();
        return;
    }

    fade_callback_ = std::move(onComplete);

    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, container_);
    lv_anim_set_exec_cb(&anim, fadeAnimCallback);
    lv_anim_set_values(&anim, Opacity::FULL, Opacity::HIDDEN);
    lv_anim_set_duration(&anim, durationMs);
    lv_anim_set_path_cb(&anim, lv_anim_path_linear);
    lv_anim_set_user_data(&anim, this);
    lv_anim_set_deleted_cb(&anim, fadeAnimDeletedCallback);
    lv_anim_start(&anim);
}

void SplashView::fadeAnimCallback(void *var, int32_t value) {
    lv_obj_t *obj = (lv_obj_t *)var;
    lv_obj_set_style_opa(obj, (lv_opa_t)value, LV_STATE_DEFAULT);
}

void SplashView::fadeAnimDeletedCallback(lv_anim_t *anim) {
    auto *self = static_cast<SplashView *>(lv_anim_get_user_data(anim));
    if (self && self->fade_callback_) {
        self->fade_callback_();
        self->fade_callback_ = nullptr;
    }
}

}  // namespace bitwig
