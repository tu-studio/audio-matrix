#ifndef MODULE_FILTER_H
#define MODULE_FILTER_H

#include <vector>
#include <Module.h>
#include <FilterConfig.h>
#include <iir.h>
#include <CustomAtomic.h>

class Filter : public Module {
    public:
        Filter() = delete;
        Filter(FilterConfigPtr config, ServerThreadPtr osc_server);
        ~Filter() = default;

        size_t initialize(size_t input_channels) override;
        void prepare(HostAudioConfig host_audio_config) override;
        void process(AudioBufferF &buffer, size_t n_frames) override;

        void set_enabled(size_t channel, bool is_enabled);
        void set_filter(size_t channel, double freq, FilterType type);
    private:
        FilterConfigPtr m_config;
        std::vector<std::vector<atomic_float>> m_a;
        std::vector<std::vector<atomic_float>> m_b;
        std::vector<std::vector<double>> m_memory_1;
        std::vector<std::vector<double>> m_memory_2;
        int m_n_taps;
        std::vector<atomic_bool> m_is_enabled;
        filter_coeff_calculator m_coeff_calc;
        
        double filter_sample(size_t channel, double current_x, std::vector<std::vector<double>>& memory);
        static int osc_filter_frequency_callback(const char *path, const char *types, lo_arg **argv, int argc, lo_message data, void *user_data);
};

#endif //MODULE_FILTER_H
