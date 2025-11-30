#include "TrackSelector.hpp"
#include "../../theme/BitwigTheme.hpp"
#include "ui/font/icon.hpp"
#include "ui/font/FontLoader.hpp"

using namespace Theme;

namespace Bitwig
{

TrackSelector::TrackSelector(lv_obj_t *parent)
    : BaseSelector(parent)
{
    setTitle("Tracks");
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
            footer_->render({.visible = false});
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

    if (!footer_)
        createFooter();

    renderFooter(props);
}

void TrackSelector::createFooter()
{
    footer_ = std::make_unique<ButtonHintBar>(parent_);
}

void TrackSelector::createTrackItems(const std::vector<std::string> &names)
{
    track_items_.clear();

    for (size_t i = 0; i < names.size(); i++)
    {
        lv_obj_t *btn = overlay().getButton(i);
        if (!btn)
            continue;

        bool isBackItem = (i == 0) && !names.empty() && (names[0] == Icon::ARROW_LEFT);

        // Remove default label
        uint32_t childCount = lv_obj_get_child_cnt(btn);
        for (uint32_t j = 0; j < childCount; j++)
        {
            lv_obj_t *child = lv_obj_get_child(btn, j);
            if (child && lv_obj_check_type(child, &lv_label_class))
            {
                if (isBackItem)
                {
                    if (bitwig_fonts.icons_14)
                        lv_obj_set_style_text_font(child, bitwig_fonts.icons_14, 0);
                    lv_obj_set_style_text_color(child, lv_color_hex(Color::INACTIVE_LIGHTER), LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(child, lv_color_hex(Color::TEXT_PRIMARY), LV_STATE_FOCUSED);
                }
                else
                {
                    lv_obj_delete(child);
                }
                break;
            }
        }

        if (!isBackItem)
        {
            auto item = std::make_unique<TrackTitleItem>(btn, true, 12);
            track_items_.push_back(std::move(item));
        }
        else
        {
            track_items_.push_back(nullptr);
        }
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

    footer_->render({
        .center = {.icon = Icon::MUTE, .color = Color::TRACK_MUTE, .active = isMuted},
        .right = {.icon = Icon::SOLO, .color = Color::TRACK_SOLO, .active = isSoloed},
        .visible = true});
}

} // namespace Bitwig
