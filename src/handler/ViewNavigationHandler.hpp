#pragma once

#include "api/ControllerAPI.hpp"

namespace Bitwig {

class ViewManager;

/**
 * @brief Plugin-level input handler for view navigation (GLOBAL)
 *
 * Handles ONLY:
 * - View navigation menu (TOP_LEFT + NAV_ENCODER)
 *
 * All other bindings are handled in view-specific InputHandlers.
 */
class ViewNavigationHandler {
public:
    ViewNavigationHandler(ControllerAPI& api, ViewManager& viewManager);
    ~ViewNavigationHandler() = default;

private:
    void setupInputBindings();
    void showMenu();
    void hideMenu();
    void navigate(float delta);
    void activateView();

    ControllerAPI& api_;
    ViewManager& viewManager_;

    bool menuVisible_ = false;
    int selectedIndex_ = 0;
};

}  // namespace Bitwig
