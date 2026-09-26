# Canonical tracked source inventory for the Bitwig firmware and SDL consumers.
set(MS_PLUGIN_BITWIG_SOURCE_PATHS
    src/context/BitwigContext.cpp
    src/handler/host/DeviceHostHandler.cpp
    src/handler/host/LastClickedHostHandler.cpp
    src/handler/host/MidiHostHandler.cpp
    src/handler/host/PageHostHandler.cpp
    src/handler/host/PluginHostHandler.cpp
    src/handler/host/RemoteControlHostHandler.cpp
    src/handler/host/TrackHostHandler.cpp
    src/handler/host/TransportHostHandler.cpp
    src/handler/input/DevicePageInputHandler.cpp
    src/handler/input/DeviceSelectorInputHandler.cpp
    src/handler/input/LastClickedInputHandler.cpp
    src/handler/input/RemoteControlInputHandler.cpp
    src/handler/input/TrackInputHandler.cpp
    src/handler/input/TransportInputHandler.cpp
    src/handler/input/ViewStateInputHandler.cpp
    src/handler/input/ViewSwitcherInputHandler.cpp
    src/main.cpp
    src/name.c
    src/ui/device/DeviceSelector.cpp
    src/ui/device/DeviceStateBar.cpp
    src/ui/device/DeviceTitleItem.cpp
    src/ui/device/LevelBar.cpp
    src/ui/font/BitwigFonts.cpp
    src/ui/remotecontrols/RemoteControlsPageSelector.cpp
    src/ui/remotecontrols/RemoteControlsView.cpp
    src/ui/resource/img/Bitwig_Logo.c
    src/ui/splash/SplashView.cpp
    src/ui/track/TrackSelector.cpp
    src/ui/track/TrackTitleItem.cpp
    src/ui/transportbar/TransportBar.cpp
    src/ui/view/ViewSelector.cpp
    src/ui/widget/BackButton.cpp
    src/ui/widget/BaseParameterWidget.cpp
    src/ui/widget/BaseSelector.cpp
    src/ui/widget/HintBar.cpp
    src/ui/widget/ParameterButtonWidget.cpp
    src/ui/widget/ParameterKnobWidget.cpp
    src/ui/widget/ParameterListWidget.cpp
    src/ui/widget/TitleItem.cpp
)

set(MS_PLUGIN_BITWIG_SOURCES ${MS_PLUGIN_BITWIG_SOURCE_PATHS})
list(TRANSFORM MS_PLUGIN_BITWIG_SOURCES
    PREPEND "${CMAKE_CURRENT_LIST_DIR}/../")
