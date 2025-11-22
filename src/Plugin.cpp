#include "Plugin.hpp"

#include "log/Macros.hpp"

namespace Bitwig
{

    Plugin::Plugin(ControllerAPI &api)
        : api_(api),
          enabled_(true),
          protocol_(api),
          viewContainer_(api.getParentContainer()),
          transport_{.bar = TransportBar(viewContainer_.getBottomZone()),
                     .controller = TransportBarController(transport_.bar),
                     .inputHandler = TransportInputHandler(api_, transport_.controller, protocol_),
                     .hostHandler = TransportHostHandler(api_, protocol_, transport_.controller)},
          device_{.view = DeviceView(viewContainer_.getMainZone()),
                  .controller = DeviceController(device_.view),
                  .inputHandler = DeviceInputHandler(api_, device_.view, device_.controller, protocol_,
                                                     device_.view.getElement()),
                  .hostHandler = DeviceHostHandler(protocol_, api_, device_.view, device_.controller,
                                                   device_.inputHandler)},
          splash_{.view = SplashView(api_.getParentContainer())},
          lastClicked_{.handler = LastClickedHandler(api_, protocol_, api_.getParentContainer())},
          viewRegistry_({{ViewID::DEVICE, device_.view}, {ViewID::SPLASH, splash_.view}}),
          viewManager_(api, viewRegistry_, viewContainer_),
          lifecycle_{.handler = PluginLifecycleHandler(viewManager_, protocol_)} {}

    Plugin::~Plugin() = default;

    bool Plugin::initialize()
    {
        return true;
    }

} // namespace Bitwig
