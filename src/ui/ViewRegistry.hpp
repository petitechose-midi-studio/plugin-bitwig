#pragma once

#include <cstdint>

class IView;

namespace Bitwig {

// ============================================================================
// SINGLE SOURCE OF TRUTH: add new views here
// ============================================================================
enum class ViewID : uint8_t {
    DEVICE,
    SPLASH,
    // Add new views above ↑
    COUNT  // Must remain last
};

// ============================================================================

class ViewRegistry {
public:
    void add(ViewID id, IView& view) {
        views_[static_cast<uint8_t>(id)] = &view;
    }

    template <typename T = IView>
    T& get(ViewID id) const {
        return static_cast<T&>(*views_[static_cast<uint8_t>(id)]);
    }

private:
    IView* views_[static_cast<uint8_t>(ViewID::COUNT)] = {nullptr};
};

}  // namespace Bitwig
