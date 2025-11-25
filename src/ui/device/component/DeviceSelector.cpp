#include "DeviceSelector.hpp"

namespace Bitwig
{

    DeviceSelector::DeviceSelector(lv_obj_t *parent)
        : parent_(parent), overlay_(parent)
    {
        overlay_.setTitle("Select Device");
    }

    DeviceSelector::~DeviceSelector()
    {
        destroyFooter();
    }

    void DeviceSelector::setTitle(const std::string &title)
    {
        overlay_.setTitle(title);
    }

    void DeviceSelector::setItems(const std::vector<std::string> &items)
    {
        items_ = items;
        overlay_.setItems(items_);
    }

    void DeviceSelector::setDeviceItems(const std::vector<std::string> &names,
                                        int currentIndex,
                                        const std::vector<bool> &deviceStates,
                                        const std::vector<bool> &hasSlots,
                                        const std::vector<bool> &hasLayers,
                                        const std::vector<bool> &hasDrums)
    {
        items_ = names;
        current_item_index_ = currentIndex;
        overlay_.setDeviceItems(names, currentIndex, deviceStates, hasSlots, hasLayers, hasDrums);
    }

    void DeviceSelector::setChildrenItems(const std::vector<std::string> &items, const std::vector<uint8_t> &itemTypes)
    {
        items_ = items;
        overlay_.setChildrenItems(items, itemTypes);
    }

    void DeviceSelector::setCurrentItemIndex(int index)
    {
        current_item_index_ = index;

        overlay_.setSelectedIndex(index);
    }

    void DeviceSelector::setSelectedIndex(int index)
    {
        overlay_.setSelectedIndex(index);
    }

    void DeviceSelector::show()
    {
        overlay_.setSelectedIndex(current_item_index_);
        overlay_.show();
    }

    void DeviceSelector::showWithFooter()
    {
        overlay_.setSelectedIndex(current_item_index_);
        overlay_.show();

        if (!footer_)
        {
            createFooter();
        }
        footer_->show();
    }

    void DeviceSelector::showWithoutFooter()
    {
        overlay_.setSelectedIndex(current_item_index_);
        overlay_.show();

        if (footer_)
        {
            footer_->hide();
        }
    }

    void DeviceSelector::hide()
    {
        overlay_.hide();

        if (footer_)
        {
            footer_->hide();
        }
    }

    bool DeviceSelector::isVisible() const
    {
        return overlay_.isVisible();
    }

    int DeviceSelector::getSelectedIndex() const
    {
        return overlay_.getSelectedIndex();
    }

    int DeviceSelector::getItemCount() const
    {
        return overlay_.getItemCount();
    }

    void DeviceSelector::setDeviceStateAtIndex(int displayIndex, bool enabled)
    {
        overlay_.setDeviceStateAtIndex(displayIndex, enabled);
    }

    void DeviceSelector::createFooter()
    {
        if (!parent_)
            return;

        footer_ = std::make_unique<ButtonHintBar>(parent_);
        footer_->setLeftLabel("Track");
        footer_->setCenterLabel("State");
    }

    void DeviceSelector::destroyFooter()
    {
        footer_.reset();
    }

} // namespace Bitwig
