#pragma once

#include <lvgl.h>

#include <array>
#include <map>
#include <memory>
#include <etl/array.h>
#include <etl/string.h>
#include <etl/vector.h>

#include "../theme/BitwigTheme.hpp"
#include "../transportbar/TransportBar.hpp"
#include "interface/IView.hpp"
#include "font/FontLoader.hpp"
#include "widget/ButtonIndicator.hpp"
#include "widget/IParameterWidget.hpp"
#include "component/DeviceStateBar.hpp"
#include "component/PageSelector.hpp"
#include "component/DeviceSelector.hpp"
#include "component/TrackSelector.hpp"
#include "../state/DeviceState.hpp"

namespace Bitwig
{

    class DeviceView : public UI::IView
    {
    public:
        struct Config
        {
            lv_color_t bg_color;
            lv_color_t container_color;

            Config()
                : bg_color(lv_color_hex(Theme::Color::BACKGROUND_FILL)),
                  container_color(lv_color_hex(Theme::Color::BACKGROUND_FILL)) {}

            bool isValid() const
            {
                return true;
            }
        };

        explicit DeviceView(lv_obj_t *zone, const Config &viewConfig = Config());
        ~DeviceView();

        void onActivate() override;
        void onDeactivate() override;

        const char *getViewId() const override
        {
            return "bitwig.device";
        }

        lv_obj_t *getElement() const override
        {
            return zone_;
        }

        void render();
        void update();
        void sync();
        void setActive(bool active);

        DeviceState &state() { return state_; }

        lv_obj_t *getPageSelectorElement() const;
        lv_obj_t *getDeviceSelectorElement() const;
        lv_obj_t *getTrackSelectorElement() const;

        void updateDeviceState(int displayIndex, bool enabled);

        void setParameterTypeAndMetadata(uint8_t paramIndex,
                                         uint8_t parameterType,
                                         int16_t discreteCount,
                                         const etl::vector<etl::string<16>, 32> &discreteValueNames,
                                         uint8_t currentValueIndex,
                                         float origin,
                                         const char *displayValue);

        void setParameterValueWithDisplay(uint8_t paramIndex, float normalizedValue, const char *displayValue);
        void setParameterName(uint8_t paramIndex, const char *name);
        void setParameterVisible(uint8_t paramIndex, bool visible);
        void setParameterLoading(uint8_t paramIndex, bool loading);
        void setAllWidgetsLoading(bool loading);
        void setParameterDiscreteValues(uint8_t paramIndex,
                                        const etl::vector<etl::string<16>, 32> &discreteValueNames,
                                        uint8_t currentValueIndex);

    private:
        DeviceState state_;
        Config viewConfig_;
        bool initialized_;
        bool active_;

        std::array<std::unique_ptr<IParameterWidget>, 8> widgets_;
        std::unique_ptr<DeviceStateBar> top_bar_component_;
        std::unique_ptr<PageSelector> page_selector_;
        std::unique_ptr<DeviceSelector> device_selector_;
        std::unique_ptr<TrackSelector> track_selector_;

        lv_obj_t *zone_; // Dedicated zone for this view's content (non-owned)

        lv_obj_t *top_bar_container_;
        lv_obj_t *body_container_;


        void createUI();
        void setupLayout();
        void createDeviceStateBar();
        void updateDeviceStateBarContent();
        void initializeMappingsFromConfig();

        void syncDeviceInfo();
        void syncParameter(uint8_t index);
        void syncPageSelector();
        void syncDeviceSelector();
        void syncTrackSelector();
    };

} // namespace Bitwig