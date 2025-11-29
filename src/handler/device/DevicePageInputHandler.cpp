#include "DevicePageInputHandler.hpp"
#include "InputUtils.hpp"
#include "../../ui/device/DeviceView.hpp"
#include "../../ui/device/DeviceController.hpp"
#include "../../protocol/struct/RequestDevicePageNamesMessage.hpp"
#include "../../protocol/struct/DevicePageSelectByIndexMessage.hpp"

namespace Bitwig {

// =============================================================================
// Construction / Destruction
// =============================================================================

DevicePageInputHandler::DevicePageInputHandler(ControllerAPI& api, DeviceView& view,
                                               DeviceController& controller, Protocol::Protocol& protocol,
                                               lv_obj_t* scope)
    : api_(api), view_(view), scope_(scope), controller_(controller), protocol_(protocol)
{
    setupBindings();
}

DevicePageInputHandler::~DevicePageInputHandler() = default;

// =============================================================================
// Public API
// =============================================================================

void DevicePageInputHandler::setPageSelectionState(uint8_t pageCount, uint8_t currentIndex) {
    state_.count = pageCount;
    state_.cursor = currentIndex;
    state_.currentSelectorIndex = currentIndex;

    api_.setEncoderMode(EncoderID::NAV, Hardware::EncoderMode::Relative);
    controller_.handlePageSelectorSetIndex(currentIndex);
}

// =============================================================================
// Bindings Setup
// =============================================================================

void DevicePageInputHandler::setupBindings() {
    // Open/close page selector (latch behavior)
    api_.onPressed(ButtonID::LEFT_BOTTOM, [this]() { requestPageList(); }, scope_, true);
    api_.onReleased(ButtonID::LEFT_BOTTOM, [this]() { closeSelector(); }, scope_);

    // Navigate pages
    api_.onTurnedWhilePressed(EncoderID::NAV, ButtonID::LEFT_BOTTOM,
        [this](float delta) { navigate(delta); }, scope_);

    // Confirm selection (scoped to overlay)
    api_.onPressed(ButtonID::NAV, [this]() { confirmSelection(); }, view_.getPageSelectorElement());
}

// =============================================================================
// Handlers
// =============================================================================

void DevicePageInputHandler::requestPageList() {
    if (!state_.requested) {
        protocol_.send(Protocol::RequestDevicePageNamesMessage{});
        state_.requested = true;
    }
}

void DevicePageInputHandler::navigate(float delta) {
    if (state_.count == 0) return;

    state_.currentSelectorIndex += static_cast<int>(delta);
    state_.currentSelectorIndex = InputUtils::wrapIndex(state_.currentSelectorIndex, state_.count);
    controller_.handlePageSelectorSetIndex(state_.currentSelectorIndex);
}

void DevicePageInputHandler::confirmSelection() {
    int index = controller_.getPageSelectorSelectedIndex();
    if (index >= 0) {
        protocol_.send(Protocol::DevicePageSelectByIndexMessage{static_cast<uint8_t>(index)});
    }
}

void DevicePageInputHandler::closeSelector() {
    int index = controller_.getPageSelectorSelectedIndex();
    controller_.handlePageSelectorConfirm();

    if (index >= 0) {
        protocol_.send(Protocol::DevicePageSelectByIndexMessage{static_cast<uint8_t>(index)});
    }

    state_.requested = false;
    api_.setLatch(ButtonID::LEFT_BOTTOM, false);
}

} // namespace Bitwig
