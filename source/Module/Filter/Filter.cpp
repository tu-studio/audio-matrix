#include <Module.h>
#include <Filter.h>
#include <cstring>

Filter::Filter(FilterConfigPtr config, ServerThreadPtr osc_server) : m_config(config) {
    if (!m_config->osc_controllable) return;
    
    if (osc_server != nullptr) {
        std::cout << "[info] Filter listening on path " << m_config->osc_path
            << " in format (frequency (float), type (lp/hp), enabled (0/1))"
            << std::endl;
        osc_server->add_method(m_config->osc_path, "fsi", osc_filter_frequency_callback, this);
    } else {
        std::cout << "[error] osc_server is null, cannot add filter callback method" << std::endl;
    }
}

size_t Filter::initialize(size_t input_channels) {
    m_n_input_channels = input_channels;
    m_n_output_channels = input_channels;
    m_n_taps = m_config->order / 2 + 1;
    
    for (size_t i = 0; i < input_channels; i++) {
        m_memory_1.push_back(std::vector<double>(m_n_taps, 0.0) );
        m_memory_2.push_back(std::vector<double>(m_n_taps, 0.0) );
    }
    return m_n_output_channels;
    
}

void Filter::prepare(HostAudioConfig host_audio_config) {
    m_a.resize(m_config->order);
    m_b.resize(m_n_taps);

    coeff_calc.prepare(m_config->order, 1. / host_audio_config.m_host_sample_rate);
    calculate_filter_coefficients();
}

void Filter::process(AudioBufferF &buffer, size_t n_frames) {
    if(!is_enabled) return;

    for (size_t channel = 0; channel < m_n_input_channels; channel++) {
        for (size_t sample = 0; sample < n_frames; sample++) {

            float current_value = buffer.getSample(channel, sample);  

            // filter twice with different memory
            double y_n = filter_sample(current_value, m_memory_1[channel] );
            y_n = filter_sample(y_n, m_memory_2[channel] );

            buffer.setSample(channel, sample, y_n);
        }
    }
}

double Filter::filter_sample(double current_x, std::vector<double> &memory) {
    // Filter using this difference equasion (direct form II IIR filter):
    // w[n] = sum_{k=1}^N a_k * w[n-k] + x[n]
    // y[n] = sum_{k=0}^M b_k * w[n-k]

    // TODO if necessary this function could be optimized, by using a ringbuffer for memory and by merging the loops. this wouldn't help readability
    double w_n = current_x;
    double y_n = 0.0;

    // calculate w[n]
    for (size_t i = 1; i < m_n_taps; i++) {
        w_n -= memory[i-1] * m_a[i];
    }

    // move elements in memory back
    for (size_t i = 1; i < m_n_taps; i++) {
        size_t index = m_n_taps - i;
        memory[index] = memory[index-1];
    }
    memory[0] = w_n;

    // calculate y[n]
    for (size_t i = 0; i < m_n_taps; i++) {
        y_n += m_b[i] * memory[i];
    }
    return y_n;
}

void Filter::calculate_filter_coefficients() {
    switch (m_config->type) {
    case FilterType::LP:
        coeff_calc.calc_lowpass(m_config->freq);
        break;
    case FilterType::HP:
        coeff_calc.calc_highpass(m_config->freq);
        break;
    default:
        std::cout << "[ERROR] invalid Filter Type" << std::endl;
        throw "Invalid Filter Type";
    }

    // TODO:
    // do something more sophisticated/atomic than just copying :]
    for(size_t i = 0; i < m_a.size(); ++i) {
        m_a[i] = coeff_calc.getA()[i];
    }
    for(size_t i = 0; i < m_b.size(); ++i) {
        m_b[i] = coeff_calc.getB()[i];
    }
    
}

void Filter::set_enabled(bool is_enabled_) {
    is_enabled = is_enabled_;
}

int Filter::osc_filter_frequency_callback(const char *path, const char *types, lo_arg **argv, int argc, lo_message data, void *user_data) {
    std::ignore = path;
    std::ignore = types;
    std::ignore = argc;
    std::ignore = data;
    Filter* filter = (Filter*) user_data;
    auto& config = filter->m_config;

    // Filter frequency
    const float f = argv[0]->f;

    // Filter type
    char* type = &(argv[1]->s);
    if(!std::strncmp(type, "lp", 2)) {
        config->type = FilterType::LP;
    } else if(!std::strncmp(type, "hp", 2)) {
        config->type = FilterType::HP;
    } else {
        std::cout<<"Unknown filter type " << type << std::endl;
        return -1;
    }

    filter->calculate_filter_coefficients();

    // on or off
    filter->set_enabled((bool)argv[2]->i);

    return 0;
}
