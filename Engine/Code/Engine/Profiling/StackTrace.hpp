#pragma once

#include <atomic>
#include <shared_mutex>
#include <vector>

struct callstack_line_t;
class FileLogger;

class StackTrace final {
public:
    explicit StackTrace();
	StackTrace([[maybe_unused]]unsigned long framesToSkip,
               [[maybe_unused]]unsigned long framesToCapture);
    ~StackTrace();
    bool operator==(const StackTrace& rhs);
    bool operator!=(const StackTrace& rhs);
    bool operator<(const StackTrace& rhs);
protected:
private:

    static void Initialize();
    static void Shutdown();
    static std::atomic_uint64_t GetRefs();
    static void IncrementRefs();
    static void DecrementRefs();
    [[maybe_unused]] static uint32_t GetLines([[maybe_unused]]StackTrace* st,
                          [[maybe_unused]]callstack_line_t* lines,
                          [[maybe_unused]]unsigned long max_lines);
    unsigned long _hash = 0;
    unsigned long _frame_count = 0;
    static constexpr auto MAX_FRAMES_PER_CALLSTACK = 128ul;
    void* _frames[MAX_FRAMES_PER_CALLSTACK];
    static std::shared_mutex _cs;
    static std::atomic_uint64_t _refs;
    static std::atomic_bool _did_init;
};