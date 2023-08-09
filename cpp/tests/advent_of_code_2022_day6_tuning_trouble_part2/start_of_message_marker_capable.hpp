#pragma once
#include <string>
#include <cstdint>
#include <vector>

class StartOfMessageMarkerCapable {
public:
    /**
     * @brief Get the start of message marker indicies is a vector containing all indices of start
     * message which are a sequence of num_distinct_letters
     *
     * @param message message to parse
     * @param num_distinct_letters number of letters for finding start of message marker
     * @return std::vector<uint32_t>
     */
    virtual std::vector<uint32_t> get_start_of_message_marker_indicies(const std::string& message,
                                                                       uint8_t num_distinct_letters) = 0;
};

class SimpleStartMessageMarker : public StartOfMessageMarkerCapable {
public:
    std::vector<uint32_t> get_start_of_message_marker_indicies(const std::string& message,
                                                               uint8_t num_distinct_letters) override;
};


class SuperSimpleStartMessageMarker : public StartOfMessageMarkerCapable {
public:
    std::vector<uint32_t> get_start_of_message_marker_indicies(const std::string& message,
                                                               uint8_t num_distinct_letters) override;
};


class ComplexStartMessageMarker : public StartOfMessageMarkerCapable {
public:
    std::vector<uint32_t> get_start_of_message_marker_indicies(const std::string& message,
                                                               uint8_t num_distinct_letters) override;
};