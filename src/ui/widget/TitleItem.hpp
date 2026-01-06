#pragma once

#include <memory>

#include <lvgl.h>

#include <oc/ui/lvgl/IWidget.hpp>
#include <oc/ui/lvgl/widget/Label.hpp>

#include "ui/theme/BitwigTheme.hpp"

namespace bitwig::ui {

struct TitleItemProps {
    const char* text = "";
    const char* icon = nullptr;
    const lv_font_t* iconFont = nullptr;
    const lv_font_t* textFont = nullptr;
    uint32_t iconColor = 0xFFFFFF;
    uint32_t textColor = 0xFFFFFF;
    lv_opa_t textOpacity = theme::Opacity::FULL;
    lv_opa_t iconOpacity = theme::Opacity::FULL;
    bool showIndicator = false;
    const char* indicator = nullptr;
    uint32_t indicatorColor = 0x888888;
    bool visible = true;
};

/**
 * @brief Title item widget with optional icon and indicator
 *
 * Layout: [icon?] [label] [indicator?]
 * Uses render(Props) pattern for stateless updates.
 */
class TitleItem : public oc::ui::lvgl::IWidget {
public:
    explicit TitleItem(lv_obj_t* parent);
    ~TitleItem() override;

    // Non-copyable, non-movable
    TitleItem(const TitleItem&) = delete;
    TitleItem& operator=(const TitleItem&) = delete;

    void render(const TitleItemProps& props);
    lv_coord_t getContentWidth() const;
    bool isCreated() const { return container_ != nullptr; }

    lv_obj_t* getElement() const override { return container_; }

private:
    void applyProps(const TitleItemProps& props);

    lv_obj_t* container_ = nullptr;
    lv_obj_t* icon_ = nullptr;
    std::unique_ptr<oc::ui::lvgl::Label> label_;
    lv_obj_t* indicator_ = nullptr;
};

}  // namespace bitwig::ui
