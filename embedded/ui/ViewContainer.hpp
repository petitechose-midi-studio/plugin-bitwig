#pragma once

#include <lvgl.h>

class TransportBar;

namespace UI {
class IView;
}

namespace Plugin::Bitwig {

class ViewContainer {
public:
    ViewContainer(lv_obj_t* parentScreen);
    ~ViewContainer();

    lv_obj_t* getMainZone() const { return mainZone_; }
    lv_obj_t* getBottomZone() const { return bottomZone_; }
    lv_obj_t* getContainer() const { return container_; }

private:
    lv_obj_t* container_{nullptr};
    lv_obj_t* mainZone_{nullptr};
    lv_obj_t* bottomZone_{nullptr};
};

}  // namespace Plugin::Bitwig
