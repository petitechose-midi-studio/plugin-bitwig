#include "DeviceView.hpp"

#include "font/FontLoader.hpp"
#include "widget/ParameterButtonWidget.hpp"
#include "widget/ParameterKnobWidget.hpp"
#include "widget/ParameterListWidget.hpp"
#include "log/Macros.hpp"

namespace Bitwig
{

    DeviceView::DeviceView(lv_obj_t *zone, const Config &viewConfig)
        : viewConfig_(viewConfig),
          initialized_(false),
          active_(false),
          zone_(zone),
          top_bar_container_(nullptr),
          body_container_(nullptr),
          is_active_(false)
    {

        for (auto &widget : widgets_)
        {
            widget = nullptr;
        }

        createUI();
        initialized_ = true;

        // Hidden by default (follows IView pattern - shown via onActivate())
        // lv_obj_add_flag(zone_, LV_OBJ_FLAG_HIDDEN);
    }

    DeviceView::~DeviceView()
    {
        setActive(false);

        for (auto &widget : widgets_)
        {
            widget.reset();
        }

        if (top_bar_container_)
        {
            lv_obj_delete(top_bar_container_);
            top_bar_container_ = nullptr;
        }
        if (body_container_)
        {
            lv_obj_delete(body_container_);
            body_container_ = nullptr;
        }
    }

    void DeviceView::createUI()
    {
        // Fonts are loaded once at startup, not per view
        // load_fonts();

        top_bar_container_ = lv_obj_create(zone_);
        lv_obj_set_size(top_bar_container_, LV_PCT(100), 20);
        lv_obj_set_style_bg_opa(top_bar_container_, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_width(top_bar_container_, 0, 0);
        lv_obj_set_style_radius(top_bar_container_, 0, 0);
        lv_obj_set_style_pad_all(top_bar_container_, 0, 0);

        body_container_ = lv_obj_create(zone_);
        lv_obj_set_size(body_container_, LV_PCT(100), LV_SIZE_CONTENT);
        lv_obj_set_flex_grow(body_container_, 1);
        lv_obj_set_style_bg_opa(body_container_, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_width(body_container_, 0, 0);
        lv_obj_set_style_radius(body_container_, 0, 0);
        lv_obj_set_style_pad_all(body_container_, 0, 0);

        createDeviceStateBar();

        // Widgets are created dynamically in setParameterTypeAndMetadata()
        // based on parameter type from Bitwig

        setupLayout();
        initializeMappingsFromConfig();
    }

    void DeviceView::render() {}

    void DeviceView::update()
    {
        if (!active_ || !initialized_)
        {
            return;
        }

        for (const auto &widget : widgets_)
        {
            if (widget)
            {
            }
        }
    }

    void DeviceView::setActive(bool active)
    {
        if (active_ == active)
        {
            return;
        }

        active_ = active;

        if (top_bar_container_)
        {
            if (active)
            {
                lv_obj_clear_flag(top_bar_container_, LV_OBJ_FLAG_HIDDEN);
            }
            else
            {
                lv_obj_add_flag(top_bar_container_, LV_OBJ_FLAG_HIDDEN);
            }
        }
        if (body_container_)
        {
            if (active)
            {
                lv_obj_clear_flag(body_container_, LV_OBJ_FLAG_HIDDEN);
            }
            else
            {
                lv_obj_add_flag(body_container_, LV_OBJ_FLAG_HIDDEN);
            }
        }

        is_active_ = active;
    }

    void DeviceView::onActivate()
    {
    }

    void DeviceView::onDeactivate()
    {
    }

    void DeviceView::setParameterTypeAndMetadata(uint8_t paramIndex,
                                                 uint8_t parameterType,
                                                 int16_t discreteCount,
                                                 const etl::vector<etl::string<16>, 32> &discreteValueNames,
                                                 uint8_t currentValueIndex,
                                                 float origin,
                                                 const char *displayValue)
    {
        if (!initialized_ || paramIndex >= 8 || !body_container_)
            return;

        widgets_[paramIndex].reset();

        switch (parameterType)
        {
        case 1:
            widgets_[paramIndex] =
                std::make_unique<ParameterButtonWidget>(body_container_, 80, 100, paramIndex);
            break;

        case 2:
            widgets_[paramIndex] = std::make_unique<ParameterListWidget>(body_container_,
                                                                         80,
                                                                         100,
                                                                         paramIndex,
                                                                         discreteCount);
            break;

        case 0:
        default:
        {
            bool isCentered = (origin == 0.5f);
            auto knob = std::make_unique<ParameterKnobWidget>(body_container_, 80, 100, paramIndex, isCentered);
            knob->setOrigin(origin);
            widgets_[paramIndex] = std::move(knob);
            break;
        }
        }

        if (widgets_[paramIndex])
        {
            int col = paramIndex % 4;
            int row = paramIndex / 4;

            lv_obj_set_grid_cell(widgets_[paramIndex]->getContainer(),
                                 LV_GRID_ALIGN_CENTER,
                                 col,
                                 1,
                                 LV_GRID_ALIGN_CENTER,
                                 row,
                                 1);

            if (parameterType == 1 || parameterType == 2)
            {
                widgets_[paramIndex]->setDiscreteMetadata(discreteCount, discreteValueNames, currentValueIndex);
            }
        }
    }

    void DeviceView::setParameterValue(uint8_t paramIndex, float normalizedValue)
    {
        if (!initialized_ || paramIndex >= 8)
            return;

        if (widgets_[paramIndex])
        {
            widgets_[paramIndex]->setValue(normalizedValue);
        }
    }

    void DeviceView::setParameterValueWithDisplay(uint8_t paramIndex, float normalizedValue,
                                                  const char *displayValue)
    {
        if (!initialized_ || paramIndex >= 8)
            return;

        if (widgets_[paramIndex])
        {
            widgets_[paramIndex]->setValueWithDisplay(normalizedValue, displayValue);
        }
    }

    void DeviceView::setParameterName(uint8_t paramIndex, const char *name)
    {
        if (!initialized_ || paramIndex >= 8 || !name)
            return;

        if (widgets_[paramIndex])
        {
            widgets_[paramIndex]->setName(name);
        }
    }

    void DeviceView::setParameterVisible(uint8_t paramIndex, bool visible)
    {
        if (!initialized_ || paramIndex >= 8)
            return;

        if (widgets_[paramIndex])
        {
            lv_obj_t *container = widgets_[paramIndex]->getContainer();
            if (container)
            {
                if (visible)
                {
                    lv_obj_clear_flag(container, LV_OBJ_FLAG_HIDDEN);
                }
                else
                {
                    lv_obj_add_flag(container, LV_OBJ_FLAG_HIDDEN);
                }
            }
        }
    }

    void DeviceView::setParameterLoading(uint8_t paramIndex, bool loading)
    {
        if (!initialized_ || paramIndex >= 8)
            return;

        if (widgets_[paramIndex])
        {
            // TODO: Add visual loading indicator (grayed out, spinner, etc.)
            // For now, just note the state internally
        }
    }

    void DeviceView::setAllWidgetsLoading(bool loading)
    {
        if (!initialized_)
            return;

        for (uint8_t i = 0; i < 8; i++)
        {
            setParameterLoading(i, loading);
        }
    }

    void DeviceView::setParameterDiscreteValues(uint8_t paramIndex,
                                                const etl::vector<etl::string<16>, 32> &discreteValueNames,
                                                uint8_t currentValueIndex)
    {
        if (!initialized_ || paramIndex >= 8)
            return;

        if (widgets_[paramIndex])
        {
            widgets_[paramIndex]->setDiscreteMetadata(discreteValueNames.size(), discreteValueNames, currentValueIndex);
        }
    }

    void DeviceView::setDeviceName(const char *name)
    {
        if (!initialized_ || !name)
            return;

        if (top_bar_component_)
        {
            top_bar_component_->setDeviceName(name);
        }
    }

    void DeviceView::setDeviceEnabled(bool enabled)
    {
        if (!initialized_)
            return;

        if (top_bar_component_)
        {
            top_bar_component_->setDeviceState(enabled);
        }
    }

    void DeviceView::setDeviceHasChildren(bool hasChildren)
    {
        if (!initialized_)
            return;

        if (top_bar_component_)
        {
            top_bar_component_->setDeviceHasChildren(hasChildren);
        }
    }

    void DeviceView::setDevicePageInfo(uint8_t currentPage, uint8_t totalPages)
    {
        if (!initialized_)
            return;
    }

    void DeviceView::setDevicePageName(const char *name)
    {
        if (!initialized_ || !name)
            return;

        if (top_bar_component_)
        {
            top_bar_component_->setPageName(name);
        }
    }

    void DeviceView::setupLayout()
    {
        if (!body_container_)
        {
            return;
        }

        static const lv_coord_t col_dsc[] = {LV_GRID_FR(1),
                                             LV_GRID_FR(1),
                                             LV_GRID_FR(1),
                                             LV_GRID_FR(1),
                                             LV_GRID_TEMPLATE_LAST};
        static const lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

        lv_obj_set_layout(body_container_, LV_LAYOUT_GRID);
        lv_obj_set_grid_dsc_array(body_container_, col_dsc, row_dsc);

        for (int i = 0; i < 8; i++)
        {
            if (widgets_[i])
            {
                int col = i % 4;
                int row = i / 4;

                lv_obj_set_grid_cell(widgets_[i]->getContainer(),
                                     LV_GRID_ALIGN_CENTER,
                                     col,
                                     1,
                                     LV_GRID_ALIGN_CENTER,
                                     row,
                                     1);
            }
        }
    }

    void DeviceView::initializeMappingsFromConfig()
    {
        if (!viewConfig_.isValid())
        {
            return;
        }
    }

    int8_t DeviceView::getWidgetIndexForButton(uint16_t button_id) const
    {
        return -1;
    }

    IParameterWidget *DeviceView::getWidgetForButton(uint16_t button_id)
    {
        int8_t index = getWidgetIndexForButton(button_id);
        if (index >= 0 && index < 8)
        {
            return widgets_[index].get();
        }
        return nullptr;
    }

    void DeviceView::createDeviceStateBar()
    {
        if (!top_bar_container_)
        {
            return;
        }

        top_bar_component_ = std::make_unique<DeviceStateBar>(top_bar_container_);

        top_bar_component_->setDeviceName("Device 1");
        top_bar_component_->setDeviceState(true);
        top_bar_component_->setPageName("Page");

        page_selector_ = std::make_unique<PageSelector>(zone_);

        device_selector_ = std::make_unique<DeviceSelector>(zone_);

        track_list_selector_ = std::make_unique<TrackListSelector>(zone_);
    }

    void DeviceView::showPageSelector(const std::vector<std::string> &pageNames, int currentIndex)
    {
        if (!page_selector_)
            return;
        page_selector_->setPageNames(pageNames);
        page_selector_->setCurrentPageIndex(currentIndex);
        page_selector_->show();
    }

    void DeviceView::setPageSelectorIndex(int index)
    {
        if (!page_selector_)
            return;
        page_selector_->setSelectedIndex(index);
    }

    void DeviceView::hidePageSelector()
    {
        if (!page_selector_)
            return;
        page_selector_->hide();
    }

    int DeviceView::getPageSelectorIndex() const
    {
        if (!page_selector_)
            return -1;
        return page_selector_->getSelectedIndex();
    }

    lv_obj_t *DeviceView::getPageSelectorElement() const
    {
        if (!page_selector_)
            return nullptr;
        return page_selector_->getElement();
    }

    void DeviceView::showDeviceList(const std::vector<std::string> &names,
                                    int currentIndex,
                                    const std::vector<bool> &deviceStates,
                                    const std::vector<bool> &hasSlots,
                                    const std::vector<bool> &hasLayers,
                                    const std::vector<bool> &hasDrums)
    {
        if (!device_selector_)
            return;
        device_selector_->setDeviceItems(names, currentIndex, deviceStates, hasSlots, hasLayers, hasDrums);
        device_selector_->showWithFooter();
    }

    void DeviceView::showDeviceChildren(const std::vector<std::string> &items, const std::vector<uint8_t> &itemTypes)
    {
        if (!device_selector_)
            return;
        device_selector_->setChildrenItems(items, itemTypes);
        device_selector_->setCurrentItemIndex(1);
        device_selector_->showWithoutFooter();
    }

    void DeviceView::setDeviceSelectorIndex(int index)
    {
        if (!device_selector_)
            return;
        device_selector_->setSelectedIndex(index);
    }

    void DeviceView::hideDeviceSelector()
    {
        if (!device_selector_)
            return;
        device_selector_->hide();
    }

    void DeviceView::showDeviceSelector()
    {
        if (!device_selector_)
            return;
        device_selector_->show();
    }

    int DeviceView::getDeviceSelectorIndex() const
    {
        if (!device_selector_)
            return -1;
        return device_selector_->getSelectedIndex();
    }

    int DeviceView::getDeviceSelectorItemCount() const
    {
        if (!device_selector_)
            return 0;
        return device_selector_->getItemCount();
    }

    lv_obj_t *DeviceView::getDeviceSelectorElement() const
    {
        if (!device_selector_)
            return nullptr;
        return device_selector_->getElement();
    }

    bool DeviceView::isDeviceSelectorVisible() const
    {
        if (!device_selector_)
            return false;
        return device_selector_->isVisible();
    }

    void DeviceView::updateDeviceState(int displayIndex, bool enabled)
    {
        if (!device_selector_ || !device_selector_->isVisible())
            return;
        device_selector_->setDeviceStateAtIndex(displayIndex, enabled);
    }

    void DeviceView::showTrackList(const std::vector<std::string> &names,
                                   int currentIndex,
                                   const std::vector<bool> &muteStates,
                                   const std::vector<bool> &soloStates,
                                   const std::vector<uint8_t> &trackTypes,
                                   const std::vector<uint32_t> &trackColors)
    {
        if (!track_list_selector_)
            return;
        track_list_selector_->setTrackItems(names, currentIndex, muteStates, soloStates, trackTypes, trackColors);
        track_list_selector_->show();
    }

    void DeviceView::setTrackListSelectorIndex(int index)
    {
        if (!track_list_selector_)
            return;
        track_list_selector_->setSelectedIndex(index);
    }

    void DeviceView::hideTrackSelector()
    {
        if (!track_list_selector_)
            return;
        track_list_selector_->hide();
    }

    void DeviceView::showTrackSelector()
    {
        if (!track_list_selector_)
            return;
        track_list_selector_->show();
    }

    int DeviceView::getTrackListSelectorIndex() const
    {
        if (!track_list_selector_)
            return -1;
        return track_list_selector_->getSelectedIndex();
    }

    int DeviceView::getTrackListSelectorItemCount() const
    {
        if (!track_list_selector_)
            return 0;
        return track_list_selector_->getItemCount();
    }

    lv_obj_t *DeviceView::getTrackListSelectorElement() const
    {
        if (!track_list_selector_)
            return nullptr;
        return track_list_selector_->getElement();
    }

    bool DeviceView::isTrackSelectorVisible() const
    {
        if (!track_list_selector_)
            return false;
        return track_list_selector_->isVisible();
    }

    void DeviceView::updateTrackMuteState(int displayIndex, bool isMuted)
    {
        if (!track_list_selector_)
            return;
        track_list_selector_->setTrackMuteStateAtIndex(displayIndex, isMuted);
    }

    void DeviceView::updateTrackSoloState(int displayIndex, bool isSoloed)
    {
        if (!track_list_selector_)
            return;
        track_list_selector_->setTrackSoloStateAtIndex(displayIndex, isSoloed);
    }

    bool DeviceView::getTrackMuteState(int displayIndex) const
    {
        if (!track_list_selector_)
            return false;
        return track_list_selector_->getTrackMuteStateAtIndex(displayIndex);
    }

    bool DeviceView::getTrackSoloState(int displayIndex) const
    {
        if (!track_list_selector_)
            return false;
        return track_list_selector_->getTrackSoloStateAtIndex(displayIndex);
    }

} // namespace Bitwig
