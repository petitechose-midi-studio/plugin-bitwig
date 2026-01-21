#include "BitwigContext.hpp"

#include <api/InputAPI.hpp>
#include <oc/log/Log.hpp>
#include <ui/OverlayBindingContext.hpp>
#include <oc/ui/lvgl/FontLoader.hpp>
#include <oc/ui/lvgl/Screen.hpp>
#include <ui/font/CoreFonts.hpp>

#include <config/App.hpp>
#include "protocol/MessageStructure.hpp"
#include "ui/font/BitwigFonts.hpp"

namespace bitwig {

// =============================================================================
// Static Resource Loading (called by ContextManager during registration)
// =============================================================================

void BitwigContext::loadResources() {
    OC_LOG_INFO("BitwigContext::loadResources()");
    // Fonts are now loaded in initialize() via FontLoader API
}

// Destructor must be in .cpp where handler types are complete
BitwigContext::~BitwigContext() = default;

// =============================================================================
// IContext Lifecycle
// =============================================================================

oc::Result<void> BitwigContext::init() {
    OC_LOG_INFO("BitwigContext initializing...");

    // Load fonts: core text fonts + plugin icon fonts
    oc::ui::lvgl::font::load(CORE_FONT_ENTRIES, CORE_FONT_COUNT);
    oc::ui::lvgl::font::load(BITWIG_FONT_ENTRIES, BITWIG_FONT_COUNT);
    linkCoreFontAliases();
    linkBitwigFontAliases();

    createProtocol();
    createHostHandlers();
    createViews();
    createOverlayManager();
    createInputHandlers();

    OC_LOG_INFO("BitwigContext initialized");
    return oc::Result<void>::ok();
}

void BitwigContext::update() {
    // Handlers and views are self-updating via Signal subscriptions
    // Only need to check host timeout here if implemented
}

void BitwigContext::cleanup() {
    OC_LOG_INFO("BitwigContext cleanup");

    // Destroy in reverse order of creation

    // Input Handlers (they hold bindings that may reference state)
    input_view_state_.reset();
    input_last_clicked_.reset();
    input_track_.reset();
    input_device_selector_.reset();
    input_device_page_.reset();
    input_remote_control_.reset();
    input_view_switcher_.reset();
    input_transport_.reset();

    // Global overlays (subscriptions first, then widget)
    view_selector_subs_.clear();
    view_selector_.reset();

    // Overlay controller
    overlay_controller_.reset();

    // Views (reset ViewManager first to deactivate, then destroy)
    state_.views.reset();
    transport_bar_.reset();
    clip_view_.reset();
    mix_view_.reset();
    remote_controls_view_.reset();
    view_container_.reset();

    // Host Handlers
    host_midi_.reset();
    host_last_clicked_.reset();
    host_remote_control_.reset();
    host_page_.reset();
    host_track_.reset();
    host_device_.reset();
    host_transport_.reset();
    host_plugin_.reset();

    // Protocol
    protocol_.reset();

    // State
    state_.resetAll();
}

void BitwigContext::onConnected() {
    OC_LOG_INFO("BitwigContext activated");

    // Request current state from Bitwig
    protocol_->requestHostStatus();
    protocol_->requestDeviceListWindow(0);
    protocol_->requestTrackListWindow(0);
}

void BitwigContext::onDisconnected() {
    OC_LOG_INFO("BitwigContext deactivated");
    state_.resetAll();
}

// =============================================================================
// Factory Methods
// =============================================================================

void BitwigContext::createProtocol() {
    protocol_ = std::make_unique<BitwigProtocol>(frames());
}

void BitwigContext::createHostHandlers() {
    // Order matters: plugin handler requests host status first
    host_plugin_ = std::make_unique<handler::PluginHostHandler>(state_, *protocol_);
    host_transport_ = std::make_unique<handler::TransportHostHandler>(state_, *protocol_);
    host_device_ = std::make_unique<handler::DeviceHostHandler>(state_, *protocol_);
    host_track_ = std::make_unique<handler::TrackHostHandler>(state_, *protocol_);
    host_page_ = std::make_unique<handler::PageHostHandler>(state_, *protocol_, encoders());
    host_remote_control_ = std::make_unique<handler::RemoteControlHostHandler>(state_, *protocol_, encoders());
    host_last_clicked_ = std::make_unique<handler::LastClickedHostHandler>(state_, *protocol_, encoders());
    host_midi_ = std::make_unique<handler::MidiHostHandler>(state_, midi());
}

void BitwigContext::createOverlayManager() {
    using ui::OverlayType;
    using ButtonID = Config::ButtonID;

    // Create manager wrapping the state's ExclusiveVisibilityStack
    overlay_controller_ = std::make_unique<core::state::OverlayManager<bitwig::ui::OverlayType>>(state_.overlays, buttons());

    // Get scope IDs from overlay elements
    lv_obj_t* deviceSelectorOverlay = remote_controls_view_ ? remote_controls_view_->getDeviceSelectorElement() : nullptr;
    lv_obj_t* trackSelectorOverlay = remote_controls_view_ ? remote_controls_view_->getTrackSelectorElement() : nullptr;
    lv_obj_t* pageSelectorOverlay = remote_controls_view_ ? remote_controls_view_->getPageSelectorElement() : nullptr;
    lv_obj_t* viewSelectorOverlay = view_selector_ ? view_selector_->getElement() : nullptr;

    // Register cleanup info for each overlay
    // Note: static_cast needed to convert Config::ButtonID enum to oc::ButtonID
    if (pageSelectorOverlay) {
        overlay_controller_->registerCleanup(
            OverlayType::PAGE_SELECTOR,
            reinterpret_cast<oc::ScopeID>(pageSelectorOverlay),
            static_cast<oc::ButtonID>(ButtonID::LEFT_BOTTOM)
        );
    }

    if (deviceSelectorOverlay) {
        overlay_controller_->registerCleanup(
            OverlayType::DEVICE_SELECTOR,
            reinterpret_cast<oc::ScopeID>(deviceSelectorOverlay),
            static_cast<oc::ButtonID>(ButtonID::LEFT_CENTER)
        );
    }

    if (trackSelectorOverlay) {
        overlay_controller_->registerCleanup(
            OverlayType::TRACK_SELECTOR,
            reinterpret_cast<oc::ScopeID>(trackSelectorOverlay),
            static_cast<oc::ButtonID>(ButtonID::BOTTOM_LEFT)
        );
    }

    if (viewSelectorOverlay) {
        overlay_controller_->registerCleanup(
            OverlayType::VIEW_SELECTOR,
            reinterpret_cast<oc::ScopeID>(viewSelectorOverlay),
            static_cast<oc::ButtonID>(ButtonID::LEFT_TOP)
        );
    }

    // Connect authority resolver to InputBinding for automatic scope filtering
    buttons().setAuthorityResolver(&overlay_controller_->authority());

    OC_LOG_INFO("OverlayManager created with {} overlays registered", 4);
}

void BitwigContext::createInputHandlers() {
    using OverlayCtx = core::ui::OverlayBindingContext<bitwig::ui::OverlayType>;

    // Scope hierarchy: overlay > view > global
    lv_obj_t* mainZone = view_container_->getMainZone();
    lv_obj_t* scopeElement = remote_controls_view_ ? remote_controls_view_->getElement() : lv_screen_active();
    lv_obj_t* deviceSelectorOverlay = remote_controls_view_ ? remote_controls_view_->getDeviceSelectorElement() : nullptr;
    lv_obj_t* trackSelectorOverlay = remote_controls_view_ ? remote_controls_view_->getTrackSelectorElement() : nullptr;
    lv_obj_t* pageSelectorOverlay = remote_controls_view_ ? remote_controls_view_->getPageSelectorElement() : nullptr;
    lv_obj_t* viewSelectorOverlay = view_selector_ ? view_selector_->getElement() : nullptr;

    // Create InputAPI facade for handlers that use both encoders and buttons
    core::api::InputAPI input{encoders(), buttons()};

    // Global scope (lowest priority)
    input_transport_ = std::make_unique<handler::TransportInputHandler>(
        state_, *protocol_, input);

    // ViewSwitcher: scope = mainZone (parent of all views), overlay = ViewSelector
    OverlayCtx viewSwitcherCtx{*overlay_controller_, mainZone, viewSelectorOverlay};
    input_view_switcher_ = std::make_unique<handler::ViewSwitcherInputHandler>(
        state_, viewSwitcherCtx, input);

    // View + overlay scopes
    OverlayCtx deviceSelectorCtx{*overlay_controller_, scopeElement, deviceSelectorOverlay};
    input_device_selector_ = std::make_unique<handler::DeviceSelectorInputHandler>(
        state_, deviceSelectorCtx, *protocol_, input);

    input_remote_control_ = std::make_unique<handler::RemoteControlInputHandler>(
        state_, *protocol_, input, scopeElement);

    // Track selector: no scopeElement, only overlayElement
    OverlayCtx trackSelectorCtx{*overlay_controller_, nullptr, trackSelectorOverlay};
    input_track_ = std::make_unique<handler::TrackInputHandler>(
        state_, trackSelectorCtx, *protocol_, input);

    OverlayCtx pageSelectorCtx{*overlay_controller_, scopeElement, pageSelectorOverlay};
    input_device_page_ = std::make_unique<handler::DevicePageInputHandler>(
        state_, pageSelectorCtx, *protocol_, input);

    // LastClicked: OPT encoder for adjusting last clicked parameter
    input_last_clicked_ = std::make_unique<handler::LastClickedInputHandler>(
        state_, *protocol_, encoders(), scopeElement);

    // ViewState: notifies host about active view and selector state
    input_view_state_ = std::make_unique<handler::ViewStateInputHandler>(
        state_, *protocol_);
}

void BitwigContext::createViews() {
    // ViewType is in global scope from protocol/ViewType.hpp
    view_container_ = std::make_unique<core::ui::ViewContainer>(oc::ui::lvgl::Screen::root());
    lv_obj_t* mainZone = view_container_->getMainZone();

    // Create all views (they start hidden)
    remote_controls_view_ = std::make_unique<ui::RemoteControlsView>(mainZone, state_);
    mix_view_ = std::make_unique<ui::MixView>(mainZone);
    clip_view_ = std::make_unique<ui::ClipView>(mainZone);

    // Register views with ViewManager
    state_.views.registerView(ViewType::REMOTE_CONTROLS, remote_controls_view_.get());
    state_.views.registerView(ViewType::MIX, mix_view_.get());
    state_.views.registerView(ViewType::CLIP, clip_view_.get());

    // Initialize ViewManager (activates first registered view)
    state_.views.initialize();

    // Persistent UI (always visible)
    transport_bar_ = std::make_unique<ui::TransportBar>(view_container_->getBottomZone(), state_.transport);

    // Global overlay: ViewSelector (parent = mainZone so it covers views but not TransportBar)
    view_selector_ = std::make_unique<ui::ViewSelector>(mainZone);

    // Setup bindings for ViewSelector rendering
    auto renderViewSelector = [this]() {
        static const std::vector<std::string> VIEW_NAMES = {"Remote Controls", "Mix", "Clip"};
        view_selector_->render({
            VIEW_NAMES,
            state_.viewSelector.selectedIndex.get(),
            state_.viewSelector.visible.get()
        });
    };

    view_selector_subs_.push_back(state_.viewSelector.visible.subscribe(
        [renderViewSelector](bool) { renderViewSelector(); }));
    view_selector_subs_.push_back(state_.viewSelector.selectedIndex.subscribe(
        [renderViewSelector](int) { renderViewSelector(); }));
}

}  // namespace bitwig
