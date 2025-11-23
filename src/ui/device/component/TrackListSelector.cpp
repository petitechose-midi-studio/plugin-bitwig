#include "TrackListSelector.hpp"
#include "font/binary_font_buffer.hpp"
#include "../../theme/BitwigTheme.hpp"

namespace Bitwig
{

    TrackListSelector::TrackListSelector(lv_obj_t *parent)
        : parent_(parent), overlay_(parent)
    {
        overlay_.setTitle("Select Track");
    }

    TrackListSelector::~TrackListSelector()
    {
        destroyFooter();
    }

    void TrackListSelector::setTitle(const std::string &title)
    {
        overlay_.setTitle(title);
    }

    void TrackListSelector::setItems(const std::vector<std::string> &items)
    {
        items_ = items;
        overlay_.setItems(items_);
    }

    void TrackListSelector::setTrackItems(const std::vector<std::string> &items,
                                          int currentIndex,
                                          const std::vector<bool> &muteStates,
                                          const std::vector<bool> &soloStates,
                                          const std::vector<bool> &groupStates,
                                          const std::vector<uint32_t> &trackColors)
    {
        items_ = items;
        current_item_index_ = currentIndex;
        overlay_.setTrackItems(items, currentIndex, muteStates, soloStates, groupStates, trackColors);
    }

    void TrackListSelector::setCurrentItemIndex(int index)
    {
        current_item_index_ = index;
        overlay_.setSelectedIndex(index);
    }

    void TrackListSelector::setSelectedIndex(int index)
    {
        overlay_.setSelectedIndex(index);
    }

    void TrackListSelector::show()
    {
        overlay_.setSelectedIndex(current_item_index_);
        overlay_.show();

        // Show/create footer
        if (!footer_container_)
        {
            createFooter();
        }
        else
        {
            lv_obj_clear_flag(footer_container_, LV_OBJ_FLAG_HIDDEN);
        }
    }

    void TrackListSelector::hide()
    {
        overlay_.hide();

        // Hide footer when overlay is hidden
        if (footer_container_)
        {
            lv_obj_add_flag(footer_container_, LV_OBJ_FLAG_HIDDEN);
        }
    }

    bool TrackListSelector::isVisible() const
    {
        return overlay_.isVisible();
    }

    int TrackListSelector::getSelectedIndex() const
    {
        return overlay_.getSelectedIndex();
    }

    int TrackListSelector::getItemCount() const
    {
        return overlay_.getItemCount();
    }

    void TrackListSelector::createFooter()
    {
        if (!parent_)
            return;

        // Create footer container at parent screen level (fixed position, bottom center, full width)
        footer_container_ = lv_obj_create(parent_);
        lv_obj_add_flag(footer_container_, LV_OBJ_FLAG_FLOATING);   // Remove from parent layout
        lv_obj_set_size(footer_container_, LV_PCT(100), 30);        // Full width, fixed height
        lv_obj_align(footer_container_, LV_ALIGN_BOTTOM_MID, 0, 0); // Centered at bottom, flush with edge
        lv_obj_set_style_bg_opa(footer_container_, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_width(footer_container_, 0, 0);
        lv_obj_set_style_pad_left(footer_container_, 10, 0);
        lv_obj_set_style_pad_right(footer_container_, 10, 0);
        lv_obj_set_style_pad_top(footer_container_, 0, 0); // No vertical padding
        lv_obj_set_style_pad_bottom(footer_container_, 0, 0);

        // Grid layout: 1 row × 3 columns (left, center, right - each taking equal space)
        lv_obj_set_layout(footer_container_, LV_LAYOUT_GRID);

        static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST}; // Equal fractions
        static lv_coord_t row_dsc[] = {LV_GRID_CONTENT, LV_GRID_TEMPLATE_LAST};
        lv_obj_set_grid_dsc_array(footer_container_, col_dsc, row_dsc);

        lv_obj_set_style_pad_column(footer_container_, 10, 0); // Gap between columns

        // Cell gauche (0, 0) vide (pas de label créé)

        // Label centre : "Mute" (cell 1, 0) - centered in its cell
        footer_center_label_ = lv_label_create(footer_container_);
        lv_label_set_text(footer_center_label_, "Mute");
        lv_obj_set_style_text_color(footer_center_label_, lv_color_hex(Theme::Color::TEXT_LIGHT), 0);
        lv_obj_set_grid_cell(footer_center_label_, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);

        // Label droit : "Solo" (cell 2, 0) - right aligned in its cell
        footer_left_label_ = lv_label_create(footer_container_); // Reuse footer_left_label_ member for "Solo"
        lv_label_set_text(footer_left_label_, "Solo");
        lv_obj_set_style_text_color(footer_left_label_, lv_color_hex(Theme::Color::TEXT_LIGHT), 0);
        lv_obj_set_grid_cell(footer_left_label_, LV_GRID_ALIGN_END, 2, 1, LV_GRID_ALIGN_CENTER, 0, 1);

        // Use small font if available
        if (fonts.tempo_label)
        {
            lv_obj_set_style_text_font(footer_center_label_, fonts.tempo_label, 0);
            lv_obj_set_style_text_font(footer_left_label_, fonts.tempo_label, 0); // "Solo" label
        }
    }

    void TrackListSelector::destroyFooter()
    {
        if (footer_container_)
        {
            lv_obj_del(footer_container_);
            footer_container_ = nullptr;
            footer_left_label_ = nullptr;
            footer_center_label_ = nullptr;
        }
    }

    void TrackListSelector::setTrackMuteStateAtIndex(uint8_t trackIndex, bool isMuted)
    {
        overlay_.setTrackMuteStateAtIndex(trackIndex, isMuted);
    }

    void TrackListSelector::setTrackSoloStateAtIndex(uint8_t trackIndex, bool isSoloed)
    {
        overlay_.setTrackSoloStateAtIndex(trackIndex, isSoloed);
    }

    bool TrackListSelector::getTrackMuteStateAtIndex(uint8_t trackIndex) const
    {
        return overlay_.getTrackMuteStateAtIndex(trackIndex);
    }

    bool TrackListSelector::getTrackSoloStateAtIndex(uint8_t trackIndex) const
    {
        return overlay_.getTrackSoloStateAtIndex(trackIndex);
    }

} // namespace Bitwig
