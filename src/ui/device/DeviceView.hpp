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
#include "font/binary_font_buffer.hpp"
#include "widget/ButtonIndicator.hpp"
#include "widget/IParameterWidget.hpp"
#include "component/DeviceStateBar.hpp"
#include "component/PageSelector.hpp"
#include "component/DeviceSelector.hpp"
#include "component/TrackListSelector.hpp"

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
        void setActive(bool active);

        int8_t getWidgetIndexForButton(uint16_t button_id) const;
        void setButtonState(uint16_t button_id, bool pressed, bool animate = true);
        IParameterWidget *getWidgetForButton(uint16_t button_id);

        void showPageSelector(const std::vector<std::string> &pageNames, int currentIndex);
        void setPageSelectorIndex(int index);
        void hidePageSelector();
        int getPageSelectorIndex() const;
        lv_obj_t *getPageSelectorElement() const;

        void showDeviceList(const std::vector<std::string> &names,
                           int currentIndex,
                           const std::vector<bool> &deviceStates,
                           const std::vector<bool> &hasSlots,
                           const std::vector<bool> &hasLayers,
                           const std::vector<bool> &hasDrums);
        void showDeviceChildren(const std::vector<std::string> &items);
        void setDeviceSelectorIndex(int index);
        void hideDeviceSelector();
        void showDeviceSelector();
        int getDeviceSelectorIndex() const;
        int getDeviceSelectorItemCount() const;
        lv_obj_t *getDeviceSelectorElement() const;
        bool isDeviceSelectorVisible() const;
        void updateDeviceState(int displayIndex, bool enabled);

        void showTrackList(const std::vector<std::string> &names,
                          int currentIndex,
                          const std::vector<bool> &muteStates,
                          const std::vector<bool> &soloStates,
                          const std::vector<bool> &groupStates);
        void setTrackListSelectorIndex(int index);
        void hideTrackSelector();
        void showTrackSelector();
        int getTrackListSelectorIndex() const;
        int getTrackListSelectorItemCount() const;
        lv_obj_t *getTrackListSelectorElement() const;
        bool isTrackSelectorVisible() const;
        void updateTrackMuteState(int displayIndex, bool isMuted);
        void updateTrackSoloState(int displayIndex, bool isSoloed);

        /**
         * @brief Set parameter type and metadata (called ONCE per device/page change)
         * @param paramIndex Parameter index (0-7)
         * @param parameterType Widget type detected by Bitwig (0=Knob, 1=Button, 2=List)
         * @param discreteCount Discrete value count (-1=continuous, 2=button, N=list)
         * @param discreteValueNames Array of discrete value names (empty for continuous)
         * @param currentValueIndex Current index in discreteValueNames array
         * @param origin Knob origin (0.0 or 0.5)
         * @param displayValue Initial formatted display value from Bitwig
         */
        void setParameterTypeAndMetadata(uint8_t paramIndex,
                                         uint8_t parameterType,
                                         int16_t discreteCount,
                                         const etl::vector<etl::string<16>, 32> &discreteValueNames,
                                         uint8_t currentValueIndex,
                                         float origin,
                                         const char *displayValue);

        /**
         * @brief Update parameter value and widget display
         * @param paramIndex Parameter index (0-7)
         * @param normalizedValue Normalized value (0.0-1.0)
         */
        void setParameterValue(uint8_t paramIndex, float normalizedValue);

        /**
         * @brief Update parameter value with formatted display text (optimistic updates)
         * @param paramIndex Parameter index (0-7)
         * @param normalizedValue Normalized value (0.0-1.0)
         * @param displayValue Formatted text from Bitwig (e.g., "50.0 Hz", "Sine")
         */
        void setParameterValueWithDisplay(uint8_t paramIndex, float normalizedValue, const char *displayValue);

        /**
         * @brief Update parameter name
         * @param paramIndex Parameter index (0-7)
         * @param name Parameter name (e.g., "Cutoff", "Resonance")
         */
        void setParameterName(uint8_t paramIndex, const char *name);

        /**
         * @brief Show or hide parameter widget
         * @param paramIndex Parameter index (0-7)
         * @param visible True to show, false to hide
         */
        void setParameterVisible(uint8_t paramIndex, bool visible);

        /**
         * @brief Set parameter loading state (NEW - for progressive rendering)
         * @param paramIndex Parameter index (0-7)
         * @param loading True to show loading indicator, false when ready
         */
        void setParameterLoading(uint8_t paramIndex, bool loading);

        /**
         * @brief Set all widgets to loading state (NEW - for device change header)
         * @param loading True to show loading indicators, false when ready
         */
        void setAllWidgetsLoading(bool loading);

        /**
         * @brief Update discrete value names for List parameter (NEW)
         * @param paramIndex Parameter index (0-7)
         * @param discreteValueNames Array of discrete value names
         * @param currentValueIndex Current index in discreteValueNames array
         */
        void setParameterDiscreteValues(uint8_t paramIndex,
                                        const etl::vector<etl::string<16>, 32> &discreteValueNames,
                                        uint8_t currentValueIndex);

        /**
         * @brief Update device name in top bar
         * @param name Device name (e.g., "Operator", "EQ Eight")
         */
        void setDeviceName(const char *name);

        /**
         * @brief Update device enabled/bypassed state
         * @param enabled True if device is active, false if bypassed
         */
        void setDeviceEnabled(bool enabled);

        /**
         * @brief Update device page info
         * @param currentPage Current page index (0-based)
         * @param totalPages Total number of pages
         */
        void setDevicePageInfo(uint8_t currentPage, uint8_t totalPages);

        /**
         * @brief Update device page name
         * @param name Page name (e.g., "General", "Filter")
         */
        void setDevicePageName(const char *name);

        /**
         * @brief Update track name
         * @param name Track name
         */
        void setTrackName(const char *name);

        /**
         * @brief Update track color
         * @param color Track color as uint32 RGB hex (0xRRGGBB)
         */
        void setTrackColor(uint32_t color);

    private:
        Config viewConfig_;
        bool initialized_;
        bool active_;

        std::array<std::unique_ptr<IParameterWidget>, 8> widgets_;
        std::unique_ptr<DeviceStateBar> top_bar_component_;
        std::unique_ptr<PageSelector> page_selector_;
        std::unique_ptr<DeviceSelector> device_selector_;
        std::unique_ptr<TrackListSelector> track_list_selector_;

        lv_obj_t *zone_; // Dedicated zone for this view's content (non-owned)

        lv_obj_t *top_bar_container_;
        lv_obj_t *body_container_;

        lv_obj_t *top_bar_title_label_;
        lv_obj_t *top_bar_status_label_;

        bool is_active_;

        void createUI();
        void setupLayout();
        void createDeviceStateBar();
        void updateDeviceStateBarContent();
        void initializeMappingsFromConfig();
    };

} // namespace Bitwig