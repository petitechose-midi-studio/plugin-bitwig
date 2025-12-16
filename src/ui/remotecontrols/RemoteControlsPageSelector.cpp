#include "RemoteControlsPageSelector.hpp"

#include <oc/log/Log.hpp>

namespace bitwig {

RemoteControlsPageSelector::RemoteControlsPageSelector(lv_obj_t *parent)
    : BaseSelector(parent) {
    setTitle("Select Page");
}

void RemoteControlsPageSelector::render(const RemoteControlsPageSelectorProps &props) {
    OC_LOG_DEBUG("[RemoteControlsPageSelector] render() visible={} names.size()={} selectedIdx={}",
                 props.visible, props.names.size(), props.selectedIndex);

    if (!props.visible) {
        hide();
        return;
    }

    // Show overlay even if names is empty (will update when data arrives)
    if (!isVisible()) show();

    if (props.names.empty()) {
        OC_LOG_DEBUG("[RemoteControlsPageSelector] names empty, waiting for data...");
        return;
    }

    OC_LOG_DEBUG("[RemoteControlsPageSelector] setting {} items", props.names.size());
    overlay().setItems(props.names);
    overlay().setSelectedIndex(props.selectedIndex);
}

}  // namespace bitwig
