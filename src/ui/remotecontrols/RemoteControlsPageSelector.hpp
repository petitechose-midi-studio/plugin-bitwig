#pragma once

#include <string>
#include <vector>

#include "ui/widget/BaseSelector.hpp"

namespace bitwig {

struct RemoteControlsPageSelectorProps {
    std::vector<std::string> names;
    int selectedIndex = 0;
    bool visible = false;
};

class RemoteControlsPageSelector : public BaseSelector {
public:
    explicit RemoteControlsPageSelector(lv_obj_t *parent);
    ~RemoteControlsPageSelector() override = default;

    void render(const RemoteControlsPageSelectorProps &props);

    int getPageCount() const { return getItemCount(); }
};

}  // namespace bitwig
