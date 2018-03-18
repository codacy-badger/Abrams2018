#include "Engine/RHI/RHITypes.hpp"

#include <type_traits>

RHIOutputMode& operator++(RHIOutputMode& mode) {
    using IntType = typename std::underlying_type<RHIOutputMode>::type;
    mode = static_cast<RHIOutputMode>(static_cast<IntType>(mode) + 1);
    if(mode == RHIOutputMode::LAST_) {
        mode = RHIOutputMode::FIRST_;
    }
    return mode;
}

RHIOutputMode operator++(RHIOutputMode& mode, int) {
    RHIOutputMode result = mode;
    ++result;
    return result;
}
