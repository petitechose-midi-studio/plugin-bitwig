#include "ViewManager.hpp"

#include "ViewContainer.hpp"

#include <lvgl.h>

#include "splash/SplashView.hpp"
#include "theme/BitwigTheme.hpp"

namespace Bitwig {

ViewManager::ViewManager(ViewRegistry& registry, ViewContainer& viewContainer)
    : registry_(registry), view_container_(viewContainer),
      current_view_id_(ViewID::SPLASH) {}

ViewManager::~ViewManager() {
    if (splash_timer_) { lv_timer_delete(splash_timer_); }
}

void ViewManager::showDefault() { show(ViewID::DEVICE); }

void ViewManager::show(ViewID id) {
    if (current_view_id_ == id) return;

    deactivateCurrentView();
    activateView(registry_.get(id));
    current_view_id_ = id;
}

void ViewManager::hideAll() {
    deactivateCurrentView();
    lv_obj_add_flag(view_container_.getContainer(), LV_OBJ_FLAG_HIDDEN);
    current_view_id_ = ViewID::SPLASH;
}

void ViewManager::showSplash(uint32_t durationMs, const char* message) {
    deactivateCurrentView();

    auto& splash_view = registry_.get<SplashView>(ViewID::SPLASH);
    splash_view.setText(message);
    activateView(splash_view);
    current_view_id_ = ViewID::SPLASH;

    if (splash_timer_) { lv_timer_delete(splash_timer_); }

    uint32_t fade_start_time = durationMs > Theme::Animation::FADE_MS
                                   ? durationMs - Theme::Animation::FADE_MS
                                   : durationMs;
    splash_timer_ = lv_timer_create(splashTimerCallback, fade_start_time, this);
    lv_timer_set_repeat_count(splash_timer_, 1);
}

void ViewManager::activateView(IView& view) {
    lv_obj_clear_flag(view_container_.getContainer(), LV_OBJ_FLAG_HIDDEN);

    if (view.getElement()) {
        lv_obj_clear_flag(view.getElement(), LV_OBJ_FLAG_HIDDEN);
    }
    view.onActivate();
    current_view_ = &view;
}

void ViewManager::deactivateCurrentView() {
    if (current_view_) {
        current_view_->onDeactivate();
        if (current_view_->getElement()) {
            lv_obj_add_flag(current_view_->getElement(), LV_OBJ_FLAG_HIDDEN);
        }
        current_view_ = nullptr;
    }
}

void ViewManager::splashTimerCallback(lv_timer_t* timer) {
    auto self = static_cast<ViewManager*>(lv_timer_get_user_data(timer));
    auto& splash_view = self->registry_.get<SplashView>(ViewID::SPLASH);

    splash_view.fadeOut(Theme::Animation::FADE_MS, [self]() {
        self->splash_active_ = false;
        self->showDefault();
    });

    self->splash_timer_ = nullptr;
}

}  // namespace Bitwig
