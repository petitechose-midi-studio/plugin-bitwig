#pragma once

#include <memory>
#include <string>
#include <vector>

#include <lvgl.h>

#include <oc/ui/lvgl/IComponent.hpp>
#include <oc/ui/lvgl/widget/Label.hpp>
#include <oc/ui/lvgl/widget/VirtualList.hpp>

namespace bitwig {

// Import VirtualList types from shared component
using oc::ui::lvgl::widget::VirtualList;
using oc::ui::lvgl::widget::VirtualSlot;
using oc::ui::lvgl::widget::ScrollMode;

struct RemoteControlsPageSelectorProps {
    std::vector<std::string> names;
    int selectedIndex = 0;
    int totalCount = 0;  // Total pages (for windowed loading)
    bool visible = false;
};

/**
 * @brief Widgets inside a single VirtualList slot (reusable pool)
 */
struct PageSlotWidgets {
    lv_obj_t* label = nullptr;
    lv_obj_t* indexLabel = nullptr;
    bool created = false;
};

/**
 * Page selector using VirtualList for O(1) rendering.
 * Stateless - all data comes from props.
 */
class RemoteControlsPageSelector : public oc::ui::lvgl::IComponent {
public:
    explicit RemoteControlsPageSelector(lv_obj_t* parent);
    ~RemoteControlsPageSelector() override;

    RemoteControlsPageSelector(const RemoteControlsPageSelector&) = delete;
    RemoteControlsPageSelector& operator=(const RemoteControlsPageSelector&) = delete;

    void render(const RemoteControlsPageSelectorProps& props);

    int getPageCount() const { return currentProps_.totalCount; }

    // IComponent
    void show() override;
    void hide() override;
    bool isVisible() const override { return visible_; }
    lv_obj_t* getElement() const override { return overlay_; }
    lv_obj_t* getContainer() const { return container_; }

private:
    // Overlay structure
    void createOverlay();
    void createHeader();

    // VirtualList callbacks
    void bindSlot(VirtualSlot& slot, int index, bool isSelected);
    void updateSlotHighlight(VirtualSlot& slot, bool isSelected);
    void ensureSlotWidgets(VirtualSlot& slot, int slotIndex);

    // Highlight
    void applyHighlightStyle(PageSlotWidgets& widgets, bool isSelected);

    // Overlay
    lv_obj_t* parent_ = nullptr;
    lv_obj_t* overlay_ = nullptr;
    lv_obj_t* container_ = nullptr;

    // Header
    lv_obj_t* header_ = nullptr;
    lv_obj_t* header_label_ = nullptr;

    // Virtual list
    std::unique_ptr<VirtualList> list_;
    std::vector<PageSlotWidgets> slotWidgets_;

    // State
    RemoteControlsPageSelectorProps currentProps_;
    bool visible_ = false;
};

}  // namespace bitwig
