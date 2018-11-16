#pragma once

#include <chrono>
#include <string>

namespace TimeUtils {

using FPSeconds = std::chrono::duration<float>;
using FPMilliseconds = std::chrono::duration<float, std::milli>;
using FPMicroseconds = std::chrono::duration<float, std::micro>;
using FPNanoseconds = std::chrono::duration<float, std::nano>;
using FPFrames = std::chrono::duration<float, std::ratio<1, 60>>;
using Frames = std::chrono::duration<uint64_t, std::ratio<1, 60>>;

double GetCurrentTimeSeconds();

template<typename Clock = std::chrono::steady_clock>
decltype(auto) Now() noexcept {
    return Clock::now();
}

//Get the elapsed time between calls (defaults to float-precision seconds)
template<typename Clock = std::chrono::steady_clock>
decltype(auto) GetCurrentTimeElapsed() noexcept {
    using namespace std::chrono;
    static auto initial_now = Now<Clock>();
    auto now = Now<Clock>();
    return (now - initial_now);
}

struct DateTimeStampOptions {
    bool use_separator = false;
    bool use_24_hour_clock = true;
    bool include_milliseconds = true;
};

std::string GetDateTimeStampFromNow(const DateTimeStampOptions& options = DateTimeStampOptions{});
std::string GetTimeStampFromNow(const DateTimeStampOptions& options = DateTimeStampOptions{});
std::string GetDateStampFromNow(const DateTimeStampOptions& options = DateTimeStampOptions{});

} //End TimeUtils