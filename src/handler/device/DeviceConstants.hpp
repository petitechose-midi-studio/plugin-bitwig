#pragma once

#include <cstdint>
#include "../../ui/LVGLSymbol.hpp"

namespace Bitwig::Device
{

    // ============================================================================
    // PARAMETER CONSTANTS
    // ============================================================================

    constexpr uint8_t PARAMETER_COUNT = 8;

    // ============================================================================
    // DEVICE NAVIGATION CONSTANTS
    // ============================================================================

    constexpr uint8_t MAX_DEVICES = 16;
    constexpr uint8_t MAX_CHILD_TYPES = 4;

    // ============================================================================
    // CHILD TYPE DEFINITIONS
    // ============================================================================

    enum ChildType : uint8_t
    {
        None = 0,
        Slots = 1,
        Layers = 2,
        Drums = 4
    };

    inline const char *getChildTypeName(uint8_t type)
    {
        switch (type)
        {
        case Slots:
            return "[S] Slots";
        case Layers:
            return "[L] Layers";
        case Drums:
            return "[D] Drum Pads";
        default:
            return "";
        }
    }

    // Converts array of child types to bitflags (Slots|Layers|Drums)
    template <typename ChildTypeArray>
    inline uint8_t getChildTypeFlags(const ChildTypeArray &types)
    {
        uint8_t flags = 0;
        for (uint8_t type : types)
        {
            if (type == None)
                break;
            flags |= type;
        }
        return flags;
    }

    // ============================================================================
    // PARAMETER TYPE DEFINITIONS
    // ============================================================================

    enum ParameterType : uint8_t
    {
        Knob = 0,   // Continuous parameter (optimistic updates)
        Button = 1, // On/Off parameter (wait for echo with displayValue)
        List = 2    // Discrete selection (wait for echo with displayValue)
    };

    // ============================================================================
    // UI TEXT CONSTANTS
    // ============================================================================

    constexpr const char *BACK_TO_PARENT_TEXT = LVGLSymbol::BACK;

} // namespace Bitwig::Device
