#include "ViewManager.hpp"

#include <lvgl.h>

#include "api/ControllerAPI.hpp"
#include "ViewContainer.hpp"
#include "interface/IView.hpp"
#include "log/Macros.hpp"
#include "splash/SplashView.hpp"
#include "theme/BitwigTheme.hpp"

namespace Bitwig
{

    ViewManager::ViewManager(ControllerAPI &api, ViewRegistry &registry, ViewContainer &viewContainer)
        : api_(api),
          registry_(registry),
          view_container_(viewContainer),
          current_view_id_(ViewID::SPLASH) {}

    ViewManager::~ViewManager()
    {
        if (splash_timer_)
        {
            lv_timer_delete(splash_timer_);
        }
    }

    void ViewManager::showDefault()
    {
        show(ViewID::DEVICE);
    }

    void ViewManager::show(ViewID id)
    {
        if (current_view_id_ == id)
            return;

        // Deactivate the previous view
        if (current_view_)
        {
            current_view_->onDeactivate();
        }

        lv_obj_clear_flag(view_container_.getContainer(), LV_OBJ_FLAG_HIDDEN);
        auto &view = registry_.getView(id);
        api_.showPluginView(view);
        current_view_id_ = id;
        current_view_ = &view;
    }

    void ViewManager::showCore()
    {
        current_view_id_ = ViewID::SPLASH;
        current_view_ = nullptr;
        api_.hidePluginView();
    }

    void ViewManager::showSplash(uint32_t durationMs, const char *message)
    {
        // Deactivate the previous view
        if (current_view_ && current_view_id_ != ViewID::SPLASH)
        {
            current_view_->onDeactivate();
        }

        auto &splashView = registry_.getView<SplashView>(ViewID::SPLASH);
        splashView.setText(message);
        api_.showPluginView(splashView);
        current_view_id_ = ViewID::SPLASH;
        current_view_ = &splashView;

        if (splash_timer_)
        {
            lv_timer_delete(splash_timer_);
        }

        uint32_t fadeStartTime = durationMs > Theme::Animation::FADE_MS
            ? durationMs - Theme::Animation::FADE_MS
            : durationMs;
        splash_timer_ = lv_timer_create(splashTimerCallback, fadeStartTime, this);
        lv_timer_set_repeat_count(splash_timer_, 1);
    }

    void ViewManager::splashTimerCallback(lv_timer_t *timer)
    {
        auto self = static_cast<ViewManager *>(lv_timer_get_user_data(timer));
        auto &splashView = self->registry_.getView<SplashView>(ViewID::SPLASH);

        splashView.fadeOut(Theme::Animation::FADE_MS, [self, &splashView]()
                           {
        self->splash_active_ = false;
        self->showDefault(); });

        self->splash_timer_ = nullptr;
    }

} // namespace Bitwig
