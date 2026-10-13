#include <Delay.h>
#include <cstddef>
#include <iostream>

Delay::Delay(DelayConfigPtr config, std::shared_ptr<lo::ServerThread> osc_server): m_config(config) {
    if(!m_config->osc_controllable) return;

    if (osc_server != nullptr) {
        std::cout << "[info] Delay listening on path " << m_config->osc_path
            << " in format (index (int), time (in ms, float), enabled (0/1))"
            << std::endl;
        osc_server->add_method(m_config->osc_path, "ifi", osc_delay_frequency_callback, this);
    } else {
        std::cout << "[error] osc_server is null, cannot add delay callback method" << std::endl;
    }
}

size_t Delay::initialize(size_t input_channels) {
    m_n_input_channels = input_channels;
    m_n_output_channels = input_channels;
    return m_n_output_channels;
}

void Delay::prepare(HostAudioConfig host_audio_config) {
    m_samplerate = host_audio_config.m_host_sample_rate;
    m_max_delay_samps = static_cast<size_t>(m_samplerate * max_delay_s);

    m_ringbuffer.initialize(m_n_input_channels, m_max_delay_samps);
    m_ringbuffer.clear();
    m_is_enabled.resize(m_n_input_channels);
    m_delay_samps.resize(m_n_input_channels);

    for(size_t channel = 0; channel < m_n_input_channels; ++channel) {
        m_is_enabled[channel].store(true);
        set_time_ms(channel, m_config->delay_time);
    }
}

void Delay::process(AudioBufferF &buffer, size_t nframes){
    for (size_t channel = 0; channel < m_n_input_channels; channel++) {
        if(!m_is_enabled[channel].load())    continue;
        const auto delay_samps = static_cast<size_t>(m_delay_samps[channel].load());
        for (size_t sample = 0; sample < nframes; sample++) {
            float current = buffer.getSample(channel, sample);
            float delayed = m_ringbuffer.delay(channel, delay_samps, current);
            buffer.setSample(channel, sample, delayed);
        }
    }  
}

void Delay::set_time_ms(size_t channel, float time_ms) {
    const auto max = m_ringbuffer.getMaxNumSamples(channel) - 1;
    auto delay_samps = static_cast<size_t>(time_ms * 0.001f * m_samplerate);

    if(delay_samps > max) {
        std::cout<<"[warn] delay time clamped to "<< max <<std::endl;
        delay_samps = max;
    }

    m_delay_samps[channel].store(delay_samps);
}

void Delay::set_enabled(size_t channel, bool is_enabled) {
    m_is_enabled[channel].store(is_enabled);
}

int Delay::osc_delay_frequency_callback(const char *path, const char *types, lo_arg **argv, int argc, lo_message data, void *user_data) {
    std::ignore = path;
    std::ignore = types;
    std::ignore = argc;
    std::ignore = data;
    Delay* delay = (Delay*) user_data;
    auto& config = delay->m_config;

    // arg 0: channel index
    const int channel = argv[0]->i;
    if(channel >= delay->m_n_input_channels) {
        std::cout<<"[warn] tried to set delay on invalid channel "<< channel<<std::endl;
        return -1;
    }

    // arg 1: delay time
    const float delay_ms = argv[1]->f;
    delay->set_time_ms(channel, delay_ms);

    // arg 2: on/off
    const auto enabled = static_cast<bool>(argv[2]->i);
    delay->set_enabled(channel, enabled);

    return 0;
}
