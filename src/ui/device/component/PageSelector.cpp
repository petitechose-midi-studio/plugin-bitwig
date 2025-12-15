#include "PageSelector.hpp"

namespace bitwig {

PageSelector::PageSelector(lv_obj_t *parent) : BaseSelector(parent) { setTitle("Select Page"); }

void PageSelector::render(const PageSelectorProps &props) {
    if (!props.visible) {
        hide();
        return;
    }

    if (!props.names) return;

    overlay().setItems(*props.names);
    overlay().setSelectedIndex(props.selectedIndex);

    if (!isVisible()) show();
}

}  // namespace bitwig
