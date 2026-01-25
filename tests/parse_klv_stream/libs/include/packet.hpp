#pragma once
#include "common_include.hpp"
#include "keys.hpp"
#include "utils.hpp"

class Packet
{
public:
    Packet() = default;
    ~Packet() = default;

    bool new_packet();

    bool add_klv(Tag tag, uint8_t value);
    bool add_klv(Tag tag, uint16_t value);
    bool add_klv(Tag tag, uint32_t value);
    bool add_klv(Tag tag, uint64_t value);
    bool add_klv(Tag tag, int8_t value);
    bool add_klv(Tag tag, int16_t value);
    bool add_klv(Tag tag, int32_t value);
    bool add_klv(Tag tag, int64_t value);
    bool add_klv(Tag tag, float value);
    bool add_klv(Tag tag, double value);
    bool add_klv(Tag tag, const char *value);

    int finalize_packet();

    const uint8_t *get_content() const { return data_.content.data(); }
    size_t get_size() const { return data_.content.size(); }
    bool is_ready() const { return data_.ready; }

private:
    struct Klv_Data
    {
        std::vector<uint8_t> content;
        bool ready;
    } data_{{}, false};

    bool add_klv(const Field &field, const GenericValue &value);
    char *encode_value(const Field &field, GenericValue &value);
    std::vector<uint8_t> create_final_packet(int length_size, size_t total_size) const;
    int compute_length_size(size_t value_length) const;
};