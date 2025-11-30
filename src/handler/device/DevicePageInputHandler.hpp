#pragma once

#include "api/ControllerAPI.hpp"
#include "../../protocol/Protocol.hpp"

namespace Bitwig {

class DeviceView;
class DeviceController;

/**
 * @brief Hardware input handler for PageSelector overlay
 *
 * Handles page navigation when PageSelector overlay is visible:
 * - Open/close with LEFT_BOTTOM (latch behavior)
 * - Navigate with encoder
 * - Confirm selection with NAV button
 */
class DevicePageInputHandler {
public:
    DevicePageInputHandler(ControllerAPI& api, DeviceView& view, DeviceController& controller,
                           Protocol::Protocol& protocol, lv_obj_t* scope);
    ~DevicePageInputHandler();

    void setPageSelectionState(uint8_t pageCount, uint8_t currentIndex);

private:
    void setupBindings();

    void requestPageList();
    void navigate(float delta);
    void confirmSelection();
    void closeSelector();

    ControllerAPI& api_;
    DeviceView& view_;
    lv_obj_t* scope_;
    DeviceController& controller_;
    Protocol::Protocol& protocol_;

    struct PageState {
        uint8_t count = 0;
        uint8_t cursor = 0;
        bool requested = false;
        // currentSelectorIndex removed - use view_.state().pageSelector.selectedIndex
    };
    PageState state_;
};

} // namespace Bitwig
