#pragma once

#include <cstdint>

#include <map>

#include <initializer_list>

class IView;

namespace Bitwig {

enum class ViewID : uint8_t {
    DEVICE = 0,
    TRANSPORT = 1,
    MIXER = 2,
    SPLASH = 255,
};

class ViewRegistry {
public:
    struct ViewEntry {
        ViewID id;
        IView &view;
    };

    ViewRegistry(std::initializer_list<ViewEntry> views) {
        for (const auto &entry : views) { views_[entry.id] = &entry.view; }
    }

    template <typename T = IView>
    T &getView(ViewID id) const {
        auto it = views_.find(id);
        if (it == views_.end() || !it->second) {
            // This should never happen in normal operation.
            // If it does, we have a programming error.
            while (true) {}  // Halt in debug
        }
        return static_cast<T &>(*it->second);
    }
private:
    std::map<ViewID, IView *> views_;
};

}  // namespace Bitwig
