#include "Plugin.hpp"

#include "font/FontLoader.hpp"
#include "ui/font/FontLoader.hpp"

// Bitwig-specific icon fonts only (text fonts come from core)
#include "ui/font/data/bitwig_icons_12.c.inc"
#include "ui/font/data/bitwig_icons_14.c.inc"
#include "ui/font/data/bitwig_icons_16.c.inc"

BitwigFonts bitwig_fonts;

namespace Bitwig {

void Plugin::loadResources() {
    // Register all icon font sizes
    registerFont(&bitwig_fonts.icons_12, bitwig_icons_12_bin, bitwig_icons_12_bin_len);
    registerFont(&bitwig_fonts.icons_14, bitwig_icons_14_bin, bitwig_icons_14_bin_len);
    registerFont(&bitwig_fonts.icons_16, bitwig_icons_16_bin, bitwig_icons_16_bin_len);

    // Map core's generic fonts to plugin-specific uses
    bitwig_fonts.track_label = fonts.inter_14_bold;
    bitwig_fonts.device_label = fonts.inter_14_semibold;
    bitwig_fonts.page_label = fonts.inter_14_regular;
    bitwig_fonts.param_label = fonts.inter_14_regular;
    bitwig_fonts.lvgl_symbols = &lv_font_montserrat_12;
}

Plugin::Plugin(ControllerAPI &api)
    : api_(api), enabled_(true), protocol_(api), view_container_(api.getParentContainer()),
      // Views and state (no handler dependencies)
      transport_bar_(view_container_.getBottomZone()), transport_controller_(transport_bar_),
      device_view_(view_container_.getMainZone()), splash_view_(api_.getParentContainer()),
      last_clicked_state_{},
      // Handlers (can safely reference views/state initialized above)
      transport_handlers_{.input = TransportInputHandler(api_, protocol_),
                          .host = TransportHostHandler(api_, protocol_, transport_controller_)},
      device_handlers_{
          .input = DeviceInputHandler(api_, device_view_, protocol_, device_view_.getElement()),
          .host = DeviceHostHandler(protocol_, api_, device_view_, device_handlers_.input)},
      last_clicked_handlers_{.input = LastClickedInputHandler(api_, protocol_, last_clicked_state_,
                                                              api_.getParentContainer()),
                             .host = LastClickedHostHandler(api_, protocol_, last_clicked_state_)},
      midi_handler_(api_, transport_controller_),
      // View management
      view_manager_(api, view_registry_, view_container_),
      lifecycle_handler_(view_manager_, protocol_) {
    view_registry_.add(ViewID::DEVICE, device_view_);
    view_registry_.add(ViewID::SPLASH, splash_view_);
}

Plugin::~Plugin() = default;

bool Plugin::initialize() { return true; }

}  // namespace Bitwig
