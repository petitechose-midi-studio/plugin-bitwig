#include "Plugin.hpp"

#include "log/Macros.hpp"
#include "font/FontLoader.hpp"
#include "ui/font/FontLoader.hpp"

// Bitwig-specific icon fonts only (text fonts come from core)
#include "ui/font/data/bitwig_icons_12.c.inc"
#include "ui/font/data/bitwig_icons_14.c.inc"
#include "ui/font/data/bitwig_icons_16.c.inc"

BitwigFonts bitwig_fonts;

namespace Bitwig
{

    void Plugin::loadResources() {
        // Register all icon font sizes
        register_font(&bitwig_fonts.icons_12, bitwig_icons_12_bin, bitwig_icons_12_bin_len);
        register_font(&bitwig_fonts.icons_14, bitwig_icons_14_bin, bitwig_icons_14_bin_len);
        register_font(&bitwig_fonts.icons_16, bitwig_icons_16_bin, bitwig_icons_16_bin_len);

        // Map core's generic fonts to plugin-specific uses
        bitwig_fonts.track_label = fonts.inter_14_bold;
        bitwig_fonts.device_label = fonts.inter_14_semibold;
        bitwig_fonts.page_label = fonts.inter_14_regular;
        bitwig_fonts.param_label = fonts.inter_14_regular;
        bitwig_fonts.lvgl_symbols = &lv_font_montserrat_12;
    }

    Plugin::Plugin(ControllerAPI &api)
        : api_(api),
          enabled_(true),
          protocol_(api),
          viewContainer_(api.getParentContainer()),
          transport_{.bar = TransportBar(viewContainer_.getBottomZone()),
                     .controller = TransportBarController(transport_.bar),
                     .inputHandler = TransportInputHandler(api_, protocol_),
                     .hostHandler = TransportHostHandler(api_, protocol_, transport_.controller)},
          device_{.view = DeviceView(viewContainer_.getMainZone()),
                  .inputHandler = DeviceInputHandler(api_, device_.view, protocol_,
                                                     device_.view.getElement()),
                  .hostHandler = DeviceHostHandler(protocol_, api_, device_.view,
                                                   device_.inputHandler)},
          splash_{.view = SplashView(api_.getParentContainer())},
          lastClicked_{.handler = LastClickedHandler(api_, protocol_, api_.getParentContainer())},
          midi_{.handler = MidiInputHandler(api_, transport_.controller)},
          viewRegistry_({{ViewID::DEVICE, device_.view}, {ViewID::SPLASH, splash_.view}}),
          viewManager_(api, viewRegistry_, viewContainer_),
          lifecycle_{.handler = PluginLifecycleHandler(viewManager_, protocol_)} {}

    Plugin::~Plugin() = default;

    bool Plugin::initialize()
    {
        return true;
    }

} // namespace Bitwig
