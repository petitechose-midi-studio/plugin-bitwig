#pragma once

#include "DeviceListOverlay.hpp"
#include "resource/common/ui/interface/IComponent.hpp"
#include <string>
#include <vector>

namespace Plugin::Bitwig {

/**
 * @brief Device selector component with hierarchical navigation
 *
 * Encapsulates ListOverlay for device selection with support for:
 * - Device list navigation
 * - Hierarchical child navigation (Slots/Layers/DrumPads)
 * - Multi-type folder selection
 * - Back navigation for nested contexts
 *
 * PURE UI COMPONENT - No logic, no callbacks, no protocol dependencies.
 * All data is pre-formatted by Controller.
 *
 * Usage (from DeviceController):
 *   // Set formatted device names
 *   deviceSelector.setItems({"Operator [S]", "EQ Eight ✓", "Compressor"});
 *   deviceSelector.setCurrentItemIndex(1);  // EQ Eight
 *   deviceSelector.show();
 *
 *   // Later, from controller:
 *   int selected = deviceSelector.getSelectedIndex();
 *   deviceSelector.hide();
 */
class DeviceSelector : public UI::IComponent {
public:
    /**
     * @brief Navigation mode for selector
     *
     * Used internally to track state, but not exposed to protocol.
     */
    enum class NavigationMode {
        DEVICES,    // List of devices in bank
        FOLDERS,    // List of child type folders (when multiple types)
        CHILDREN    // List of children (slots/layers/drum pads)
    };

    /**
     * @brief Construct device selector
     * @param parent Parent LVGL object (typically screen)
     */
    explicit DeviceSelector(lv_obj_t* parent);

    /**
     * @brief Destructor
     */
    ~DeviceSelector();

    /**
     * @brief Set selector title
     * @param title Title text (e.g., "Select Device", "Select Slot")
     */
    void setTitle(const std::string& title);

    /**
     * @brief Set list items (already formatted by controller)
     * @param items Vector of formatted item labels
     *
     * Items should already include:
     * - Current marker: "EQ Eight ✓"
     * - Back item: "[^ Back]" (if needed)
     * - Folder names: "[S] Slots", "[L] Layers"
     * - Child names: "Kick (C1)", "Layer 1"
     *
     * Note: For device mode, use setDeviceItems() to include colored indicators.
     */
    void setItems(const std::vector<std::string>& items);

    /**
     * @brief Set device items with state and children type indicators
     * @param names Device names (without [S]/[L]/[D] text indicators)
     * @param currentIndex Index of device with Bitwig focus
     * @param deviceStates Device enabled states (per device)
     * @param hasSlots Slot indicators per device
     * @param hasLayers Layer indicators per device
     * @param hasDrums Drum pad indicators per device
     *
     * Use this for DEVICES mode. Colored dots and state indicators will be added.
     * For FOLDERS/CHILDREN modes, use setItems() instead.
     */
    void setDeviceItems(const std::vector<std::string>& names,
                        int currentIndex,
                        const std::vector<bool>& deviceStates,
                        const std::vector<bool>& hasSlots,
                        const std::vector<bool>& hasLayers,
                        const std::vector<bool>& hasDrums);

    /**
     * @brief Set current item index (initial selection when showing)
     * @param index Current item index (0-based)
     *
     * Note: If back item is present at index 0, current item should be index 1+
     */
    void setCurrentItemIndex(int index);

    /**
     * @brief Set selected item index (navigation)
     * @param index Selected index (0-based), clamped to valid range
     */
    void setSelectedIndex(int index);

    /**
     * @brief Set navigation mode (for internal state tracking)
     * @param mode Navigation mode
     *
     * This is used by controller to track state, but doesn't affect UI directly.
     * The UI is driven by setItems() and setTitle().
     */
    void setMode(NavigationMode mode);

    /**
     * @brief Show device selector overlay (from IComponent)
     */
    void show() override;

    /**
     * @brief Hide device selector overlay (from IComponent)
     */
    void hide() override;

    /**
     * @brief Check if selector is visible (from IComponent)
     * @return true if visible
     */
    bool isVisible() const override;

    /**
     * @brief Get currently selected item index
     * @return Selected index (0-based), -1 if no items
     */
    int getSelectedIndex() const;

    /**
     * @brief Get number of items in list
     * @return Item count
     */
    int getItemCount() const;

    /**
     * @brief Get current navigation mode
     * @return Current mode (DEVICES, FOLDERS, or CHILDREN)
     */
    NavigationMode getMode() const { return mode_; }

    /**
     * @brief Set device state at specific index in list
     * @param deviceIndex Device index in bank
     * @param enabled Device enabled state
     */
    void setDeviceStateAtIndex(uint8_t deviceIndex, bool enabled);

    /**
     * @brief Get underlying LVGL element (from IElement)
     * @return Overlay object (nullptr if not created)
     */
    lv_obj_t* getElement() const override { return overlay_.getElement(); }

private:
    void createFooter();
    void destroyFooter();

    lv_obj_t* parent_;  // Parent screen for footer
    DeviceListOverlay overlay_;
    std::vector<std::string> items_;
    int current_item_index_ = 0;
    NavigationMode mode_ = NavigationMode::DEVICES;

    // Footer labels (shown in DEVICES mode only, at parent screen level)
    lv_obj_t* footer_container_ = nullptr;
    lv_obj_t* footer_left_label_ = nullptr;   // "Track"
    lv_obj_t* footer_center_label_ = nullptr; // "State"
};

}  // namespace Plugin::Bitwig
