#include "PageSelector.hpp"

namespace Bitwig
{

    PageSelector::PageSelector(lv_obj_t *parent) : overlay_(parent)
    {
        overlay_.setTitle("Select Page");
    }

    void PageSelector::setPageNames(const std::vector<std::string> &pageNames)
    {
        page_names_ = pageNames;
        overlay_.setItems(page_names_);
    }

    void PageSelector::setCurrentPageIndex(int index)
    {
        current_page_index_ = index;

        overlay_.setSelectedIndex(index);
    }

    void PageSelector::setSelectedIndex(int index)
    {
        overlay_.setSelectedIndex(index);
    }

    void PageSelector::show()
    {
        overlay_.setSelectedIndex(current_page_index_);
        overlay_.show();
    }

    void PageSelector::hide()
    {
        overlay_.hide();
    }

    bool PageSelector::isVisible() const
    {
        return overlay_.isVisible();
    }

    int PageSelector::getSelectedIndex() const
    {
        return overlay_.getSelectedIndex();
    }

    int PageSelector::getPageCount() const
    {
        return overlay_.getItemCount();
    }

} // namespace Bitwig
