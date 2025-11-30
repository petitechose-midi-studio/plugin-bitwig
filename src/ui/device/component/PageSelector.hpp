#pragma once

#include "widget/BaseSelector.hpp"
#include <string>
#include <vector>

namespace Bitwig
{

struct PageSelectorProps
{
    const std::vector<std::string> *names = nullptr;
    int selectedIndex = 0;
    bool visible = false;
};

class PageSelector : public UI::BaseSelector
{
public:
    explicit PageSelector(lv_obj_t *parent);
    ~PageSelector() override = default;

    void render(const PageSelectorProps &props);

    int getPageCount() const { return getItemCount(); }
};

} // namespace Bitwig
