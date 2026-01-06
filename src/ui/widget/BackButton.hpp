#pragma once

#include <lvgl.h>
#include <oc/ui/lvgl/IListItem.hpp>

namespace bitwig::ui {

/**
 * @brief Back navigation button for list selectors
 *
 * Displays a left arrow icon used in hierarchical navigation.
 * Implements IListItem for consistent handling with other list items.
 */
class BackButton : public oc::ui::lvgl::IListItem {
public:
    explicit BackButton(lv_obj_t* parent);
    ~BackButton() override;

    BackButton(const BackButton&) = delete;
    BackButton& operator=(const BackButton&) = delete;

    // IListItem
    void setHighlighted(bool highlighted) override;

    // IComponent
    void show() override;
    void hide() override;
    bool isVisible() const override;
    lv_obj_t* getElement() const override { return label_; }

private:
    lv_obj_t* label_ = nullptr;
    bool visible_ = false;
};

}  // namespace bitwig::ui
