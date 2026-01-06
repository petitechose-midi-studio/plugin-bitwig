#pragma once

/**
 * @file MixView.hpp
 * @brief Placeholder view for mixer controls (volume, pan, sends)
 *
 * This is a placeholder that will be implemented with:
 * - Volume/Pan controls for current track
 * - Send levels
 * - Track selection integration
 */

#include <memory>

#include <lvgl.h>

#include <oc/ui/lvgl/IView.hpp>
#include <oc/ui/lvgl/style/StyleBuilder.hpp>
#include <oc/ui/lvgl/widget/Label.hpp>

#include "ui/theme/BitwigTheme.hpp"

namespace bitwig::ui {

using oc::ui::lvgl::IView;
namespace style = oc::ui::lvgl::style;

/**
 * @brief Mixer view for track volume, pan, and sends
 *
 * Currently a placeholder displaying "Mix View - Coming Soon"
 */
class MixView : public IView {
public:
    explicit MixView(lv_obj_t* zone) : zone_(zone) {
        createUI();
    }

    ~MixView() {
        if (container_) {
            lv_obj_delete(container_);
            container_ = nullptr;
        }
    }

    // Non-copyable, non-movable
    MixView(const MixView&) = delete;
    MixView& operator=(const MixView&) = delete;
    MixView(MixView&&) = delete;
    MixView& operator=(MixView&&) = delete;

    // =========================================================================
    // IView interface
    // =========================================================================

    void onActivate() override {
        if (container_) lv_obj_clear_flag(container_, LV_OBJ_FLAG_HIDDEN);
    }

    void onDeactivate() override {
        if (container_) lv_obj_add_flag(container_, LV_OBJ_FLAG_HIDDEN);
    }

    const char* getViewId() const override { return "bitwig.mix"; }

    lv_obj_t* getElement() const override { return zone_; }

private:
    void createUI() {
        container_ = lv_obj_create(zone_);
        lv_obj_set_size(container_, LV_PCT(100), LV_PCT(100));
        style::apply(container_).transparent().noScroll();

        // Center label placeholder using framework Label
        label_ = std::make_unique<oc::ui::lvgl::Label>(container_);
        label_->alignment(LV_TEXT_ALIGN_CENTER)
              .color(theme::Color::TEXT_LIGHT)
              .ownsLvglObjects(false);
        label_->setText("Mix View\nComing Soon");
        lv_obj_center(label_->getElement());

        // Start hidden (ViewManager will activate)
        lv_obj_add_flag(container_, LV_OBJ_FLAG_HIDDEN);
    }

    lv_obj_t* zone_;       // Parent zone (non-owned)
    lv_obj_t* container_{nullptr};
    std::unique_ptr<oc::ui::lvgl::Label> label_;
};

}  // namespace bitwig::ui
