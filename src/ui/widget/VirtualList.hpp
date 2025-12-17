#pragma once

/**
 * @file VirtualList.hpp
 * @brief Virtual scrolling list widget with slot pooling
 *
 * Renders only visible items using a fixed pool of reusable slots.
 * Supports lists of arbitrary size with O(1) rendering performance.
 *
 * Usage:
 *   VirtualList list(parent, 12, 32);
 *   list.setOnBindSlot([](VirtualSlot& slot, int index, bool selected) {
 *       // Create/update widgets in slot.container for items[index]
 *   });
 *   list.setTotalCount(64);
 *   list.setSelectedIndex(0);
 *   list.show();
 */

#include <functional>
#include <vector>

#include <lvgl.h>

#include <oc/ui/lvgl/IComponent.hpp>

namespace bitwig::ui {

/**
 * @brief A reusable slot in the VirtualList
 */
struct VirtualSlot {
    lv_obj_t* container = nullptr;  ///< LVGL container (created by VirtualList)
    int boundIndex = -1;            ///< Currently bound logical index (-1 = unbound)
    void* userData = nullptr;       ///< Free pointer for owner (reusable widgets)
};

/**
 * @brief Callback to bind a slot to a logical index
 *
 * @param slot       The slot to bind (container + userData)
 * @param index      The logical index of the item to display
 * @param isSelected true if this item is currently selected
 *
 * The owner MUST:
 * - Reuse/update existing widgets in slot.userData
 * - OR create widgets if slot.userData == nullptr (first bind)
 * - Apply highlighted style if isSelected == true
 */
using BindSlotCallback = std::function<void(VirtualSlot& slot, int index, bool isSelected)>;

/**
 * @brief Optional callback to update only the highlight state
 *
 * Called when only the selection state changes (not the bound index).
 * If not provided, onBindSlot is called instead.
 */
using UpdateHighlightCallback = std::function<void(VirtualSlot& slot, bool isSelected)>;

/**
 * @brief Virtual scrolling list with slot pooling
 *
 * Only renders visible items (visibleCount slots), reusing them as the
 * selection moves through a list of totalCount items.
 */
class VirtualList : public oc::ui::lvgl::IComponent {
public:
    // ══════════════════════════════════════════════════════════════════
    // Construction
    // ══════════════════════════════════════════════════════════════════

    /**
     * @param parent        Parent LVGL object
     * @param visibleCount  Number of visible slots (default: 12)
     * @param itemHeight    Fixed height per item in pixels (default: 32)
     */
    explicit VirtualList(lv_obj_t* parent, int visibleCount = 12, int itemHeight = 32);
    ~VirtualList();

    // Non-copyable
    VirtualList(const VirtualList&) = delete;
    VirtualList& operator=(const VirtualList&) = delete;

    // ══════════════════════════════════════════════════════════════════
    // Configuration (called once at setup)
    // ══════════════════════════════════════════════════════════════════

    /**
     * @brief Set the callback to bind slots to logical indices
     * @param callback Function called when a slot needs to display an item
     */
    void setOnBindSlot(BindSlotCallback callback);

    /**
     * @brief Set optional callback for highlight-only updates
     * @param callback Function called when only selection state changes
     */
    void setOnUpdateHighlight(UpdateHighlightCallback callback);

    // ══════════════════════════════════════════════════════════════════
    // Data (called when data changes)
    // ══════════════════════════════════════════════════════════════════

    /**
     * @brief Set the total number of items in the list
     *
     * Triggers a full rebind if the visible window changes.
     */
    void setTotalCount(int count);
    int getTotalCount() const { return totalCount_; }

    // ══════════════════════════════════════════════════════════════════
    // Navigation (called on user input)
    // ══════════════════════════════════════════════════════════════════

    /**
     * @brief Set the selected index
     *
     * - If index is in visible window: updates highlight only
     * - If index moves out of window: rebinds slots + smooth scroll
     */
    void setSelectedIndex(int index);
    int getSelectedIndex() const { return selectedIndex_; }

    /**
     * @brief Force a rebind of all visible slots
     *
     * Useful when underlying data changes without changing totalCount
     * (e.g., a device's enabled state changes)
     */
    void invalidate();

    /**
     * @brief Invalidate a single slot by logical index
     *
     * If the index is currently visible, rebinds that slot.
     */
    void invalidateIndex(int logicalIndex);

    // ══════════════════════════════════════════════════════════════════
    // Slot access (for debug or advanced cases)
    // ══════════════════════════════════════════════════════════════════

    /**
     * @brief Get the slot bound to a logical index, or nullptr if not visible
     */
    VirtualSlot* getSlotForIndex(int logicalIndex);

    /**
     * @brief Get all slots (for iteration)
     */
    const std::vector<VirtualSlot>& getSlots() const { return slots_; }

    /**
     * @brief Get the first visible logical index
     */
    int getWindowStart() const { return windowStart_; }

    // ══════════════════════════════════════════════════════════════════
    // IComponent
    // ══════════════════════════════════════════════════════════════════

    void show() override;
    void hide() override;
    bool isVisible() const override { return visible_; }
    lv_obj_t* getElement() const override { return container_; }

private:
    // Container & slots creation
    void createContainer();
    void createSlots();

    // Core logic
    int calculateWindowStart() const;
    int logicalIndexToSlotIndex(int logicalIndex) const;
    void rebindAllSlots();
    void updateSelection(int oldIndex, int newIndex);
    void updateHighlightOnly(int oldIndex, int newIndex);
    void rebindSlot(VirtualSlot& slot, int newIndex);
    void updateSlotHighlight(VirtualSlot& slot, bool isSelected);

    lv_obj_t* parent_ = nullptr;
    lv_obj_t* container_ = nullptr;

    std::vector<VirtualSlot> slots_;
    int visibleCount_ = 12;
    int itemHeight_ = 32;

    int totalCount_ = 0;
    int selectedIndex_ = 0;
    int previousSelectedIndex_ = -1;
    int windowStart_ = 0;

    BindSlotCallback onBindSlot_;
    UpdateHighlightCallback onUpdateHighlight_;

    bool visible_ = false;
    bool initialized_ = false;
};

}  // namespace bitwig::ui
