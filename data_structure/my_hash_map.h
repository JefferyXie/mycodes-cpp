#ifndef MY_HASH_MAP_H
#define MY_HASH_MAP_H

#include "../main/header.h"
#include "../main/utility.h"

// TODO: initial version, need more test
template <typename T>
class my_hash_map
{
    struct node_t {
        T       object{};
        node_t* next{nullptr};
    };

public:
    explicit my_hash_map(size_t init_bucket_count = 16) : hasher_{std::hash<T>{}.operator()}
    {
        buckets_.resize(init_bucket_count, nullptr);
    }

    explicit my_hash_map(size_t init_bucket_count, std::function<size_t(const T&)>&& hasher)
        : hasher_{std::move(hasher)}
    {
        buckets_.resize(init_bucket_count, nullptr);
    }

    ~my_hash_map()
    {
        for (auto node : buckets_) {
            while (node) {
                auto temp = node;
                node      = node->next;
                delete temp;
            }
        }
        node_count_ = 0;
    }

    template <typename... Args>
    std::pair<bool, T*> emplace(Args&&... args)
    {
        T          obj{std::forward<Args>(args)...};
        const auto hash_code  = hasher_(obj);
        const auto bucket_idx = hash_code % buckets_.size();

        auto bucket_head = buckets_[bucket_idx];
        auto bucket_node = bucket_head;
        while (bucket_node) {
            if (obj == bucket_node->object) {
                return {false, &bucket_node->object};
            }
            bucket_node = bucket_node->next;
        }

        auto new_node = new node_t{
            .object = std::move(obj),
            .next   = bucket_head,
        };
        if (!new_node) {
            throw std::bad_alloc();
        }

        buckets_[bucket_idx] = new_node;

        ++node_count_;
        try_rehash();

        return {true, &new_node->object};
    }

    template <typename... Args>
    T* find(Args&&... args)
    {
        T          obj{std::forward<Args>(args)...};
        const auto hash_code  = hasher_(obj);
        const auto bucket_idx = hash_code % buckets_.size();

        auto bucket_head = buckets_[bucket_idx];
        auto bucket_node = bucket_head;
        while (bucket_node) {
            if (obj == bucket_node->object) {
                return &bucket_node->object;
            }
            bucket_node = bucket_node->next;
        }
        return nullptr;
    }

    T* erase(T* obj)
    {
        if (!obj)
            return nullptr;

        const auto hash_code  = hasher_(*obj);
        const auto bucket_idx = hash_code % buckets_.size();

        node_t* bucket_node_prev = nullptr;
        node_t* bucket_head      = buckets_[bucket_idx];
        node_t* bucket_node      = bucket_head;
        while (bucket_node) {
            if (obj == &bucket_node->object) {
                auto next = bucket_node->next;
                if (bucket_node_prev) {
                    bucket_node_prev->next = next;
                } else {
                    buckets_[bucket_idx] = next;
                }

                --node_count_;
                delete bucket_node;
                if (next) {
                    return &next->object;
                }
                return nullptr;
            }
            bucket_node_prev = bucket_node;
            bucket_node      = bucket_node->next;
        }
        return nullptr;
    }

    template <typename... Args>
    T* erase(Args&&... args)
    {
        T          obj{std::forward<Args>(args)...};
        const auto hash_code  = hasher_(obj);
        const auto bucket_idx = hash_code % buckets_.size();

        node_t* bucket_node_prev = nullptr;
        node_t* bucket_head      = buckets_[bucket_idx];
        node_t* bucket_node      = bucket_head;
        while (bucket_node) {
            if (obj == bucket_node->object) {
                auto next = bucket_node->next;
                if (bucket_node_prev) {
                    bucket_node_prev->next = next;
                } else {
                    buckets_[bucket_idx] = next;
                }

                --node_count_;
                delete bucket_node;
                if (next) {
                    return &next->object;
                }
                return nullptr;
            }
            bucket_node_prev = bucket_node;
            bucket_node      = bucket_node->next;
        }
        return nullptr;
    }

    T* begin()
    {
        // TODO:...
        return nullptr;
    }

    T* end()
    {
        // TODO:...
        return nullptr;
    }

private:
    void try_rehash()
    {
        if (node_count_ > 2 * buckets_.size()) {
            std::cout << __FUNCTION__ << ": node_count=" << node_count_ << ", # buckets=" << buckets_.size()
                      << std::endl;

            const auto           new_bucket_count = node_count_ * 2;
            std::vector<node_t*> new_buckets(new_bucket_count, nullptr);
            for (auto bucket_head : buckets_) {
                auto node = bucket_head;
                while (node) {
                    auto temp = node->next;

                    const auto hash_code  = hasher_(node->object);
                    const auto bucket_idx = hash_code % new_bucket_count;
                    if (new_buckets[bucket_idx]) {
                        node->next = new_buckets[bucket_idx];
                    }
                    new_buckets[bucket_idx] = node;

                    node = temp;
                }
            }

            std::swap(buckets_, new_buckets);
        }
    }

private:
    size_t                          node_count_{0};
    std::vector<node_t*>            buckets_;
    std::function<size_t(const T&)> hasher_;
};

void run_my_hash_map()
{
    cout << __FILE__ << ":" << __FUNCTION__ << endl;

    {
        my_hash_map<int> hm;
        hm.emplace(1);
        hm.emplace(2);
        hm.emplace(3);

        hm.erase(2);
        hm.erase(1);
        hm.erase(3);
    }

    {
        my_hash_map<int> hm{2};
        for (int i = -100; i < 100; ++i) {
            hm.emplace(i);
        }
    }

    {
        my_hash_map<int> hm{100, [](const int& v) {
                                return v;
                            }};
        for (int i = -100; i < 100; ++i) {
            hm.emplace(i);
        }
    }

    {
        my_hash_map<std::string> hm{4, [](const std::string& v) {
                                        return std::hash<std::string>()(v);
                                    }};
        for (int i = -100; i < 100; ++i) {
            hm.emplace(std::to_string(i));
        }
        auto obj = hm.find(0);
        while (obj) {
            obj = hm.erase(obj);
        }
    }
}

#endif
