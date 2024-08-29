#ifndef MODULE_FILTER_H
#define MODULE_FILTER_H

#include <vector>
#include <Module.h>
#include <FilterConfig.h>
#include <iir.h>

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
        void calculate_filter_coefficients(double samplerate);
        std::vector<double> m_a;
        std::vector<double> m_b;
        std::vector<std::vector<double>> m_memory_1;
        std::vector<std::vector<double>> m_memory_2;
        
        double filter_sample(double new_value, std::vector<double> &memory);
        int m_n_taps;
};

#endif //MODULE_FILTER_H