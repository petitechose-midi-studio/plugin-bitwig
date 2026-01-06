#pragma once

#include "TrackTitleItem.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <lvgl.h>

#include <oc/ui/lvgl/IComponent.hpp>
#include <oc/ui/lvgl/widget/VirtualList.hpp>

#include "protocol/TrackType.hpp"
#include "ui/widget/BackButton.hpp"
#include "ui/widget/HintBar.hpp"

namespace bitwig::ui {

// Import VirtualList types from shared component
using oc::ui::lvgl::widget::VirtualList;
using oc::ui::lvgl::widget::VirtualSlot;
using oc::ui::lvgl::widget::ScrollMode;

struct TrackSelectorProps {
    std::vector<std::string> names;
    std::vector<bool> muteStates;
    std::vector<bool> soloStates;
    std::vector<TrackType> trackTypes;
    std::vector<uint32_t> trackColors;
    int selectedIndex = 0;
    bool visible = false;
};

/**
 * Track list selector with mute/solo indicators.
 * Uses VirtualList for O(1) rendering of large track lists.
 * Stateless - all data comes from props.
 */
class TrackSelector : public oc::ui::lvgl::IComponent {
public:
    explicit TrackSelector(lv_obj_t *parent);
    ~TrackSelector() override;

    TrackSelector(const TrackSelector &) = delete;
    TrackSelector &operator=(const TrackSelector &) = delete;

    void render(const TrackSelectorProps &props);

    // IComponent
    void show() override;
    void hide() override;
    bool isVisible() const override { return visible_; }
    lv_obj_t *getElement() const override { return overlay_; }
    lv_obj_t *getContainer() const { return container_; }

private:
    // Overlay structure
    void createOverlay();
    void createHeader();
    void createFooter();
    void renderFooter(const TrackSelectorProps &props);

    // VirtualList callbacks
    void bindSlot(VirtualSlot &slot, int index, bool isSelected);
    void updateSlotHighlight(VirtualSlot &slot, bool isSelected);
    void ensureSlotWidgets(int slotIndex);

    // Highlight
    void applyHighlightStyle(int slotIndex, bool isSelected);

    // Overlay
    lv_obj_t *parent_ = nullptr;
    lv_obj_t *overlay_ = nullptr;
    lv_obj_t *container_ = nullptr;

    // Header
    lv_obj_t *header_ = nullptr;
    lv_obj_t *header_label_ = nullptr;

    // Virtual list
    std::unique_ptr<VirtualList> list_;
    std::vector<std::unique_ptr<TrackTitleItem>> slotItems_;
    std::vector<std::unique_ptr<BackButton>> backButtons_;  // Back navigation buttons (one per slot)

    // Footer
    std::unique_ptr<HintBar> footer_;
    lv_obj_t *footer_mute_ = nullptr;
    lv_obj_t *footer_solo_ = nullptr;

    // State
    TrackSelectorProps currentProps_;
    bool visible_ = false;
};

}  // namespace bitwig::ui
