#ifndef MODULE_FILTER_H
#define MODULE_FILTER_H

#include <atomic>
#include <vector>
#include <Module.h>
#include <FilterConfig.h>
#include <iir.h>

class Filter : public Module {
    public:
        Filter() = delete;
        Filter(FilterConfigPtr config, ServerThreadPtr osc_server);
        ~Filter() = default;

        size_t initialize(size_t input_channels) override;
        void prepare(HostAudioConfig host_audio_config) override;
        void process(AudioBufferF &buffer, size_t n_frames) override;

        void set_enabled(bool is_enabled_);
    private:
        FilterConfigPtr m_config;
        void calculate_filter_coefficients(double samplerate);
        std::vector<double> m_a;
        std::vector<double> m_b;
        std::vector<std::vector<double>> m_memory_1;
        std::vector<std::vector<double>> m_memory_2;
        
        double filter_sample(double new_value, std::vector<double> &memory);
        static int osc_filter_frequency_callback(const char *path, const char *types, lo_arg **argv, int argc, lo_message data, void *user_data);
        int m_n_taps;
        std::atomic<bool> is_enabled {true};

        // filter coefficient internal state stuff
        std::vector<int> m_b_coeffs_int;
        std::vector<double> m_rcof;
};

#endif //MODULE_FILTER_H
