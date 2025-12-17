#pragma once

#include <string>
#include <vector>

#include "ui/widget/BaseSelector.hpp"

namespace bitwig {

struct ViewSelectorProps {
    std::vector<std::string> names;
    int selectedIndex = 0;
    bool visible = false;
};

class ViewSelector : public BaseSelector {
public:
    explicit ViewSelector(lv_obj_t* parent);
    ~ViewSelector() override = default;

    void render(const ViewSelectorProps& props);

    int getViewCount() const { return getItemCount(); }
};

}  // namespace bitwig
