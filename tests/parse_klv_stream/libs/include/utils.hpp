#pragma once
#include "common_include.hpp"

class Utils
{
public:
    // Encode DEC --> UINT8
    static uint8_t unsigned_dec_to_uint8(float value, float range, float offset);
    static uint8_t unsigned_dec_to_uint8(double value, double range, double offset);

    // Encode DEC --> UINT16
    static uint16_t unsigned_dec_to_uint16(float value, float range, float offset);
    static uint16_t unsigned_dec_to_uint16(double value, double range, double offset);

    // Encode DEC --> UINT32
    static uint32_t unsigned_dec_to_uint32(float value, float range, float offset);
    static uint32_t unsigned_dec_to_uint32(double value, double range, double offset);

    // Encode DEC --> INT16
    static int16_t signed_dec_to_int16(float value, float range);
    static int16_t signed_dec_to_int16(double value, double range);

    // Encode DEC --> INT32
    static int32_t signed_dec_to_int32(float value, float range);
    static int32_t signed_dec_to_int32(double value, double range);

    /////////////////////////////////////////////////////////////////////////////////////////////

    // Decode UINT8 --> DEC
    static float uint8_to_unsigned_dec(uint8_t value, float range, float offset);
    static double uint8_to_unsigned_dec(uint8_t value, double range, double offset);

    // Decode UINT16 --> DEC
    static float uint16_to_unsigned_dec(uint16_t value, float range, float offset);
    static double uint16_to_unsigned_dec(uint16_t value, double range, double offset);

    // Decode UINT32 --> DEC
    static float uint32_to_unsigned_dec(uint32_t value, float range, float offset);
    static double uint32_to_unsigned_dec(uint32_t value, double range, double offset);

    // Decode INT16 --> DEC
    static float int16_to_signed_dec(int16_t value, float range);
    static double int16_to_signed_dec(int16_t value, double range);

    // Decode INT32 --> DEC
    static float int32_to_signed_dec(int32_t value, float range);
    static double int32_to_signed_dec(int32_t value, double range);

    /////////////////////////////////////////////////////////////////////////////////////////////

    static uint16_t bcc_16(uint8_t *buff, uint16_t len);
    static uint64_t get_timestamp();
    static std::string uint64_to_timestamp(uint64_t timestamp_us);
    static uint64_t get_localtimestamp();
    static std::string uint64_to_localtimestamp(uint64_t local_timestamp_us);
};