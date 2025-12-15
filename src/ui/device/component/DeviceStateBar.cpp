#include "DeviceStateBar.hpp"

#include <oc/ui/lvgl/style/StyleBuilder.hpp>

#include "ui/font/BitwigFonts.hpp"
#include "ui/theme/BitwigTheme.hpp"

using namespace Theme;
namespace style = oc::ui::lvgl::style;

namespace bitwig {

DeviceStateBar::DeviceStateBar(lv_obj_t *parent) : parent_(parent) {
    if (!parent_) return;

    container_ = lv_obj_create(parent_);
    if (!container_) return;

    lv_obj_set_size(container_, LV_PCT(100), Layout::TOP_BAR_HEIGHT);
    lv_obj_set_pos(container_, 0, 0);
    style::apply(container_).bgColor(Color::BACKGROUND_FILL).padH(Layout::PAD_SM).padV(0);
    lv_obj_set_style_border_width(container_, 0, LV_STATE_DEFAULT);
    lv_obj_set_scrollbar_mode(container_, LV_SCROLLBAR_MODE_OFF);

    // Grid: 2 columns (device | page)
    static const lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static const lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_layout(container_, LV_LAYOUT_GRID);
    lv_obj_set_grid_dsc_array(container_, col_dsc, row_dsc);
    lv_obj_set_style_pad_column(container_, Layout::GAP_MD, LV_STATE_DEFAULT);

    device_cell_ = createCellWrapper(container_, LV_FLEX_ALIGN_START);
    lv_obj_set_grid_cell(device_cell_, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
    device_item_ =
        std::make_unique<DeviceTitleItem>(device_cell_, DeviceTitleItem::IconSize::MEDIUM);

    page_cell_ = createCellWrapper(container_, LV_FLEX_ALIGN_END);
    lv_obj_set_grid_cell(page_cell_, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
    page_item_ = std::make_unique<TitleItem>(page_cell_);
}

DeviceStateBar::~DeviceStateBar() {
    if (container_) {
        lv_obj_delete(container_);
        container_ = nullptr;
    }
}

void DeviceStateBar::render(const DeviceStateBarProps &props) {
    if (device_item_) {
        device_item_->render({.name = props.deviceName,
                              .deviceType = props.deviceType,
                              .enabled = props.deviceEnabled,
                              .hasChildren = props.deviceHasChildren});
    }

    if (page_item_) {
        TitleItemProps pageProps;
        pageProps.text = (props.pageName && props.pageName[0]) ? props.pageName : "Page";
        pageProps.textColor = Color::TEXT_LIGHT;
        pageProps.textFont = bitwig_fonts.page_label;
        page_item_->render(pageProps);
    }
}

lv_obj_t *DeviceStateBar::createCellWrapper(lv_obj_t *parent, lv_flex_align_t hAlign) {
    lv_obj_t *cell = lv_obj_create(parent);
    lv_obj_set_size(cell, LV_PCT(100), LV_PCT(100));
    style::apply(cell).transparent().noScroll().flexRow(hAlign, Layout::GAP_MD);
    lv_obj_remove_flag(cell, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
    return cell;
}

}  // namespace bitwig
