#pragma once

#include <etl/map.h>
#include <initializer_list>

namespace UI { class IView; }

namespace Bitwig
{

    enum class ViewID : uint8_t
    {
        DEVICE = 0,
        TRANSPORT = 1,
        MIXER = 2,
        SPLASH = 255,
    };

    class ViewRegistry
    {
    public:
        struct ViewEntry
        {
            ViewID id;
            UI::IView &view;
        };

        ViewRegistry(std::initializer_list<ViewEntry> views)
        {
            for (const auto &entry : views)
            {
                views_[entry.id] = &entry.view;
            }
        }

        template <typename T = UI::IView>
        T &getView(ViewID id) const
        {
            return static_cast<T &>(*views_.find(id)->second);
        }

    private:
        etl::map<ViewID, UI::IView *, 8> views_;
    };

} // namespace Bitwig
