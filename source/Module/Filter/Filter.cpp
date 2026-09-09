#include "FilterConfig.h"
#include <Module.h>
#include <Filter.h>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

Filter::Filter(FilterConfigPtr config, ServerThreadPtr osc_server) : m_config(config) {
    if (!m_config->osc_controllable) return;
    
    if (osc_server != nullptr) {
        std::cout << "[info] Filter listening on path " << m_config->osc_path
            << " in format (index (int), frequency (float), type (lp/hp), enabled (0/1))"
            << std::endl;
        osc_server->add_method(m_config->osc_path, "ifsi", osc_filter_frequency_callback, this);
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
        m_a.push_back(std::vector<atomic_float>(m_config->order));
        m_b.push_back(std::vector<atomic_float>(m_n_taps));
    }
    return m_n_output_channels;
    
}

void Filter::prepare(HostAudioConfig host_audio_config) {
    m_is_enabled.resize(m_n_input_channels);
    m_coeff_calc.prepare(m_config->order, 1. / host_audio_config.m_host_sample_rate);
    for(size_t i = 0; i < m_n_input_channels; ++i) {
        m_is_enabled[i].store(true);
        set_filter(i, m_config->freq, m_config->type);
    }
}

void Filter::process(AudioBufferF &buffer, size_t n_frames) {
    for (size_t channel = 0; channel < m_n_input_channels; channel++) {
        // skip disabled channels
        if(!m_is_enabled[channel].load())   continue;

        for (size_t sample = 0; sample < n_frames; sample++) {

            float current_value = buffer.getSample(channel, sample);  

            // filter twice with different memory
            double y_n = filter_sample(channel, current_value, m_memory_1);
            y_n = filter_sample(channel, y_n, m_memory_2);
            buffer.setSample(channel, sample, y_n);
        }
    }
}

double Filter::filter_sample(size_t channel, double current_x, std::vector<std::vector<double>>& memory) {
    // Filter using this difference equation (direct form II IIR filter):
    // w[n] = sum_{k=1}^N a_k * w[n-k] + x[n]
    // y[n] = sum_{k=0}^M b_k * w[n-k]

    // TODO if necessary this function could be optimized, by using a ringbuffer
    // for memory and by merging the loops. this wouldn't help readability
    double w_n = current_x;
    double y_n = 0.0;

    // calculate w[n]
    for (size_t i = 1; i < m_n_taps; i++) {
        w_n -= memory[channel][i-1] * m_a[channel][i].load();
    }

    // move elements in memory back
    for (size_t i = 1; i < m_n_taps; i++) {
        size_t index = m_n_taps - i;
        memory[channel][index] = memory[channel][index-1];
    }
    memory[channel][0] = w_n;

    // calculate y[n]
    for (size_t i = 0; i < m_n_taps; i++) {
        y_n += m_b[channel][i].load() * memory[channel][i];
    }
    return y_n;
}

void Filter::set_filter(size_t channel, double freq, FilterType type) {
    switch (type) {
    case FilterType::LP:
        m_coeff_calc.calc_lowpass(freq);
        break;
    case FilterType::HP:
        m_coeff_calc.calc_highpass(freq);
        break;
    default:
        std::cout << "[ERROR] invalid Filter Type" << std::endl;
        throw "Invalid Filter Type";
    }

    const auto& a = m_coeff_calc.getA();
    for(size_t i = 0; i < m_a[channel].size(); ++i) {
        m_a[channel][i].store(a[i]);
    }

    const auto& b = m_coeff_calc.getB();
    for(size_t i = 0; i < m_b[channel].size(); ++i) {
        m_b[channel][i].store(b[i]);
    }
    
}

void Filter::set_enabled(size_t channel, bool is_enabled) {
    m_is_enabled[channel].store(is_enabled);
}

int Filter::osc_filter_frequency_callback(const char *path, const char *types, lo_arg **argv, int argc, lo_message data, void *user_data) {
    std::ignore = path;
    std::ignore = types;
    std::ignore = argc;
    std::ignore = data;
    Filter* filter = (Filter*) user_data;
    auto& config = filter->m_config;

    // arg 0: channel index
    const int channel = argv[0]->i;
    if(channel >= filter->m_n_input_channels) {
        std::cout<<"Channel index "<< channel<<" out of range!"<<std::endl;
        return -1;
    }

    // arg 1: Filter frequency
    const float freq = argv[1]->f;

    // arg 2: Filter type
    FilterType type;
    // assuming we are allowed to do a little heap allocation here ...
    // if not, we could still copy the incoming string to the stack
    std::string typeString(&(argv[2]->s));
    std::transform(typeString.begin(), typeString.end(), typeString.begin(),
                   [](auto c) { return std::tolower(c); });

    if(typeString == "lp") {
        type = FilterType::LP;
    } else if(typeString == "hp") {
        type = FilterType::HP;
    } else {
        std::cout<<"Unknown filter type " << typeString << std::endl;
        return -1;
    }

    // arg 3: on/off
    const auto enabled = static_cast<bool>(argv[3]->i);
    filter->set_enabled(channel, enabled);

    if(enabled) {
        filter->set_filter(channel, freq, type);
    }

    return 0;
}
