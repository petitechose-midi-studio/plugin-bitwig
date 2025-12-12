#pragma once

#include <string>

#include <oc/ui/lvgl/IComponent.hpp>

namespace Bitwig {

/**
 * @brief Interface for list selector components
 *
 * Provides navigation, selection and visibility control.
 */
class ISelector : public oc::ui::lvgl::IComponent {
public:
    ~ISelector() override = default;

    virtual void setTitle(const std::string& title) = 0;
    virtual void setSelectedIndex(int index) = 0;
    virtual int getSelectedIndex() const = 0;
    virtual int getItemCount() const = 0;
};

}  // namespace Bitwig
