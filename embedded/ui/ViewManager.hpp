#pragma once

#include "ViewRegistry.hpp"
#include <functional>

class ControllerAPI;
typedef struct _lv_obj_t lv_obj_t;
typedef struct _lv_timer_t lv_timer_t;

namespace Plugin::Bitwig {

class ViewContainer;

class ViewManager {
public:
    ViewManager(ControllerAPI& api, ViewRegistry& registry, ViewContainer& viewContainer);
    ~ViewManager();

    void showDefault();
    void show(ViewID id);
    void showCore();
    void showSplash(uint32_t durationMs, const char* message);

private:
    ControllerAPI& api_;
    ViewRegistry& registry_;
    ViewContainer& viewContainer_;

    ViewID currentViewID_;
    UI::IView* currentView_{nullptr};
    lv_timer_t* splashTimer_{nullptr};
    bool splashActive_{false};

    static void splashTimerCallback(lv_timer_t* timer);
};

}  // namespace Plugin::Bitwig
