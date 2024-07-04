#ifndef FILTER_H
#define FILTER_H

#include <Module.h>
#include <FilterConfig.h>

class Filter : public Module {
    public:
        Filter() = delete;
        Filter(FilterConfigPtr config, std::shared_ptr<lo::ServerThread> osc_server);
        ~Filter() = default;

        size_t initialize(size_t input_channels) override;
        void prepare(HostAudioConfig host_audio_config) override;
        void process(AudioBufferF &buffer, size_t n_frames) override;
    private:
        FilterConfigPtr m_config;
        void calculate_filter_coefficients();
        std::vector<float> m_a;
        std::vector<float> m_b;
        std::vector<std::vector<float>> m_memory;
};

#endif //FILTER_H