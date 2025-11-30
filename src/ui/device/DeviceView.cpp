#include "DeviceView.hpp"

#include "handler/device/DeviceConstants.hpp"
#include "font/FontLoader.hpp"
#include "widget/ParameterButtonWidget.hpp"
#include "widget/ParameterKnobWidget.hpp"
#include "widget/ParameterListWidget.hpp"
#include "ui/theme/BitwigTheme.hpp"
#include "ui/theme/StyleHelpers.hpp"
#include "log/Macros.hpp"
#include "util/TextUtils.hpp"

using namespace Theme;

namespace Bitwig {

    DeviceView::DeviceView(lv_obj_t *zone, const Config &viewConfig)
        : viewConfig_(viewConfig),
          initialized_(false),
          zone_(zone),
          top_bar_container_(nullptr),
          body_container_(nullptr)
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
        top_bar_container_ = lv_obj_create(zone_);
        lv_obj_set_size(top_bar_container_, LV_PCT(100), Layout::TOP_BAR_HEIGHT);
        Style::applyTransparentContainer(top_bar_container_);
        lv_obj_set_style_radius(top_bar_container_, 0, 0);

        body_container_ = lv_obj_create(zone_);
        lv_obj_set_size(body_container_, LV_PCT(100), LV_SIZE_CONTENT);
        lv_obj_set_flex_grow(body_container_, 1);
        Style::applyTransparentContainer(body_container_);
        lv_obj_set_style_radius(body_container_, 0, 0);

        createDeviceStateBar();

        // Widgets are created dynamically in setParameterTypeAndMetadata()
        // based on parameter type from Bitwig

        setupLayout();
    }

    void DeviceView::sync()
    {
        if (!initialized_ || !state_.dirty.any())
            return;

        if (state_.dirty.device)
            syncDeviceInfo();

        for (uint8_t i = 0; i < 8; i++)
        {
            if (state_.dirty.parameters[i])
                syncParameter(i);
        }

        if (state_.dirty.pageSelector)
            syncPageSelector();

        if (state_.dirty.deviceSelector)
            syncDeviceSelector();

        if (state_.dirty.trackSelector)
            syncTrackSelector();

        state_.dirty.clear();
    }

    void DeviceView::syncDeviceInfo()
    {
        if (!top_bar_component_)
            return;

        top_bar_component_->render({
            .deviceName = state_.device.name.c_str(),
            .deviceType = state_.device.deviceType,
            .deviceEnabled = state_.device.enabled,
            .deviceHasChildren = state_.device.hasChildren,
            .pageName = state_.device.pageName.c_str()});
    }

    void DeviceView::syncParameter(uint8_t index)
    {
        if (index >= 8 || !body_container_)
            return;

        auto &param = state_.parameters[index];

        if (param.metadataSet && !widgets_[index])
        {
            switch (param.type)
            {
            case Device::Button:
                widgets_[index] = std::make_unique<ParameterButtonWidget>(
                    body_container_, Layout::WIDGET_WIDTH, Layout::WIDGET_HEIGHT, index);
                break;
            case Device::List:
                widgets_[index] = std::make_unique<ParameterListWidget>(
                    body_container_, Layout::WIDGET_WIDTH, Layout::WIDGET_HEIGHT, index, param.discreteCount);
                break;
            case Device::Knob:
            default:
            {
                bool isCentered = (param.origin == 0.5f);
                auto knob = std::make_unique<ParameterKnobWidget>(
                    body_container_, Layout::WIDGET_WIDTH, Layout::WIDGET_HEIGHT, index, isCentered);
                knob->setOrigin(param.origin);
                widgets_[index] = std::move(knob);
                break;
            }
            }

            if (widgets_[index])
            {
                int col = index % 4;
                int row = index / 4;
                lv_obj_set_grid_cell(widgets_[index]->getContainer(),
                                     LV_GRID_ALIGN_CENTER, col, 1,
                                     LV_GRID_ALIGN_CENTER, row, 1);
            }
        }

        if (!widgets_[index])
            return;

        widgets_[index]->setName(param.name.c_str());
        widgets_[index]->setValueWithDisplay(param.value, param.displayValue.c_str());

        if (param.type == Device::Button || param.type == Device::List)
        {
            widgets_[index]->setDiscreteMetadata(param.discreteCount, param.discreteValueNames, param.currentValueIndex);
        }

        lv_obj_t *container = widgets_[index]->getContainer();
        if (container)
        {
            if (param.visible)
                lv_obj_clear_flag(container, LV_OBJ_FLAG_HIDDEN);
            else
                lv_obj_add_flag(container, LV_OBJ_FLAG_HIDDEN);
        }
    }

    void DeviceView::syncPageSelector()
    {
        if (!page_selector_)
            return;

        auto &ps = state_.pageSelector;
        page_selector_->render({
            .names = &ps.names,
            .selectedIndex = ps.selectedIndex,
            .visible = ps.visible});
    }

    void DeviceView::syncDeviceSelector()
    {
        if (!device_selector_)
            return;

        auto &ds = state_.deviceSelector;
        auto &ct = state_.currentTrack;
        device_selector_->render({
            .names = &ds.names,
            .deviceTypes = &ds.deviceTypes,
            .deviceStates = &ds.deviceStates,
            .hasSlots = &ds.hasSlots,
            .hasLayers = &ds.hasLayers,
            .hasDrums = &ds.hasDrums,
            .childrenNames = &ds.childrenNames,
            .childrenTypes = &ds.childrenTypes,
            .trackName = ct.name.c_str(),
            .trackColor = ct.color,
            .trackType = ct.trackType,
            .selectedIndex = ds.currentIndex,
            .showingChildren = ds.showingChildren,
            .showFooter = ds.showFooter,
            .visible = ds.visible});
    }

    void DeviceView::syncTrackSelector()
    {
        if (!track_selector_)
            return;

        auto &ts = state_.trackSelector;
        track_selector_->render({
            .names = &ts.names,
            .muteStates = &ts.muteStates,
            .soloStates = &ts.soloStates,
            .trackTypes = &ts.trackTypes,
            .trackColors = &ts.trackColors,
            .selectedIndex = ts.currentIndex,
            .visible = ts.visible});
    }

    void DeviceView::onActivate()
    {
        if (top_bar_container_)
            lv_obj_clear_flag(top_bar_container_, LV_OBJ_FLAG_HIDDEN);
        if (body_container_)
            lv_obj_clear_flag(body_container_, LV_OBJ_FLAG_HIDDEN);
    }

    void DeviceView::onDeactivate()
    {
        if (top_bar_container_)
            lv_obj_add_flag(top_bar_container_, LV_OBJ_FLAG_HIDDEN);
        if (body_container_)
            lv_obj_add_flag(body_container_, LV_OBJ_FLAG_HIDDEN);
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

        // Reset widget if type changes
        widgets_[paramIndex].reset();

        auto &param = state_.parameters[paramIndex];
        param.type = parameterType;
        param.discreteCount = discreteCount;
        param.discreteValueNames = discreteValueNames;
        param.currentValueIndex = currentValueIndex;
        param.origin = origin;
        param.displayValue = displayValue ? displayValue : "";
        param.metadataSet = true;

        state_.dirty.parameters[paramIndex] = true;
        sync();
    }

    void DeviceView::setParameterValueWithDisplay(uint8_t paramIndex, float normalizedValue,
                                                  const char *displayValue)
    {
        if (!initialized_ || paramIndex >= 8)
            return;

        state_.parameters[paramIndex].value = normalizedValue;
        state_.parameters[paramIndex].displayValue = displayValue ? displayValue : "";
        state_.dirty.parameters[paramIndex] = true;
        sync();
    }

    void DeviceView::setParameterName(uint8_t paramIndex, const char *name)
    {
        if (!initialized_ || paramIndex >= 8 || !name)
            return;

        state_.parameters[paramIndex].name = name;
        state_.dirty.parameters[paramIndex] = true;
        sync();
    }

    void DeviceView::setParameterVisible(uint8_t paramIndex, bool visible)
    {
        if (!initialized_ || paramIndex >= 8)
            return;

        state_.parameters[paramIndex].visible = visible;
        state_.dirty.parameters[paramIndex] = true;
        sync();
    }

    void DeviceView::setParameterLoading(uint8_t paramIndex, bool loading)
    {
        if (!initialized_ || paramIndex >= 8)
            return;

        state_.parameters[paramIndex].loading = loading;
        state_.dirty.parameters[paramIndex] = true;
        sync();
    }

    void DeviceView::setAllWidgetsLoading(bool loading)
    {
        if (!initialized_)
            return;

        for (uint8_t i = 0; i < 8; i++)
        {
            state_.parameters[i].loading = loading;
            state_.dirty.parameters[i] = true;
        }
        sync();
    }

    void DeviceView::setParameterDiscreteValues(uint8_t paramIndex,
                                                const etl::vector<etl::string<16>, 32> &discreteValueNames,
                                                uint8_t currentValueIndex)
    {
        if (!initialized_ || paramIndex >= 8)
            return;

        state_.parameters[paramIndex].discreteValueNames = discreteValueNames;
        state_.parameters[paramIndex].currentValueIndex = currentValueIndex;
        state_.dirty.parameters[paramIndex] = true;
        sync();
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

    void DeviceView::createDeviceStateBar()
    {
        if (!top_bar_container_)
        {
            return;
        }

        top_bar_component_ = std::make_unique<DeviceStateBar>(top_bar_container_);

        top_bar_component_->render({
            .deviceName = "Device 1",
            .deviceEnabled = true,
            .deviceHasChildren = false,
            .pageName = "Page"});

        page_selector_ = std::make_unique<PageSelector>(zone_);

        device_selector_ = std::make_unique<DeviceSelector>(zone_);

        track_selector_ = std::make_unique<TrackSelector>(zone_);
    }

    lv_obj_t *DeviceView::getPageSelectorElement() const
    {
        if (!page_selector_)
            return nullptr;
        return page_selector_->getElement();
    }

    lv_obj_t *DeviceView::getDeviceSelectorElement() const
    {
        if (!device_selector_)
            return nullptr;
        return device_selector_->getElement();
    }

    void DeviceView::updateDeviceState(int displayIndex, bool enabled)
    {
        if (!device_selector_ || !device_selector_->isVisible())
            return;

        // Update state so it persists across navigation
        if (displayIndex >= 0 && displayIndex < static_cast<int>(state_.deviceSelector.deviceStates.size()))
        {
            state_.deviceSelector.deviceStates[displayIndex] = enabled;
        }

        device_selector_->updateDeviceState(displayIndex, enabled);
    }

    lv_obj_t *DeviceView::getTrackSelectorElement() const
    {
        if (!track_selector_)
            return nullptr;
        return track_selector_->getElement();
    }

} // namespace Bitwig
