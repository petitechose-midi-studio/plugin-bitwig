#pragma once

#include <lvgl.h>

class TransportBar;

namespace UI {
class IView;
}

namespace Bitwig {

class ViewContainer {
public:
    ViewContainer(lv_obj_t *parentScreen);
    ~ViewContainer();

    lv_obj_t *getMainZone() const { return main_zone_; }
    lv_obj_t *getBottomZone() const { return bottom_zone_; }
    lv_obj_t *getContainer() const { return container_; }
private:
    lv_obj_t *container_{nullptr};
    lv_obj_t *main_zone_{nullptr};
    lv_obj_t *bottom_zone_{nullptr};
};

}  // namespace Bitwig
