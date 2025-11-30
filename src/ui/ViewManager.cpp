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
          viewContainer_(viewContainer),
          currentViewID_(ViewID::SPLASH) {}

    ViewManager::~ViewManager()
    {
        if (splashTimer_)
        {
            lv_timer_delete(splashTimer_);
        }
    }

    void ViewManager::showDefault()
    {
        show(ViewID::DEVICE);
    }

    void ViewManager::show(ViewID id)
    {
        if (currentViewID_ == id)
            return;

        // Deactivate the previous view
        if (currentView_)
        {
            currentView_->onDeactivate();
        }

        lv_obj_clear_flag(viewContainer_.getContainer(), LV_OBJ_FLAG_HIDDEN);
        auto &view = registry_.getView(id);
        api_.showPluginView(view);
        currentViewID_ = id;
        currentView_ = &view;
    }

    void ViewManager::showCore()
    {
        currentViewID_ = ViewID::SPLASH;
        currentView_ = nullptr;
        api_.hidePluginView();
    }

    void ViewManager::showSplash(uint32_t durationMs, const char *message)
    {
        // Deactivate the previous view
        if (currentView_ && currentViewID_ != ViewID::SPLASH)
        {
            currentView_->onDeactivate();
        }

        auto &splashView = registry_.getView<SplashView>(ViewID::SPLASH);
        splashView.setText(message);
        api_.showPluginView(splashView);
        currentViewID_ = ViewID::SPLASH;
        currentView_ = &splashView;

        if (splashTimer_)
        {
            lv_timer_delete(splashTimer_);
        }

        uint32_t fadeStartTime = durationMs > Theme::Animation::FADE_MS
            ? durationMs - Theme::Animation::FADE_MS
            : durationMs;
        splashTimer_ = lv_timer_create(splashTimerCallback, fadeStartTime, this);
        lv_timer_set_repeat_count(splashTimer_, 1);
    }

    void ViewManager::splashTimerCallback(lv_timer_t *timer)
    {
        auto self = static_cast<ViewManager *>(lv_timer_get_user_data(timer));
        auto &splashView = self->registry_.getView<SplashView>(ViewID::SPLASH);

        splashView.fadeOut(Theme::Animation::FADE_MS, [self, &splashView]()
                           {
        self->splashActive_ = false;
        self->showDefault(); });

        self->splashTimer_ = nullptr;
    }

} // namespace Bitwig
