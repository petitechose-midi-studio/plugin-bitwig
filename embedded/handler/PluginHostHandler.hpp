#pragma once

#include "api/ControllerAPI.hpp"

namespace Bitwig
{

    class ViewManager;

    /**
     * @brief Plugin-level hardware input handler (GLOBAL)
     *
     * Handles ONLY:
     * - View navigation menu (TOP_LEFT + NAV_ENCODER)
     *
     * All other bindings are handled in view-specific InputHandlers.
     */
    class PluginHostHandler
    {
    public:
        PluginHostHandler(ControllerAPI &api, ViewManager &viewManager);
        ~PluginHostHandler() = default;

    private:
        void setupViewNavigation();
        void showMenu();
        void hideMenu();
        void navigate(float delta);
        void activateView();

        ControllerAPI &api_;
        ViewManager &viewManager_;

        bool menuVisible_ = false;
        int selectedIndex_ = 0;
    };

} // namespace Bitwig
