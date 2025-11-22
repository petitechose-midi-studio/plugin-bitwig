#pragma once

#include "widget/ListOverlay.hpp"
#include "interface/IComponent.hpp"
#include <vector>
#include <string>
#include <cstdint>

namespace Bitwig
{

    /**
     * @brief Bitwig-specific list overlay with device children type indicators
     *
     * Wraps ListOverlay and adds colored dots after labels to indicate
     * device children types (slots, layers, drum pads).
     *
     * Usage:
     *   DeviceListOverlay overlay(parent);
     *   overlay.setTitle("Select Device");
     *
     *   std::vector<std::string> names = {"Polymer", "Drum Machine"};
     *   std::vector<bool> hasSlots = {true, false};
     *   std::vector<bool> hasLayers = {false, false};
     *   std::vector<bool> hasDrums = {false, true};
     *
     *   overlay.setDeviceItems(names, hasSlots, hasLayers, hasDrums);
     *   overlay.show();
     */
    class DeviceListOverlay : public UI::IComponent
    {
    public:
        /**
         * @brief Construct device list overlay
         * @param parent Parent LVGL object (typically screen)
         */
        explicit DeviceListOverlay(lv_obj_t *parent);

        /**
         * @brief Destructor
         */
        ~DeviceListOverlay();

        /**
         * @brief Set overlay title
         * @param title Title text (empty to hide title)
         */
        void setTitle(const std::string &title);

        /**
         * @brief Set items without indicators (for non-device lists)
         * @param items Item names
         *
         * Use this for folders, children, or any list that doesn't need indicators.
         */
        void setItems(const std::vector<std::string> &items);

        /**
         * @brief Set device items with state and children type indicators
         * @param names Device names
         * @param currentIndex Index of device with Bitwig focus
         * @param deviceStates Device enabled states per device
         * @param hasSlots Slot indicators per device
         * @param hasLayers Layer indicators per device
         * @param hasDrums Drum pad indicators per device
         *
         * Note: All vectors should have the same size. Missing entries = false.
         */
        void setDeviceItems(const std::vector<std::string> &names,
                            int currentIndex,
                            const std::vector<bool> &deviceStates,
                            const std::vector<bool> &hasSlots = {},
                            const std::vector<bool> &hasLayers = {},
                            const std::vector<bool> &hasDrums = {});

        /**
         * @brief Set selected item index
         * @param index Item index (0-based), wrapped modulo
         */
        void setSelectedIndex(int index);

        /**
         * @brief Show overlay (from IComponent)
         */
        void show() override;

        /**
         * @brief Hide overlay (from IComponent)
         */
        void hide() override;

        /**
         * @brief Check if overlay is currently visible (from IComponent)
         * @return true if visible
         */
        bool isVisible() const override;

        /**
         * @brief Get currently selected index
         * @return Selected index (0-based), -1 if no items
         */
        int getSelectedIndex() const;

        /**
         * @brief Get number of items in list
         * @return Item count
         */
        int getItemCount() const;

        /**
         * @brief Set device state at specific index (updates bullet visibility)
         * @param displayIndex Display index in list (already accounts for nested mode)
         * @param enabled Device enabled state
         */
        void setDeviceStateAtIndex(int displayIndex, bool enabled);

        /**
         * @brief Get underlying LVGL element (from IElement)
         * @return Overlay object (nullptr if not created)
         */
        lv_obj_t *getElement() const override { return list_.getElement(); }

    private:
        void createIndicators();
        void clearIndicators();
        void updateBulletStates();

        bool isNonDeviceItem(size_t index) const;
        bool hasChildren(size_t index) const;
        lv_obj_t* createDot(lv_obj_t* parent, uint32_t color);
        lv_obj_t* createFolderIcon(lv_obj_t* parent);

        ListOverlay list_;

        std::vector<std::string> item_names_;

        std::vector<bool> device_states_;
        int current_device_index_ = -1;
        std::vector<bool> has_slots_;
        std::vector<bool> has_layers_;
        std::vector<bool> has_drums_;

        // Store folder icons: [device_index]
        std::vector<lv_obj_t *> folder_icons_;
    };

} // namespace Bitwig
