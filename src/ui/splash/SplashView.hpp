#pragma once

#include <functional>

#include <lvgl.h>

#include "interface/IView.hpp"

namespace Bitwig {

class SplashView : public IView {
public:
    explicit SplashView(lv_obj_t *zone);
    ~SplashView();

    void onActivate() override;
    void onDeactivate() override;
    const char *getViewId() const override { return "bitwig.splash"; }
    lv_obj_t *getElement() const override { return zone_; }

    void setText(const char *message);
    void fadeOut(uint32_t durationMs, std::function<void()> onComplete);
private:
    lv_obj_t *zone_;
    lv_obj_t *container_{nullptr};
    lv_obj_t *logo_{nullptr};
    lv_obj_t *label_{nullptr};

    std::function<void()> fade_callback_;

    void createUI();
    void destroyUI();

    static void fadeAnimCallback(void *var, int32_t value);
    static void fadeAnimDeletedCallback(lv_anim_t *anim);
};

}  // namespace Bitwig
