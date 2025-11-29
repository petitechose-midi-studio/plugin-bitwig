#pragma once

#include <lvgl.h>
#include <string>

namespace Bitwig
{

    /**
     * @brief Reusable page title component
     *
     * Displays page name with appropriate styling.
     * Creates label directly in parent (no intermediate container).
     * Used in DeviceStateBar (top bar) and potentially PageSelector.
     */
    class PageTitleItem
    {
    public:
        explicit PageTitleItem(lv_obj_t *parent);
        ~PageTitleItem();

        void setName(const std::string &name);

    private:
        lv_obj_t *label_ = nullptr;  // Raw lv_label, owned by LVGL parent
    };

} // namespace Bitwig
