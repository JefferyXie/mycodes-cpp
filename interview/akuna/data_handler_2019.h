#ifndef DATA_HANDLER_2019_H
#define DATA_HANDLER_2019_H

#include "./include.h"

struct calc_data_1
{
    uint64_t cat_id;
    uint64_t seq_id;
    std::vector<uint64_t> payload_1;
};
struct calc_data_2
{
    uint64_t cat_id;
    uint64_t seq_id;
    std::vector<uint64_t> payload_2;
};
struct calc_data_3
{
    uint64_t cat_id;
    uint64_t seq_id;
    std::vector<uint64_t> payload_3;
};

// same cat id
// CD1 1   2  3
// CD2  1   2   4 
// CD3   1       3 4 5 6 7 8 9 10

struct data_packet {
	enum PACKET_MASK {
		NONE   = 0,
		DATA_1 = 1 << 0,
		DATA_2 = 1 << 1,
		DATA_3 = 1 << 2,
		ALL = DATA_1 | DATA_2 | DATA_3,
	};
	uint8_t  mask = NONE;
    uint64_t seq_id = 0;
    std::vector<vector<uint64_t>> vec_payload;
};
std::unordered_map<uint64_t, std::map<uint64_t, data_packet>> cats;

data_packet
handle_data(const calc_data_1& data, data_packet& output) {
	handle_data(data.cat_id, data.seq_id, data.payload_1, DATA_1);
}
data_packet
handle_data(const calc_data_2& data, data_packet& output) {
	return handle_data(data.cat_id, data.seq_id, data.payload_2, DATA_2);
}
data_packet
handle_data(const calc_data_3& data, data_packet& output) {
	return handle_data(data.cat_id, data.seq_id, data.payload_3, DATA_3);
}

data_packet
handle_data(uint64_t cat_id, uint64_t seq_id,
            const vector<uint64_t>& payload, uint64_t data_mask)
{
	data_packet output;
    auto it = cats.find(cat_id);
    if (it == cats.end()) {
		data_packet packet;
		packet.mask = data_mask;
		packet.seq_id = seq_id;
		packet.vec_payload.push_back(payload);
		cats.insert(cat_id, {seq_id, packet});
	} else {
        auto& seq_map = it->second;
        auto it_seq = seq_map.find(seq_id);
        if (it_seq == seq_map.end()) {
			data_packet packet;
			packet.mask = data_mask;
			packet.seq_id = seq_id;
			packet.vec_payload.push_back(payload);
			seq_map.insert(seq_id, packet);
		} else {
			auto& packet = it_seq->second;
            packet.vec_payload.push_back(input.payload);
			packet.mask |= data_mask;
        }
		auto it_top = seq_map.begin();
		auto it_sec = it_top + 1;
		if (it_top->second.mask == ALL ||
			(it_sec != seq_map.end() &&
			 (it_top->second.mask | it_sec->second.mask) == ALL)) {
			output = std::move(it_top->second);
			seq_map.erase(it_top);
		}
    }
	return output;
}

#endif

