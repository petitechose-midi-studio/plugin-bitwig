#pragma once

#include "ViewRegistry.hpp"

#include <oc/ui/lvgl/IView.hpp>

typedef struct _lv_obj_t lv_obj_t;
typedef struct _lv_timer_t lv_timer_t;

namespace Bitwig {

class ViewContainer;

/**
 * @brief Manages view switching within the Bitwig plugin context
 *
 * Handles view lifecycle (activate/deactivate) and visibility.
 * Views are shown/hidden directly via LVGL.
 */
class ViewManager {
public:
    ViewManager(ViewRegistry& registry, ViewContainer& viewContainer);
    ~ViewManager();

    void showDefault();
    void show(ViewID id);
    void hideAll();
    void showSplash(uint32_t durationMs, const char* message);

private:
    using IView = oc::ui::lvgl::IView;

    ViewRegistry& registry_;
    ViewContainer& view_container_;

    ViewID current_view_id_;
    IView* current_view_{nullptr};
    lv_timer_t* splash_timer_{nullptr};
    bool splash_active_{false};

    void activateView(IView& view);
    void deactivateCurrentView();

    static void splashTimerCallback(lv_timer_t* timer);
};

}  // namespace Bitwig
