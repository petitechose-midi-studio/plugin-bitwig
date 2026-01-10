#pragma once

/**
 * @file ViewSelector.hpp
 * @brief Main view switcher overlay
 *
 * Modal selector for switching between top-level views:
 * - Remote Controls (device parameters)
 * - Mix (volume, pan, sends) - placeholder
 * - Clip (clip launcher) - placeholder
 *
 * Uses BaseSelector/ListOverlay for simple string list display.
 * Controlled via ViewSwitcherInputHandler.
 *
 * @see ViewSwitcherInputHandler for input bindings
 * @see BitwigContext for view management
 */

#include <string>
#include <vector>

#include "ui/widget/BaseSelector.hpp"

namespace bitwig::ui {

struct ViewSelectorProps {
    std::vector<std::string> names;
    int selectedIndex = 0;
    bool visible = false;
};

class ViewSelector : public BaseSelector {
public:
    explicit ViewSelector(lv_obj_t* parent);
    ~ViewSelector() override = default;

    void render(const ViewSelectorProps& props);

    int getViewCount() const { return getItemCount(); }
};

}  // namespace bitwig::ui
