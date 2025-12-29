#include "OverlayController.hpp"

#include <oc/api/ButtonAPI.hpp>
#include <oc/log/Log.hpp>

namespace bitwig::state {

void OverlayController::hide() {
    auto current = manager_.current();
    if (current == OverlayType::NONE) return;

    // 1. Cleanup before hiding
    doCleanup(current);

    // 2. Hide via manager (handles stacking)
    manager_.hide();

    OC_LOG_DEBUG("[OverlayController] Hidden overlay {} with cleanup",
                 static_cast<int>(current));
}

void OverlayController::hideAll() {
    // Cleanup all overlays that might be visible
    for (size_t i = 1; i < static_cast<size_t>(OverlayType::COUNT); ++i) {
        doCleanup(static_cast<OverlayType>(i));
    }

    // Hide all via manager
    manager_.hideAll();

    OC_LOG_DEBUG("[OverlayController] Hidden all overlays with cleanup");
}

void OverlayController::cleanupFor(OverlayType type) {
    doCleanup(type);
}

void OverlayController::doCleanup(OverlayType type) {
    if (type == OverlayType::NONE || type == OverlayType::COUNT) return;

    auto idx = static_cast<size_t>(type);
    const auto& info = cleanup_[idx];

    // Clear latch first (before scope, so release handler can still fire)
    if (info.latchButton != 0 && buttons_) {
        buttons_->clearLatch(info.latchButton);
        OC_LOG_DEBUG("[OverlayController] Cleared latch for button {}",
                     static_cast<int>(info.latchButton));
    }

    // Clear scope (removes bindings, but we keep scoped bindings for now)
    // NOTE: We use clearLatch, not clearScope, because bindings should persist
    // Scope cleanup happens when overlay element is destroyed or bindings are re-registered
    // The key cleanup is the latch - bindings are gated by isActive predicates

    // Actually, let me reconsider: the handlers use .scope(overlayElement) which
    // makes bindings inactive when overlay is hidden via LV_OBJ_FLAG_HIDDEN check.
    // So we don't need to clearScope here - the isActive predicate handles it.
    // We DO need to clear latches though.
}

}  // namespace bitwig::state
