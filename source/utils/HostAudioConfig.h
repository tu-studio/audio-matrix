#ifndef HOSTAUDIOCONFIG_H
#define HOSTAUDIOCONFIG_H

#include <cstddef>

struct HostAudioConfig {

    HostAudioConfig() = default;
    HostAudioConfig(size_t host_buffer_size, double host_sample_rate)
        : m_host_buffer_size(host_buffer_size), m_host_sample_rate(host_sample_rate) { }

    size_t m_host_buffer_size;
    double m_host_sample_rate;

    bool operator==(const HostAudioConfig& other) const {
        return m_host_buffer_size == other.m_host_buffer_size && m_host_sample_rate == other.m_host_sample_rate;
    }

    bool operator!=(const HostAudioConfig& other) const {
        return !(*this == other);
    }
};

#endif //HOSTAUDIOCONFIG_H