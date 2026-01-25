#pragma once
#include "common_include.hpp"
#include "keys.hpp"
#include "utils.hpp"

class Unpack
{
public:
    enum class Status
    {
        OK = 0,
        WrongUniversalKey = 1,
        NoTimestamp = 2,
        NoLDSVersion = 3,
        NoChecksum = 4,
        WrongChecksum = 5
    };

    struct TagValuePair
    {
        std::string tag_name;
        GenericValue value;
        size_t size; 
    };

    Unpack() = default;
    ~Unpack() = default;

    Status unpack_misb(const uint8_t *data, size_t size, std::vector<TagValuePair> &tag_values);
    static const TagValuePair* find_tag_value(const std::vector<TagValuePair> &tag_values, const std::string &tag_name);
    void print_tag_values(const std::vector<TagValuePair> &tag_values) const;

private:
    static Status check_universal_key(uint16_t &checksum, size_t &index, const uint8_t *data);
    static size_t decode_packet_length(uint16_t &checksum, size_t &index, const uint8_t *data);
    static GenericValue decode_value(const Field &field, const uint8_t *value, uint8_t size);
    static uint16_t compute_checksum_byte(size_t index, const uint8_t *data);
    static std::string get_tag_name(Tag tag);
};