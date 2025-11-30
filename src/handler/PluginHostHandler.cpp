#include "PluginHostHandler.hpp"
#include "../ui/ViewManager.hpp"
#include "../ui/ViewRegistry.hpp"
#include "log/Macros.hpp"

namespace Bitwig
{

    PluginHostHandler::PluginHostHandler(ControllerAPI &api, ViewManager &viewManager)
        : api_(api), viewManager_(viewManager)
    {
        setupViewNavigation();
    }

    void PluginHostHandler::setupViewNavigation()
    {
        api_.onPressed(ButtonID::LEFT_TOP, [this]()
                       { showMenu(); });

        api_.onReleased(ButtonID::LEFT_TOP, [this]()
                        {
        if (menuVisible_) {
            activateView();
            hideMenu();
        } });

        api_.onTurnedWhilePressed(EncoderID::NAV, ButtonID::LEFT_TOP, [this](float delta)
                                  { navigate(delta); });
    }

    void PluginHostHandler::showMenu()
    {
        menuVisible_ = true;
        selectedIndex_ = 0;
        LOGLN("[PluginHost] Menu opened");
    }

    void PluginHostHandler::hideMenu()
    {
        menuVisible_ = false;
        LOGLN("[PluginHost] Menu closed");
    }

    void PluginHostHandler::navigate(float delta)
    {
        if (!menuVisible_)
            return;

        // Update selected index (wrap around between 0 and 1)
        selectedIndex_ += static_cast<int>(delta);
        if (selectedIndex_ < 0) selectedIndex_ = 1;
        if (selectedIndex_ > 1) selectedIndex_ = 0;
    }

    void PluginHostHandler::activateView()
    {
        LOGF("[PluginHost] Activate view index: %d\n", selectedIndex_);

        switch (selectedIndex_)
        {
        case 0:
            viewManager_.show(ViewID::DEVICE);
            break;
        case 1:
            viewManager_.show(ViewID::TRANSPORT);
            break;
        default:
            LOGLN("[PluginHost] Invalid view index");
            break;
    }
}

} // namespace Bitwig
