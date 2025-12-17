#include "ViewSelector.hpp"

#include <oc/log/Log.hpp>

namespace bitwig {

ViewSelector::ViewSelector(lv_obj_t* parent) : BaseSelector(parent) {
    setTitle("Select View");
}

void ViewSelector::render(const ViewSelectorProps& props) {
    OC_LOG_DEBUG("[ViewSelector] render() visible={} names.size()={} selectedIdx={}",
                 props.visible, props.names.size(), props.selectedIndex);

    if (!props.visible) {
        hide();
        return;
    }

    if (!isVisible()) show();

    if (props.names.empty()) {
        OC_LOG_DEBUG("[ViewSelector] names empty");
        return;
    }

    OC_LOG_DEBUG("[ViewSelector] setting {} items", props.names.size());
    overlay().setItems(props.names);
    overlay().setSelectedIndex(props.selectedIndex);
}

}  // namespace bitwig
