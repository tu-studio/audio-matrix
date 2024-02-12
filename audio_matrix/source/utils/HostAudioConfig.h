#ifndef HOSTAUDIOCONFIG_H
#define HOSTAUDIOCONFIG_H

#include <cstddef>

struct HostAudioConfig {
    size_t hostBufferSize;
    double hostSampleRate;

    bool operator==(const HostAudioConfig& other) const {
        return hostBufferSize == other.hostBufferSize && hostSampleRate == other.hostSampleRate;
    }

    bool operator!=(const HostAudioConfig& other) const {
        return !(*this == other);
    }
};

#endif //HOSTAUDIOCONFIG_H