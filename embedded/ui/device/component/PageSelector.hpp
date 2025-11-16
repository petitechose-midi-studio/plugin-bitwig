#pragma once

#include "resource/common/ui/widget/ListOverlay.hpp"
#include "resource/common/ui/interface/IComponent.hpp"
#include <string>
#include <vector>

namespace Bitwig
{

    /**
     * @brief Device page selector component
     *
     * Encapsulates ListOverlay for device page selection.
     * Manages page list state specific to Bitwig Device context.
     *
     * PURE UI COMPONENT - No logic, no callbacks, only setters/getters.
     *
     * Usage (from DeviceController):
     *   pageSelector.setPageNames({"General", "Filter", "Envelope"});
     *   pageSelector.setCurrentPageIndex(1);
     *   pageSelector.show();
     *
     *   // Later, from controller:
     *   int selected = pageSelector.getSelectedIndex();
     *   pageSelector.hide();
     */
    class PageSelector : public UI::IComponent
    {
    public:
        /**
         * @brief Construct page selector
         * @param parent Parent LVGL object (typically screen)
         */
        explicit PageSelector(lv_obj_t *parent);

        /**
         * @brief Destructor
         */
        ~PageSelector() = default;

        /**
         * @brief Set available page names
         * @param pageNames Vector of page name strings
         */
        void setPageNames(const std::vector<std::string> &pageNames);

        /**
         * @brief Set current page index (initial selection when showing)
         * @param index Current page index (0-based)
         */
        void setCurrentPageIndex(int index);

        /**
         * @brief Set selected page index (navigation)
         * @param index Selected index (0-based), clamped to valid range
         */
        void setSelectedIndex(int index);

        /**
         * @brief Show page selector overlay (from IComponent)
         */
        void show() override;

        /**
         * @brief Hide page selector overlay (from IComponent)
         */
        void hide() override;

        /**
         * @brief Check if selector is visible (from IComponent)
         * @return true if visible
         */
        bool isVisible() const override;

        /**
         * @brief Get currently selected page index
         * @return Selected index (0-based), -1 if no pages
         */
        int getSelectedIndex() const;

        /**
         * @brief Get number of pages
         * @return Page count
         */
        int getPageCount() const;

        /**
         * @brief Get underlying LVGL element (from IElement)
         * @return Overlay object (nullptr if not created)
         */
        lv_obj_t *getElement() const override { return overlay_.getElement(); }

    private:
        ListOverlay overlay_;
        std::vector<std::string> page_names_;
        int current_page_index_ = 0;
    };

} // namespace Bitwig
