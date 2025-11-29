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
     * Uses lazy initialization - LVGL widget created on first use.
     */
    class PageTitleItem
    {
    public:
        explicit PageTitleItem(lv_obj_t *parent);
        ~PageTitleItem();

        void setName(const std::string &name);

    private:
        void ensureCreated();

        lv_obj_t *parent_ = nullptr;
        lv_obj_t *label_ = nullptr;
        std::string pending_name_;
    };

} // namespace Bitwig
