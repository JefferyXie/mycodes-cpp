#include <atomic>

// https://www.youtube.com/watch?v=kPh8pod0-gk
// 0) sometimes this is called 'sticky counter' - it gets stuck at zero
// 1) required by std::weak_ptr<T>::lock;
// 2) also useful for atomic memory management / concurrent data structures, i.e. atomic<shared_ptr>
class counter_t
{
private:
    static constexpr uint64_t IS_ZERO   = 1ull << 63;
    static constexpr uint64_t BE_HELPED = 1ull << 62;
    // precondition: counter is not zero
    std::atomic<uint64_t> counter_ = 1;

public:
    // increase counter and return true if counter is greater than zero, otherwise do nothing and return false
    bool increment_if_not_zero() { return (counter_.fetch_add(1) & IS_ZERO) == 0; }

    // always decrease counter, return true if counter after decrease is zero, otherwise return false
    bool decrement()
    {
        if (counter_.fetch_sub(1) == 1) {
            uint64_t e = 0;
            if (counter_.compare_exchange_strong(e, IS_ZERO)) {
                return true;
            }
            if ((e & BE_HELPED) && (counter_.exchange(IS_ZERO) & BE_HELPED)) {
                return true;
            }
        }
        return false;
    }

    // return current value of counter
    uint64_t read()
    {
        auto val = counter_.load();
        if (val == 0 && counter_.compare_exchange_strong(val, IS_ZERO | BE_HELPED)) {
            return 0;    // in helping
        }
        return (val & IS_ZERO) ? 0 : val;
    }
};

