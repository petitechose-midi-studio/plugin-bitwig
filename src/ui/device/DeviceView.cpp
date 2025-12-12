#include "DeviceView.hpp"

#include <oc/log/Log.hpp>

#include "handler/DeviceConstants.hpp"
#include "ui/theme/BitwigTheme.hpp"
#include "ui/theme/StyleHelpers.hpp"
#include "ui/widget/ParameterButtonWidget.hpp"
#include "ui/widget/ParameterKnobWidget.hpp"
#include "ui/widget/ParameterListWidget.hpp"

using namespace Theme;

namespace Bitwig {

// =============================================================================
// Construction / Destruction
// =============================================================================

DeviceView::DeviceView(lv_obj_t* zone, bitwig::state::BitwigState& state)
    : state_(state), zone_(zone) {
    OC_LOG_INFO("[DeviceView] Creating...");

    for (auto& widget : widgets_) {
        widget = nullptr;
    }

    createUI();
    setupBindings();
    initialized_ = true;

    OC_LOG_INFO("[DeviceView] Created with {} subscriptions", subs_.size());
}

DeviceView::~DeviceView() {
    // Subscriptions auto-unsubscribe via RAII
    subs_.clear();

    for (auto& widget : widgets_) {
        widget.reset();
    }

    if (top_bar_container_) {
        lv_obj_delete(top_bar_container_);
        top_bar_container_ = nullptr;
    }
    if (body_container_) {
        lv_obj_delete(body_container_);
        body_container_ = nullptr;
    }
}

// =============================================================================
// IView Lifecycle
// =============================================================================

void DeviceView::onActivate() {
    if (top_bar_container_) lv_obj_clear_flag(top_bar_container_, LV_OBJ_FLAG_HIDDEN);
    if (body_container_) lv_obj_clear_flag(body_container_, LV_OBJ_FLAG_HIDDEN);
}

void DeviceView::onDeactivate() {
    if (top_bar_container_) lv_obj_add_flag(top_bar_container_, LV_OBJ_FLAG_HIDDEN);
    if (body_container_) lv_obj_add_flag(body_container_, LV_OBJ_FLAG_HIDDEN);
}

// =============================================================================
// Signal Bindings (Reactive Architecture)
// =============================================================================

void DeviceView::setupBindings() {
    OC_LOG_DEBUG("[DeviceView] Setting up signal bindings...");

    // =========================================================================
    // Device Info → Top Bar
    // =========================================================================
    // Subscribe to all device info signals - any change triggers full redraw
    subs_.push_back(state_.device.name.subscribe([this](const char* name) {
        OC_LOG_DEBUG("[DeviceView] signal: device.name = '{}'", name);
        updateDeviceInfo();
    }));
    subs_.push_back(state_.device.pageName.subscribe([this](const char* name) {
        OC_LOG_DEBUG("[DeviceView] signal: device.pageName = '{}'", name);
        updateDeviceInfo();
    }));
    subs_.push_back(state_.device.deviceType.subscribe([this](bitwig::state::DeviceType type) {
        OC_LOG_DEBUG("[DeviceView] signal: device.type = {}", static_cast<int>(type));
        updateDeviceInfo();
    }));
    subs_.push_back(state_.device.enabled.subscribe([this](bool enabled) {
        OC_LOG_DEBUG("[DeviceView] signal: device.enabled = {}", enabled);
        updateDeviceInfo();
    }));
    subs_.push_back(state_.device.hasChildren.subscribe([this](bool has) {
        OC_LOG_DEBUG("[DeviceView] signal: device.hasChildren = {}", has);
        updateDeviceInfo();
    }));

    // =========================================================================
    // Parameters → Widgets (8 slots)
    // =========================================================================
    for (uint8_t i = 0; i < 8; i++) {
        auto& slot = state_.parameters.slots[i];

        // Type change triggers widget creation/recreation
        subs_.push_back(slot.type.subscribe([this, i](bitwig::state::ParameterType type) {
            OC_LOG_DEBUG("[DeviceView] signal: param[{}].type = {}", i, static_cast<int>(type));
            ensureWidgetForType(i);
            updateParameter(i);
        }));

        // Value/display changes
        subs_.push_back(slot.value.subscribe([this, i](float val) {
            OC_LOG_DEBUG("[DeviceView] signal: param[{}].value = {:.3f}", i, val);
            updateParameter(i);
        }));
        subs_.push_back(slot.name.subscribe([this, i](const char* name) {
            OC_LOG_DEBUG("[DeviceView] signal: param[{}].name = '{}'", i, name);
            updateParameter(i);
        }));
        subs_.push_back(slot.displayValue.subscribe([this, i](const char* val) {
            OC_LOG_DEBUG("[DeviceView] signal: param[{}].displayValue = '{}'", i, val);
            updateParameter(i);
        }));
        subs_.push_back(slot.visible.subscribe([this, i](bool vis) {
            OC_LOG_DEBUG("[DeviceView] signal: param[{}].visible = {}", i, vis);
            updateParameter(i);
        }));
        subs_.push_back(slot.currentValueIndex.subscribe([this, i](uint8_t idx) {
            OC_LOG_DEBUG("[DeviceView] signal: param[{}].currentValueIndex = {}", i, idx);
            updateParameter(i);
        }));

        // Discrete values list change
        subs_.push_back(slot.discreteValues.subscribe([this, i]() {
            OC_LOG_DEBUG("[DeviceView] signal: param[{}].discreteValues changed", i);
            updateParameter(i);
        }));
    }

    // =========================================================================
    // Page Selector
    // =========================================================================
    subs_.push_back(state_.pageSelector.names.subscribe([this]() {
        OC_LOG_DEBUG("[DeviceView] signal: pageSelector.names changed");
        updatePageSelector();
    }));
    subs_.push_back(state_.pageSelector.selectedIndex.subscribe([this](int8_t idx) {
        OC_LOG_DEBUG("[DeviceView] signal: pageSelector.selectedIndex = {}", idx);
        updatePageSelector();
    }));
    subs_.push_back(state_.pageSelector.visible.subscribe([this](bool vis) {
        OC_LOG_DEBUG("[DeviceView] signal: pageSelector.visible = {}", vis);
        updatePageSelector();
    }));

    // =========================================================================
    // Device Selector
    // =========================================================================
    subs_.push_back(state_.deviceSelector.names.subscribe([this]() {
        OC_LOG_DEBUG("[DeviceView] signal: deviceSelector.names changed");
        updateDeviceSelector();
    }));
    subs_.push_back(state_.deviceSelector.visible.subscribe([this](bool vis) {
        OC_LOG_DEBUG("[DeviceView] signal: deviceSelector.visible = {}", vis);
        updateDeviceSelector();
    }));
    subs_.push_back(state_.deviceSelector.currentIndex.subscribe([this](int8_t idx) {
        OC_LOG_DEBUG("[DeviceView] signal: deviceSelector.currentIndex = {}", idx);
        updateDeviceSelector();
    }));
    subs_.push_back(state_.deviceSelector.showingChildren.subscribe([this](bool show) {
        OC_LOG_DEBUG("[DeviceView] signal: deviceSelector.showingChildren = {}", show);
        updateDeviceSelector();
    }));
    subs_.push_back(state_.deviceSelector.showFooter.subscribe([this](bool show) {
        OC_LOG_DEBUG("[DeviceView] signal: deviceSelector.showFooter = {}", show);
        updateDeviceSelector();
    }));
    subs_.push_back(state_.deviceSelector.childrenNames.subscribe([this]() {
        OC_LOG_DEBUG("[DeviceView] signal: deviceSelector.childrenNames changed");
        updateDeviceSelector();
    }));

    // =========================================================================
    // Track Selector
    // =========================================================================
    subs_.push_back(state_.trackSelector.names.subscribe([this]() {
        OC_LOG_DEBUG("[DeviceView] signal: trackSelector.names changed");
        updateTrackSelector();
    }));
    subs_.push_back(state_.trackSelector.visible.subscribe([this](bool vis) {
        OC_LOG_DEBUG("[DeviceView] signal: trackSelector.visible = {}", vis);
        updateTrackSelector();
    }));
    subs_.push_back(state_.trackSelector.currentIndex.subscribe([this](int8_t idx) {
        OC_LOG_DEBUG("[DeviceView] signal: trackSelector.currentIndex = {}", idx);
        updateTrackSelector();
    }));

    OC_LOG_DEBUG("[DeviceView] Bound {} subscriptions", subs_.size());
}

// =============================================================================
// Update Helpers (called by subscription callbacks)
// =============================================================================

void DeviceView::updateDeviceInfo() {
    if (!initialized_ || !top_bar_component_) return;

    OC_LOG_DEBUG("[DeviceView] >> updateDeviceInfo()");
    top_bar_component_->render({
        .deviceName = state_.device.name.get(),
        .deviceType = static_cast<uint8_t>(state_.device.deviceType.get()),
        .deviceEnabled = state_.device.enabled.get(),
        .deviceHasChildren = state_.device.hasChildren.get(),
        .pageName = state_.device.pageName.get()
    });
}

void DeviceView::ensureWidgetForType(uint8_t index) {
    if (index >= 8 || !body_container_) return;

    auto& slot = state_.parameters.slots[index];
    auto type = slot.type.get();

    // Reset existing widget
    widgets_[index].reset();

    switch (type) {
        case bitwig::state::ParameterType::BUTTON:
            widgets_[index] = std::make_unique<ParameterButtonWidget>(
                body_container_, Layout::WIDGET_WIDTH, Layout::WIDGET_HEIGHT, index);
            break;

        case bitwig::state::ParameterType::LIST:
            widgets_[index] = std::make_unique<ParameterListWidget>(
                body_container_, Layout::WIDGET_WIDTH, Layout::WIDGET_HEIGHT, index,
                slot.discreteCount.get());
            break;

        case bitwig::state::ParameterType::KNOB:
        default: {
            float origin = slot.origin.get();
            bool is_centered = (origin == 0.5f);
            auto knob = std::make_unique<ParameterKnobWidget>(
                body_container_, Layout::WIDGET_WIDTH, Layout::WIDGET_HEIGHT, index, is_centered);
            knob->setOrigin(origin);
            widgets_[index] = std::move(knob);
            break;
        }
    }

    // Position in grid
    if (widgets_[index]) {
        int col = index % 4;
        int row = index / 4;
        lv_obj_set_grid_cell(widgets_[index]->getContainer(), LV_GRID_ALIGN_CENTER, col, 1,
                             LV_GRID_ALIGN_CENTER, row, 1);
    }
}

void DeviceView::updateParameter(uint8_t index) {
    if (!initialized_ || index >= 8 || !widgets_[index]) return;

    auto& slot = state_.parameters.slots[index];
    auto type = slot.type.get();

    // Update name and value
    widgets_[index]->setName(slot.name.get());
    widgets_[index]->setValueWithDisplay(slot.value.get(), slot.displayValue.get());

    // Update discrete metadata for button/list widgets
    if (type == bitwig::state::ParameterType::BUTTON ||
        type == bitwig::state::ParameterType::LIST) {
        // Convert SignalVector to std::vector for widget API
        std::vector<std::string> discreteNames(
            slot.discreteValues.begin(), slot.discreteValues.end());
        widgets_[index]->setDiscreteMetadata(
            slot.discreteCount.get(), discreteNames, slot.currentValueIndex.get());
    }

    // Update visibility
    lv_obj_t* container = widgets_[index]->getContainer();
    if (container) {
        if (slot.visible.get()) {
            lv_obj_clear_flag(container, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_add_flag(container, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

void DeviceView::updatePageSelector() {
    if (!initialized_ || !page_selector_) return;

    bool visible = state_.pageSelector.visible.get();
    OC_LOG_DEBUG("[DeviceView] >> updatePageSelector() visible={}", visible);

    // Convert SignalVector to std::vector
    std::vector<std::string> names(
        state_.pageSelector.names.begin(), state_.pageSelector.names.end());

    page_selector_->render({
        .names = &names,
        .selectedIndex = state_.pageSelector.selectedIndex.get(),
        .visible = visible
    });
}

void DeviceView::updateDeviceSelector() {
    if (!initialized_ || !device_selector_) return;

    bool visible = state_.deviceSelector.visible.get();
    OC_LOG_DEBUG("[DeviceView] >> updateDeviceSelector() visible={}", visible);

    // Convert SignalVectors to std::vectors
    std::vector<std::string> names(
        state_.deviceSelector.names.begin(), state_.deviceSelector.names.end());
    std::vector<uint8_t> deviceTypes(
        state_.deviceSelector.deviceTypes.begin(), state_.deviceSelector.deviceTypes.end());
    std::vector<bool> hasSlots(
        state_.deviceSelector.hasSlots.begin(), state_.deviceSelector.hasSlots.end());
    std::vector<bool> hasLayers(
        state_.deviceSelector.hasLayers.begin(), state_.deviceSelector.hasLayers.end());
    std::vector<bool> hasDrums(
        state_.deviceSelector.hasDrums.begin(), state_.deviceSelector.hasDrums.end());
    std::vector<std::string> childrenNames(
        state_.deviceSelector.childrenNames.begin(), state_.deviceSelector.childrenNames.end());
    std::vector<uint8_t> childrenTypes(
        state_.deviceSelector.childrenTypes.begin(), state_.deviceSelector.childrenTypes.end());

    // Device states need special handling (array of Signals)
    std::vector<bool> deviceStates;
    deviceStates.reserve(names.size());
    for (size_t i = 0; i < names.size(); i++) {
        deviceStates.push_back(state_.deviceSelector.deviceStates[i].get());
    }

    device_selector_->render({
        .names = &names,
        .deviceTypes = &deviceTypes,
        .deviceStates = &deviceStates,
        .hasSlots = &hasSlots,
        .hasLayers = &hasLayers,
        .hasDrums = &hasDrums,
        .childrenNames = &childrenNames,
        .childrenTypes = &childrenTypes,
        .trackName = state_.currentTrack.name.get(),
        .trackColor = state_.currentTrack.color.get(),
        .trackType = static_cast<uint8_t>(state_.currentTrack.trackType.get()),
        .selectedIndex = state_.deviceSelector.currentIndex.get(),
        .showingChildren = state_.deviceSelector.showingChildren.get(),
        .showFooter = state_.deviceSelector.showFooter.get(),
        .visible = visible
    });
}

void DeviceView::updateTrackSelector() {
    if (!initialized_ || !track_selector_) return;

    bool visible = state_.trackSelector.visible.get();
    OC_LOG_DEBUG("[DeviceView] >> updateTrackSelector() visible={}", visible);

    // Convert SignalVectors to std::vectors
    std::vector<std::string> names(
        state_.trackSelector.names.begin(), state_.trackSelector.names.end());
    std::vector<uint8_t> trackTypes(
        state_.trackSelector.trackTypes.begin(), state_.trackSelector.trackTypes.end());
    std::vector<uint32_t> trackColors(
        state_.trackSelector.trackColors.begin(), state_.trackSelector.trackColors.end());

    // Mute/Solo states need special handling (array of Signals)
    std::vector<bool> muteStates, soloStates;
    muteStates.reserve(names.size());
    soloStates.reserve(names.size());
    for (size_t i = 0; i < names.size(); i++) {
        muteStates.push_back(state_.trackSelector.muteStates[i].get());
        soloStates.push_back(state_.trackSelector.soloStates[i].get());
    }

    track_selector_->render({
        .names = &names,
        .muteStates = &muteStates,
        .soloStates = &soloStates,
        .trackTypes = &trackTypes,
        .trackColors = &trackColors,
        .selectedIndex = state_.trackSelector.currentIndex.get(),
        .visible = visible
    });
}

// =============================================================================
// UI Creation
// =============================================================================

void DeviceView::createUI() {
    top_bar_container_ = lv_obj_create(zone_);
    lv_obj_set_size(top_bar_container_, LV_PCT(100), Layout::TOP_BAR_HEIGHT);
    Style::applyTransparentContainer(top_bar_container_);
    lv_obj_set_style_radius(top_bar_container_, 0, LV_STATE_DEFAULT);

    body_container_ = lv_obj_create(zone_);
    lv_obj_set_size(body_container_, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_grow(body_container_, 1);
    Style::applyTransparentContainer(body_container_);
    lv_obj_set_style_radius(body_container_, 0, LV_STATE_DEFAULT);

    createDeviceStateBar();
    setupLayout();
}

void DeviceView::setupLayout() {
    if (!body_container_) return;

    static const lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1),
                                         LV_GRID_TEMPLATE_LAST};
    static const lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    lv_obj_set_layout(body_container_, LV_LAYOUT_GRID);
    lv_obj_set_grid_dsc_array(body_container_, col_dsc, row_dsc);
}

void DeviceView::createDeviceStateBar() {
    if (!top_bar_container_) return;

    top_bar_component_ = std::make_unique<DeviceStateBar>(top_bar_container_);

    // Initial render with defaults
    top_bar_component_->render({
        .deviceName = "No Device",
        .deviceEnabled = false,
        .deviceHasChildren = false,
        .pageName = ""
    });

    page_selector_ = std::make_unique<PageSelector>(zone_);
    device_selector_ = std::make_unique<DeviceSelector>(zone_);
    track_selector_ = std::make_unique<TrackSelector>(zone_);
}

// =============================================================================
// Overlay Element Accessors
// =============================================================================

lv_obj_t* DeviceView::getPageSelectorElement() const {
    if (!page_selector_) return nullptr;
    return page_selector_->getElement();
}

lv_obj_t* DeviceView::getDeviceSelectorElement() const {
    if (!device_selector_) return nullptr;
    return device_selector_->getElement();
}

lv_obj_t* DeviceView::getTrackSelectorElement() const {
    if (!track_selector_) return nullptr;
    return track_selector_->getElement();
}

}  // namespace Bitwig
