#pragma once

#include <oc/api/ButtonAPI.hpp>
#include <oc/api/EncoderAPI.hpp>

namespace bitwig::handler {

// Bitwig-owned convenience view over the two input APIs used together by
// handler composition. It owns no state and performs no runtime allocation.
struct InputAPI {
    oc::api::EncoderAPI& encoders;
    oc::api::ButtonAPI& buttons;
};

}  // namespace bitwig::handler
