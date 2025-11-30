#include "ViewNavigationHandler.hpp"
#include "../ui/ViewManager.hpp"
#include "../ui/ViewRegistry.hpp"
#include "log/Macros.hpp"

namespace Bitwig {

ViewNavigationHandler::ViewNavigationHandler(ControllerAPI& api, ViewManager& viewManager)
    : api_(api), viewManager_(viewManager) {
    setupInputBindings();
}

void ViewNavigationHandler::setupInputBindings() {
    api_.onPressed(ButtonID::LEFT_TOP, [this]() { showMenu(); });

    api_.onReleased(ButtonID::LEFT_TOP, [this]() {
        if (menuVisible_) {
            activateView();
            hideMenu();
        }
    });

    api_.onTurnedWhilePressed(EncoderID::NAV, ButtonID::LEFT_TOP,
                              [this](float delta) { navigate(delta); });
}

void ViewNavigationHandler::showMenu() {
    menuVisible_ = true;
    selectedIndex_ = 0;
    LOGLN("[ViewNav] Menu opened");
}

void ViewNavigationHandler::hideMenu() {
    menuVisible_ = false;
    LOGLN("[ViewNav] Menu closed");
}

void ViewNavigationHandler::navigate(float delta) {
    if (!menuVisible_) return;

    selectedIndex_ += static_cast<int>(delta);
    if (selectedIndex_ < 0) selectedIndex_ = 1;
    if (selectedIndex_ > 1) selectedIndex_ = 0;
}

void ViewNavigationHandler::activateView() {
    LOGF("[ViewNav] Activate view index: %d\n", selectedIndex_);

    switch (selectedIndex_) {
    case 0:
        viewManager_.show(ViewID::DEVICE);
        break;
    case 1:
        viewManager_.show(ViewID::TRANSPORT);
        break;
    default:
        LOGLN("[ViewNav] Invalid view index");
        break;
    }
}

}  // namespace Bitwig
