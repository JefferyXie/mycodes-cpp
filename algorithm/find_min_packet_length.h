#pragma once

#include "../core/header.h"
#include "../core/util.h"

//
// 1) A market data publisher must publish all messages in a queue within time limit milliseconds.
// 2) You are given an integer array "A" of length N where A[i] represents the size in bytes of the ith message and
//    integer Y that represents the time Limit milliseconds.
// 3) Messages are batched in a packet buffer and sent every millisecond to improve throughput. The total size in bytes
//    of the messages must not exceed the size of the packet buffer.
// 4) And messages must be sent in the order they have been queued. If a message cannot fit in the packet buffer, it
//    will have to wait for the messages already in the buffer to get sent. Once the messages are sent, the packet
//    buffer will be cleared and new messages can get queued.
// 5) Find the minimum packet buffer size that will allow for all messages in the queue to be published within timeLimit
//    milliseconds.
//
// Write a function:
// int solution (vector<int> &A, int Y);
// that, given a zero-indexed array A consisting of N message sizes and an integer Y that represents the timeLimit,
// returns the minimum packet buffer size need.
//

// This problem is similar as below one but a little bit more complicated because of cumulative length
// https://www.youtube.com/watch?v=eRBpfoWujQM&ab_channel=HuaHua
//
uint32_t find_min_packet_length(const std::vector<uint32_t>& messages, uint32_t max_packets_count)
{
    const auto num_messages = messages.size();
    if (num_messages == 0 || max_packets_count == 0) {
        return 0;
    }

    if (max_packets_count >= num_messages) {
        auto find_max = [](const auto& arr) {
            auto value = arr.front();
            for (auto& v : arr) {
                if (v > value) {
                    value = v;
                }
            }
            return value;
        };
        return find_max(messages);
    }

    constexpr auto                     max_packet_length = std::numeric_limits<uint32_t>::max();
    std::vector<std::vector<uint32_t>> dp(num_messages, std::vector<uint32_t>(max_packets_count, max_packet_length));

    for (size_t msg_i = 0; msg_i < num_messages; ++msg_i) {
        auto total_msg_length_i = messages[msg_i];
        if (msg_i > 0) {
            total_msg_length_i += dp[msg_i - 1][0];
        }

        // if packet count is 1 (idx is 0), the min packet size should be length of all 0-i msgs for sure
        dp[msg_i][0] = total_msg_length_i;

        // max packet count = packet_idx + 1; let's start with packet_idx = 1, meaning pack count = 2, since target min
        // packet length must be dp[msg_i][0] which has been solved above if packet count = 1
        for (uint32_t packet_idx = 1; packet_idx < max_packets_count; ++packet_idx) {

            // NOTICE: when packet count changes (increased by one), all the messages (up to msg_i) may be re-grouped,
            // so we have to loop all the messages in order to find the optimal packet length
            for (size_t msg_j = 0; msg_j < msg_i; ++msg_j) {
                // this total msg length has been resolved before
                const auto total_msg_length_j = dp[msg_j][0];

                const auto total_msg_length_j_to_i = total_msg_length_i - total_msg_length_j;

                // clang-format off
                // This is the critical part!
                // K: messages count; L: packets count; dp[K][L]: min packet size given K & L;
                // dp[K][L] = min(dp[K][L], {j:[0, K]} max(dp[j][L-1], sum(j+1:K)));
                dp[msg_i][packet_idx] = std::min(dp[msg_i][packet_idx],
                                                 std::max(dp[msg_j][packet_idx - 1],
                                                          total_msg_length_j_to_i));

                /*
                std::cout << "\tmsg_i=" << msg_i << ", msg_j=" << msg_j
                          << ", packet_idx=" << packet_idx
                          << ", total_msg_length_i=" << total_msg_length_i
                          << ", total_msg_length_j_to_i=" << total_msg_length_j_to_i
                          << ", dp[" << msg_i << "][" << packet_idx << "]=" << dp[msg_i][packet_idx]
                          << std::endl;
                */
                // clang-format on
            }
        }
    }

    // std::cout << "\tdp[" << (num_messages - 1) << "]=" << util::dump_array(dp[num_messages - 1]) << std::endl;

    return dp[num_messages - 1][max_packets_count - 1];
}

uint32_t find_min_packet_length_2(const std::vector<uint32_t>& messages, uint32_t max_packets_count)
{
    const auto num_messages = messages.size();
    if (num_messages == 0 || max_packets_count == 0) {
        return 0;
    }

    uint32_t   total_message_len = 0;
    const auto max_message_len   = [&]() {
        auto value = messages.front();
        for (auto& v : messages) {
            if (v > value) {
                value = v;
            }
            total_message_len += v;
        }
        return value;
    }();

    auto result = (max_packets_count == 1 ? total_message_len : max_message_len);

    // the answer must be in the range [max_message_len, total_message_len], use binary search
    auto low  = max_message_len;
    auto high = total_message_len;
    while (low <= high) {
        auto mid = (low + high + 1) / 2;

        uint32_t num_packets       = 1;
        uint32_t packet_len        = 0;
        uint32_t num_sent_messages = 0;
        do {
            packet_len += messages[num_sent_messages];
            if (packet_len > mid) {
                packet_len = messages[num_sent_messages];
                ++num_packets;
            }
            ++num_sent_messages;
        } while (num_sent_messages < messages.size() && num_packets <= max_packets_count);

        if (num_sent_messages < messages.size() || num_packets > max_packets_count) {
            // 'mid' as packet size is too small
            low = mid + 1;
        } else if (num_packets < max_packets_count) {
            // 'mid' as packet size is too big
            high = mid - 1;
        } else {
            // NOTICE: this 'mid' packet size is able to send all messages with max_packets_count, but DOES NOT
            // guarantee current 'mid' is the optimal solution, so keep trying for better/smaller size
            std::cout << "\tfind a candidate packet size " << mid << " from range [" << low << ", " << high
                      << "], num_sent_messages=" << num_sent_messages << ", num_packets=" << num_packets << std::endl;
            result = mid;
            high   = mid - 1;
        }
    }
    return result;
}

void run_find_min_packet_length()
{
    using use_case_t = std::tuple<std::vector<uint32_t>, uint32_t, uint32_t>;
    for (auto& [messages, packet_count, exp_v] : {
             use_case_t{{4, 3, 10, 3, 2, 5}, 1, 27},
             use_case_t{{4, 3, 10, 3, 2, 5}, 2, 17},
             use_case_t{{4, 3, 10, 3, 2, 5}, 3, 10},
             use_case_t{{4, 3, 10, 3, 2, 5}, 4, 10},
             use_case_t{{4, 3, 10, 3, 2, 5}, 6, 10},
             use_case_t{{6, 5, 4, 3, 3, 2, 5}, 0, 0},
             use_case_t{{6, 5, 4, 3, 3, 2, 5}, 1, 28},
             use_case_t{{6, 5, 4, 3, 3, 2, 5}, 2, 15},
             use_case_t{{6, 5, 4, 3, 3, 2, 5}, 3, 11},
             use_case_t{{6, 5, 4, 3, 3, 2, 5}, 4, 9},
             use_case_t{{6, 5, 4, 3, 3, 2, 5}, 5, 7},
             use_case_t{{6, 5, 4, 3, 3, 2, 5}, 6, 6},
             use_case_t{{6, 5, 4, 3, 3, 2, 5}, 7, 6},
             use_case_t{{6, 5, 4, 3, 3, 2, 5}, 8, 6},

         }) {
        std::cout << __FUNCTION__ << ": messages=" << util::dump_array(messages)
                  << ", max_packets_count=" << packet_count << std::endl;

        const auto v1 = find_min_packet_length(messages, packet_count);
        std::cout << " find_min_packet_length=" << v1 << ", " << (exp_v == v1 ? "SUCCESS" : "FAILED") << std::endl;

        const auto v2 = find_min_packet_length_2(messages, packet_count);
        std::cout << " find_min_packet_length_2=" << v2 << ", " << (exp_v == v2 ? "SUCCESS" : "FAILED") << std::endl;
    }
}

