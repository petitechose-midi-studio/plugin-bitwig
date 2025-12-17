#include "BitwigContext.hpp"

#include <oc/log/Log.hpp>
#include <ui/font/FontLoader.hpp>  // Core's font system

#include "ui/font/BitwigFonts.hpp"

namespace bitwig {

// =============================================================================
// Static Resource Loading (called by ContextManager during registration)
// =============================================================================

void BitwigContext::loadResources() {
    OC_LOG_INFO("BitwigContext::loadResources()");
    bitwigFontsRegister();  // Queue icon fonts for loading
}

// Destructor must be in .cpp where handler types are complete
BitwigContext::~BitwigContext() = default;

// =============================================================================
// IContext Lifecycle
// =============================================================================

bool BitwigContext::initialize() {
    OC_LOG_INFO("BitwigContext initializing...");

    // Load fonts: core text fonts + plugin icon fonts
    fontsRegisterCore();
    fontsLoadEssential();
    loadPluginFonts();
    bitwigFontsLink();

    createProtocol();
    createHostHandlers();
    createViews();
    createInputHandlers();

    OC_LOG_INFO("BitwigContext initialized");
    return true;
}

void BitwigContext::update() {
    // Handlers and views are self-updating via Signal subscriptions
    // Only need to check host timeout here if implemented
}

void BitwigContext::cleanup() {
    OC_LOG_INFO("BitwigContext cleanup");

    // Destroy in reverse order of creation

    // Input Handlers (they hold bindings that may reference state)
    inputLastClicked_.reset();
    inputTrack_.reset();
    inputDeviceSelector_.reset();
    inputDevicePage_.reset();
    inputRemoteControl_.reset();
    inputViewSwitcher_.reset();
    inputTransport_.reset();

    // Global overlays (subscriptions first, then widget)
    viewSelectorSubs_.clear();
    viewSelector_.reset();

    // Views (reset ViewManager first to deactivate, then destroy)
    state_.views.reset();
    transportBar_.reset();
    clipView_.reset();
    mixView_.reset();
    remoteControlsView_.reset();
    viewContainer_.reset();

    // Host Handlers
    hostMidi_.reset();
    hostLastClicked_.reset();
    hostRemoteControl_.reset();
    hostPage_.reset();
    hostTrack_.reset();
    hostDevice_.reset();
    hostTransport_.reset();
    hostPlugin_.reset();

    // Protocol
    protocol_.reset();

    // State
    state_.resetAll();
}

void BitwigContext::onConnected() {
    OC_LOG_INFO("BitwigContext activated");

    // Request current state from Bitwig
    protocol_->requestHostStatus();
    protocol_->requestDeviceList();
    protocol_->requestTrackList();
}

void BitwigContext::onDisconnected() {
    OC_LOG_INFO("BitwigContext deactivated");
    state_.resetAll();
}

// =============================================================================
// Factory Methods
// =============================================================================

void BitwigContext::createProtocol() {
    protocol_ = std::make_unique<BitwigProtocol>(midi(), events());
}

void BitwigContext::createHostHandlers() {
    // Order matters: plugin handler requests host status first
    hostPlugin_ = std::make_unique<handler::HandlerHostPlugin>(state_, *protocol_);
    hostTransport_ = std::make_unique<handler::HandlerHostTransport>(state_, *protocol_);
    hostDevice_ = std::make_unique<handler::HandlerHostDevice>(state_, *protocol_);
    hostTrack_ = std::make_unique<handler::HandlerHostTrack>(state_, *protocol_);
    hostPage_ = std::make_unique<handler::HandlerHostPage>(state_, *protocol_, encoders());
    hostRemoteControl_ = std::make_unique<handler::HandlerHostRemoteControl>(state_, *protocol_, encoders());
    hostLastClicked_ = std::make_unique<handler::HandlerHostLastClicked>(state_, *protocol_, encoders());
    hostMidi_ = std::make_unique<handler::HandlerHostMidi>(state_, midi());
}

void BitwigContext::createInputHandlers() {
    // Scope hierarchy: overlay > view > global
    lv_obj_t* mainZone = viewContainer_->getMainZone();
    lv_obj_t* scopeElement = remoteControlsView_ ? remoteControlsView_->getElement() : lv_screen_active();
    lv_obj_t* deviceSelectorOverlay = remoteControlsView_ ? remoteControlsView_->getDeviceSelectorElement() : nullptr;
    lv_obj_t* trackSelectorOverlay = remoteControlsView_ ? remoteControlsView_->getTrackSelectorElement() : nullptr;
    lv_obj_t* pageSelectorOverlay = remoteControlsView_ ? remoteControlsView_->getPageSelectorElement() : nullptr;
    lv_obj_t* viewSelectorOverlay = viewSelector_ ? viewSelector_->getElement() : nullptr;

    // Global scope (lowest priority)
    inputTransport_ = std::make_unique<handler::HandlerInputTransport>(
        state_, *protocol_, encoders(), buttons());

    // ViewSwitcher: scope = mainZone (parent of all views), overlay = ViewSelector
    inputViewSwitcher_ = std::make_unique<handler::HandlerInputViewSwitcher>(
        state_, encoders(), buttons(), mainZone, viewSelectorOverlay);

    // View + overlay scopes
    inputDeviceSelector_ = std::make_unique<handler::HandlerInputDeviceSelector>(
        state_, *protocol_, encoders(), buttons(), scopeElement, deviceSelectorOverlay);
    inputRemoteControl_ = std::make_unique<handler::HandlerInputRemoteControl>(
        state_, *protocol_, encoders(), buttons(), scopeElement);
    inputTrack_ = std::make_unique<handler::HandlerInputTrack>(
        state_, *protocol_, encoders(), buttons(), trackSelectorOverlay);
    inputDevicePage_ = std::make_unique<handler::HandlerInputDevicePage>(
        state_, *protocol_, encoders(), buttons(), scopeElement, pageSelectorOverlay);

    // LastClicked: OPT encoder for adjusting last clicked parameter
    inputLastClicked_ = std::make_unique<handler::HandlerInputLastClicked>(
        state_, *protocol_, encoders(), scopeElement);
}

void BitwigContext::createViews() {
    using state::ViewType;

    viewContainer_ = std::make_unique<ViewContainer>(lv_screen_active());
    lv_obj_t* mainZone = viewContainer_->getMainZone();

    // Create all views (they start hidden)
    remoteControlsView_ = std::make_unique<RemoteControlsView>(mainZone, state_);
    mixView_ = std::make_unique<MixView>(mainZone);
    clipView_ = std::make_unique<ClipView>(mainZone);

    // Register views with ViewManager
    state_.views.registerView(ViewType::REMOTE_CONTROLS, remoteControlsView_.get());
    state_.views.registerView(ViewType::MIX, mixView_.get());
    state_.views.registerView(ViewType::CLIP, clipView_.get());

    // Initialize ViewManager (activates first registered view)
    state_.views.initialize();

    // Persistent UI (always visible)
    transportBar_ = std::make_unique<TransportBar>(viewContainer_->getBottomZone(), state_.transport);

    // Global overlay: ViewSelector (parent = mainZone so it covers views but not TransportBar)
    viewSelector_ = std::make_unique<ViewSelector>(mainZone);

    // Setup bindings for ViewSelector rendering
    auto renderViewSelector = [this]() {
        static const std::vector<std::string> VIEW_NAMES = {"Remote Controls", "Mix", "Clip"};
        viewSelector_->render({
            VIEW_NAMES,
            state_.viewSelector.selectedIndex.get(),
            state_.viewSelector.visible.get()
        });
    };

    viewSelectorSubs_.push_back(state_.viewSelector.visible.subscribe(
        [renderViewSelector](bool) { renderViewSelector(); }));
    viewSelectorSubs_.push_back(state_.viewSelector.selectedIndex.subscribe(
        [renderViewSelector](int) { renderViewSelector(); }));

    lv_obj_clear_flag(viewContainer_->getContainer(), LV_OBJ_FLAG_HIDDEN);
}

}  // namespace bitwig
