#pragma once

/**
 * @file HintBar.hpp
 * @brief 3-cell layout container for footer hints and shortcuts
 *
 * Positions hint elements (icons, labels) in a horizontal bar:
 * - Cell 0: Left-aligned element
 * - Cell 1: Center-aligned element
 * - Cell 2: Right-aligned element
 *
 * Used in selector footers to show available actions (mute/solo, enable/disable).
 * Pure layout - does not create content, only positions provided elements.
 *
 * @see DeviceSelector for footer usage with device actions
 * @see TrackSelector for footer usage with mute/solo hints
 */

#include <lvgl.h>

#include <oc/ui/lvgl/IComponent.hpp>

namespace bitwig::ui {

enum class HintBarPosition : uint8_t { LEFT, BOTTOM, RIGHT };

/**
 * @brief Container with 3 cells for positioning hint elements
 *
 * Pure layout component - receives LVGL elements and positions them.
 * Does not create or style content, only handles positioning.
 */
class HintBar : public oc::ui::lvgl::IComponent {
public:
    HintBar(lv_obj_t* parent, HintBarPosition position);
    ~HintBar() override;

    void setCell(int index, lv_obj_t* element);
    static constexpr int getCellCount() { return 3; }
    void setSize(lv_coord_t size);

    // IComponent interface
    void show() override;
    void hide() override;
    bool isVisible() const override;
    lv_obj_t* getElement() const override { return container_; }

private:
    static constexpr lv_coord_t DEFAULT_SIZE = 20;

    void applyGridLayout();

    HintBarPosition position_;
    lv_obj_t* parent_ = nullptr;
    lv_obj_t* container_ = nullptr;
    lv_coord_t size_ = DEFAULT_SIZE;
};

}  // namespace bitwig::ui
