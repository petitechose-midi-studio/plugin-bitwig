#pragma once

#include <memory>

#include <lvgl.h>

#include <oc/ui/lvgl/IComponent.hpp>
#include <oc/ui/lvgl/widget/StateIndicator.hpp>

#include "ui/state/TransportState.hpp"

namespace Bitwig {

using oc::ui::lvgl::IComponent;
using oc::ui::lvgl::StateIndicator;

class TransportBar : public IComponent {
public:
    explicit TransportBar(lv_obj_t* parent);
    ~TransportBar();

    // Props/Render pattern
    void render(const TransportBarProps& props);

    // IComponent interface
    void show() override;
    void hide() override;
    bool isVisible() const override;
    lv_obj_t* getElement() const override { return container_; }
private:
    void createContainer(lv_obj_t* parent);
    void createTransportControls();
    void createTempoDisplay();

    lv_obj_t* parent_ = nullptr;
    lv_obj_t* container_ = nullptr;
    std::unique_ptr<StateIndicator> midi_in_indicator_;
    std::unique_ptr<StateIndicator> midi_out_indicator_;
    lv_obj_t* play_icon_ = nullptr;
    lv_obj_t* stop_icon_ = nullptr;
    lv_obj_t* record_icon_ = nullptr;
    lv_obj_t* bpm_label_ = nullptr;
};

}  // namespace Bitwig
