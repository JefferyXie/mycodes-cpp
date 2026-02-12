#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <unordered_set>

namespace __event_queue {

class EventQueue
{
private:
    struct Event {
        using TriggerTime = std::chrono::steady_clock::time_point;
        using Callback    = std::function<void()>;
        TriggerTime triggerTime{};
        Callback    callback;

        bool operator>(const Event& other) { return triggerTime > other.triggerTime; }
    };

    bool                    stopped_{};
    std::mutex              mu_;
    std::condition_variable cv_;

    // TODO:
    // - real time high priority event
    // - real time low priority event
    // - delayed event

    // - pre-allocated Event pool
    // - lock free queue than mutex
    std::priority_queue<Event, std::vector<Event>, std::greater<>> events_;

public:
    explicit EventQueue() = default;
    ~EventQueue()
    {
        // ...
    }

    void Run()
    {
        while (true) {
            std::unique_lock<std::mutex> lk{mu_};
            if (events_.empty()) {
                cv_.wait(lk, [&]() {
                    return !events_.empty() || stopped_;
                });
            }
            if (stopped_) {
                break;
            }

            if (events_.top().triggerTime > std::chrono::steady_clock::now()) {
                continue;
            }

            // TODO: extract multiple or all events than one by one
            auto ev = std::move(events_.top());
            events_.pop();
            lk.unlock();

            ev.callback();
        }
    }
    void Stop()
    {
        std::unique_lock<std::mutex> lk{mu_};
        stopped_ = true;
        cv_.notify_all();
    }

    void Add(Event::Callback callback)
    {
        static constexpr auto kEpochTimepoint = std::chrono::steady_clock::time_point{};

        Add(kEpochTimepoint, std::move(callback));
    }

    // TODO: in place creation of Event::Callback than created in client thread stack
    void Add(Event::TriggerTime triggerTime, Event::Callback callback)
    {
        std::lock_guard<std::mutex> lk{mu_};
        events_.push(Event{
            .triggerTime = triggerTime,
            .callback    = callback,
        });
        cv_.notify_all();
    }
};

void run_event_queue()
{
    EventQueue evtQ;

    // may have multiple workers
    std::thread eventWorkerThread1{[&]() {
        evtQ.Run();
    }};

    std::thread eventWorkerThread2{[&]() {
        evtQ.Run();
    }};

    auto now = std::chrono::steady_clock::now();
    for (int i = 0; i < 10; ++i) {
        evtQ.Add(now + std::chrono::seconds{2 + i}, [i /* capture any variable you want */]() {
            // your code here, keep in mind this is running in EventQueue thread!
            // ...
            std::cout << std::this_thread::get_id() << ": client event handler, in " << 2 + i << " seconds..."
                      << std::endl;
        });

        evtQ.Add([/* capture any variable you want */]() {
            // your code here, keep in mind this is running in EventQueue thread!
            // ...
            std::cout << std::this_thread::get_id() << ": client event handler, run now..." << std::endl;
        });
    }

    std::this_thread::sleep_for(std::chrono::seconds(6));

    evtQ.Stop();

    eventWorkerThread1.join();
    eventWorkerThread2.join();
}

}    // namespace __event_queue

namespace __bandwidth {

/*
            dist switch 1    <----------------->    dist switch 2
          /               \                        /
       switch1         switch2                 switch3     ...
      /       \       /       \               /       \
    host1   host2   host3   host4           host5   host6  ...
*/

struct Switch;
struct DistSwitch;

struct Host {
    uint32_t    bandwidth{};
    std::string name;    // unique
    Switch*     localSwitch{};
    DistSwitch* distSwitch{};
};

struct Switch {
    uint32_t           bandwidth{};
    std::string        name;    // unique
    DistSwitch*        distSwitch{};
    std::vector<Host*> hosts;
};

struct DistSwitch {
    uint32_t                 bandwidth{};
    std::string              name;    // unique
    std::vector<Switch*>     localSwitches;
    std::vector<DistSwitch*> neighbors;
};

uint32_t get_max_bandwidth(Host* host1, Host* host2)
{
    if (!host1 || !host2 || host1 == host2) {
        return 0;
    }

    uint32_t result =
        std::min({host1->bandwidth, host2->bandwidth, host1->localSwitch->bandwidth, host2->localSwitch->bandwidth});
    if (host1->localSwitch == host2->localSwitch) {
        return result;
    }

    if (host1->distSwitch == host2->distSwitch) {
        result = std::min(result, host1->distSwitch->bandwidth);
        return result;
    }

    const auto                           targetNode = host2->distSwitch;
    std::unordered_set<DistSwitch*>      visited;
    std::function<uint32_t(DistSwitch*)> impl = [&](DistSwitch* node) {
        if (node == targetNode) {
            return node->bandwidth;
        }

        visited.emplace(node);

        uint32_t bandwidth = node->bandwidth;
        for (auto neighbor : node->neighbors) {
            if (visited.count(neighbor) == 0) {
                bandwidth = std::min(bandwidth, impl(neighbor));
            }
        }

        visited.erase(node);

        return bandwidth;
    };

    return std::min(result, impl(host1->distSwitch));
}

}    // namespace __bandwidth

