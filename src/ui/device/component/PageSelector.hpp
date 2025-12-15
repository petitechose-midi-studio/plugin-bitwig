#pragma once

#include <string>
#include <vector>

#include "ui/widget/BaseSelector.hpp"

namespace bitwig {

struct PageSelectorProps {
    const std::vector<std::string> *names = nullptr;
    int selectedIndex = 0;
    bool visible = false;
};

class PageSelector : public BaseSelector {
public:
    explicit PageSelector(lv_obj_t *parent);
    ~PageSelector() override = default;

    void render(const PageSelectorProps &props);

    int getPageCount() const { return getItemCount(); }
};

}  // namespace bitwig
