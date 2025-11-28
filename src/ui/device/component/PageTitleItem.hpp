#pragma once

#include <lvgl.h>
#include <string>
#include <memory>
#include "ui/shared/widget/Label.hpp"

namespace Bitwig
{

    /**
     * @brief Reusable page title component
     *
     * Displays page name with appropriate styling.
     * Used in DeviceStateBar (top bar) and potentially PageSelector.
     */
    class PageTitleItem
    {
    public:
        explicit PageTitleItem(lv_obj_t *parent);
        ~PageTitleItem();

        void setName(const std::string &name);

    private:
        std::unique_ptr<Label> label_;
    };

} // namespace Bitwig
