#ifndef FIND_MIN_PACKET_LENGTH_H
#define FIND_MIN_PACKET_LENGTH_H

#include "../main/header.h"
#include "../main/utility.h"

//
// 1) "A market data publisher must publish all messages in a queue within time Limit milliseconds.
// 2) You are given an integer array "A" of length N where A[i] represents the size in bytes of the ith message and
// integer Y that represents the time Limit milliseconds.
// 3) Messages are batched in a packet buffer and sent every millisecond to improve throughput. The total size in bytes
// of the messages must not exceed the size of the packet buffer.
// 4) And messages must be sent in the order they have been queued. If a message cannot fit in the packet buffer, it
// will have to wait for the messages already in the buffer to get sent. Once the messages are sent, the packet buffer
// will be cleared and new messages can get queued. 4) Find the minimum packet buffer size that will allow for all
// messages in the queue to be published within timeLimit milliseconds.
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
    std::cout << __FUNCTION__ << ": [INPUT] max_packets_count=" << max_packets_count << ", messages=";
    print_array(messages);
    std::cout << std::endl;

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
        std::cout << "\tfind_max=" << find_max(messages) << std::endl;
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

            // TODO: this loop might be optimized, i.e. msg_j doesn't have to go through whole [0:msg_i)
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

                std::cout << "\tmsg_i=" << msg_i << ", msg_j=" << msg_j
                          << ", packet_idx=" << packet_idx
                          << ", total_msg_length_i=" << total_msg_length_i
                          << ", total_msg_length_j_to_i=" << total_msg_length_j_to_i
                          << ", dp[" << msg_i << "][" << packet_idx << "]=" << dp[msg_i][packet_idx]
                          << std::endl;
                // clang-format on
            }
        }
    }

    std::cout << "\tdp[" << (num_messages - 1) << "]=";
    print_array(dp[num_messages - 1]);
    std::cout << std::endl;

    return dp[num_messages - 1][max_packets_count - 1];
}

#endif
