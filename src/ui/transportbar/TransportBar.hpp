#pragma once

#include <memory>
#include <vector>

#include <lvgl.h>
#include <oc/state/Signal.hpp>
#include <oc/ui/lvgl/IComponent.hpp>
#include <oc/ui/lvgl/widget/Label.hpp>
#include <oc/ui/lvgl/widget/StateIndicator.hpp>

#include "state/TransportState.hpp"

namespace bitwig::ui {

using oc::state::Subscription;
using oc::ui::lvgl::IComponent;
using oc::ui::lvgl::StateIndicator;

/**
 * @brief Transport bar component with reactive state bindings
 *
 * Displays play/record/tempo status and MIDI activity indicators.
 * Subscribes to TransportState signals and auto-updates on changes.
 */
class TransportBar : public IComponent {
public:
    /**
     * @brief Construct TransportBar with state reference
     * @param parent LVGL parent object
     * @param state Transport state to observe (must outlive this component)
     */
    TransportBar(lv_obj_t* parent, bitwig::state::TransportState& state);
    ~TransportBar();

    // Non-copyable, non-movable (owns subscriptions)
    TransportBar(const TransportBar&) = delete;
    TransportBar& operator=(const TransportBar&) = delete;

    // IComponent interface
    void show() override;
    void hide() override;
    bool isVisible() const override;
    lv_obj_t* getElement() const override { return container_; }
private:
    // State & Subscriptions
    bitwig::state::TransportState& state_;
    std::vector<Subscription> subs_;

    // UI elements
    lv_obj_t* parent_ = nullptr;
    lv_obj_t* container_ = nullptr;
    std::unique_ptr<StateIndicator> midi_in_indicator_;
    std::unique_ptr<StateIndicator> midi_out_indicator_;
    lv_obj_t* play_icon_ = nullptr;
    lv_obj_t* stop_icon_ = nullptr;
    lv_obj_t* record_icon_ = nullptr;
    lv_obj_t* automation_override_icon_ = nullptr;
    std::unique_ptr<oc::ui::lvgl::Label> bpm_label_;

    // Setup
    void createContainer(lv_obj_t* parent);
    void createTransportControls();
    void createTempoDisplay();
    void setupBindings();

    // Render (initial sync)
    void render();

    // Granular updates (called by signal callbacks)
    void setPlayState(bool playing);
    void setRecordState(bool recording);
    void setTempo(float tempo);
    void setMidiIn(bool active);
    void setMidiOut(bool active);
    void setAutomationOverride(bool active);
};

}  // namespace bitwig::ui
