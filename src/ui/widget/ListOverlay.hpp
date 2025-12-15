#pragma once

#include <string>
#include <vector>

#include <lvgl.h>

#include <oc/ui/lvgl/IComponent.hpp>
#include <oc/ui/lvgl/theme/BaseTheme.hpp>

namespace bitwig {

/**
 * @brief Pure UI widget for modal list overlay with selection
 *
 * Displays a centered modal overlay containing a scrollable list of items.
 * Supports visual selection highlighting via index.
 *
 * PURE UI - No logic, no callbacks, only setters/getters.
 *
 * Usage:
 *   ListOverlay overlay(parent);
 *   overlay.setTitle("Select Page");
 *   overlay.setItems({"Page 1", "Page 2", "Page 3"});
 *   overlay.setSelectedIndex(0);
 *   overlay.show();
 */
class ListOverlay : public oc::ui::lvgl::IComponent {
public:
    explicit ListOverlay(lv_obj_t* parent);
    ~ListOverlay();

    // Non-copyable, non-movable
    ListOverlay(const ListOverlay&) = delete;
    ListOverlay& operator=(const ListOverlay&) = delete;

    void setTitle(const std::string& title);
    void setItems(const std::vector<std::string>& items);
    void setSelectedIndex(int index);

    void show() override;
    void hide() override;
    bool isVisible() const override;

    int getSelectedIndex() const;
    int getItemCount() const;

    lv_obj_t* getButton(size_t index) const;
    void setItemFont(size_t index, const lv_font_t* font);

    lv_obj_t* getElement() const override { return overlay_; }
    lv_obj_t* getContainer() const { return container_; }

private:
    void createOverlay();
    void createTitleLabel();
    void createList();
    void populateList();

    void updateHighlight();
    void scrollToSelected(bool animate = true);

    void destroyList();
    void cleanup();

    lv_obj_t* parent_ = nullptr;
    lv_obj_t* overlay_ = nullptr;
    lv_obj_t* container_ = nullptr;
    lv_obj_t* title_label_ = nullptr;
    lv_obj_t* list_ = nullptr;

    std::vector<lv_obj_t*> buttons_;
    std::vector<std::string> items_;
    std::string title_;
    int selected_index_ = 0;
    bool visible_ = false;
    bool ui_created_ = false;
};

}  // namespace bitwig
