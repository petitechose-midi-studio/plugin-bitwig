#include "TrackSelector.hpp"
#include "ui/theme/BitwigTheme.hpp"
#include "ui/font/icon.hpp"
#include "ui/font/FontLoader.hpp"

using namespace Theme;

namespace Bitwig
{

TrackSelector::TrackSelector(lv_obj_t *parent)
    : BaseSelector(parent)
{
    setTitle("Tracks");
    createFooter();  // Create footer once, after list - ensures correct flex order
}

TrackSelector::~TrackSelector()
{
    footer_.reset();
}

void TrackSelector::render(const TrackSelectorProps &props)
{
    if (!props.visible)
    {
        hide();
        if (footer_)
            footer_->hide();
        return;
    }

    if (!props.names)
        return;

    const auto &names = *props.names;

    // Check if items changed
    bool itemsChanged = (prev_items_ != names);

    if (itemsChanged)
    {
        clearTrackItems();
        prev_items_ = names;
    }

    overlay().setItems(names);
    overlay().setSelectedIndex(props.selectedIndex);

    if (itemsChanged)
    {
        createTrackItems(names);
    }

    renderTrackItems(props);

    if (!isVisible())
        show();

    renderFooter(props);
}

void TrackSelector::createFooter()
{
    // Create footer inside overlay container (participates in flex layout)
    footer_ = std::make_unique<UI::HintBar>(overlay().getContainer(), UI::HintBarPosition::Bottom);
    footer_->setSize(Layout::HINT_BAR_HEIGHT);

    // Create mute icon (cell 1 = center)
    footer_mute_ = lv_label_create(footer_->getElement());
    Icon::set(footer_mute_, Icon::MUTE, Icon::L);
    lv_obj_set_style_text_color(footer_mute_, lv_color_hex(Color::TRACK_MUTE), 0);
    footer_->setCell(1, footer_mute_);

    // Create solo icon (cell 2 = right)
    footer_solo_ = lv_label_create(footer_->getElement());
    Icon::set(footer_solo_, Icon::SOLO, Icon::L);
    lv_obj_set_style_text_color(footer_solo_, lv_color_hex(Color::TRACK_SOLO), 0);
    footer_->setCell(2, footer_solo_);
}

void TrackSelector::createTrackItems(const std::vector<std::string> &names)
{
    track_items_.clear();

    bool hasBackItem = !names.empty() && (names[0] == Icon::ARROW_LEFT);

    // Apply icon font to back button if present
    if (hasBackItem)
    {
        overlay().setItemFont(0, bitwig_fonts.icons_14);
        track_items_.push_back(nullptr);
    }

    // Create TrackTitleItem for regular items (skip back button if present)
    size_t startIndex = hasBackItem ? 1 : 0;
    for (size_t i = startIndex; i < names.size(); i++)
    {
        lv_obj_t *btn = overlay().getButton(i);
        if (!btn)
            continue;

        // Remove default label to replace with TrackTitleItem
        uint32_t childCount = lv_obj_get_child_cnt(btn);
        for (uint32_t j = 0; j < childCount; j++)
        {
            lv_obj_t *child = lv_obj_get_child(btn, j);
            if (child && lv_obj_check_type(child, &lv_label_class))
            {
                lv_obj_delete(child);
                break;
            }
        }

        auto item = std::make_unique<TrackTitleItem>(btn, true, 12);
        track_items_.push_back(std::move(item));
    }
}

void TrackSelector::clearTrackItems()
{
    track_items_.clear();
}

void TrackSelector::renderTrackItems(const TrackSelectorProps &props)
{
    if (!props.names)
        return;

    const auto &names = *props.names;
    size_t count = names.size();

    for (size_t i = 0; i < track_items_.size() && i < count; i++)
    {
        if (!track_items_[i])
            continue;

        bool isMuted = props.muteStates && i < props.muteStates->size() ? (*props.muteStates)[i] : false;
        bool isSoloed = props.soloStates && i < props.soloStates->size() ? (*props.soloStates)[i] : false;
        uint8_t trackType = props.trackTypes && i < props.trackTypes->size() ? (*props.trackTypes)[i] : 0;
        uint32_t trackColor = props.trackColors && i < props.trackColors->size() ? (*props.trackColors)[i] : 0xFFFFFF;

        track_items_[i]->render({
            .name = names[i].c_str(),
            .color = trackColor,
            .trackType = trackType,
            .isMuted = isMuted,
            .isSoloed = isSoloed,
            .highlighted = static_cast<int>(i) == props.selectedIndex});
    }
}

void TrackSelector::renderFooter(const TrackSelectorProps &props)
{
    if (!footer_)
        return;

    int idx = props.selectedIndex;
    bool isMuted = props.muteStates && idx >= 0 && idx < static_cast<int>(props.muteStates->size())
                       ? (*props.muteStates)[idx]
                       : false;
    bool isSoloed = props.soloStates && idx >= 0 && idx < static_cast<int>(props.soloStates->size())
                        ? (*props.soloStates)[idx]
                        : false;

    // Update opacity based on state (active = full, inactive = faded)
    if (footer_mute_)
        lv_obj_set_style_text_opa(footer_mute_, isMuted ? LV_OPA_COVER : LV_OPA_40, 0);
    if (footer_solo_)
        lv_obj_set_style_text_opa(footer_solo_, isSoloed ? LV_OPA_COVER : LV_OPA_40, 0);

    footer_->show();
}

} // namespace Bitwig
