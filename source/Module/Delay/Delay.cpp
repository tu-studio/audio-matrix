#include <Delay.h>

Delay::Delay(DelayConfigPtr config, std::shared_ptr<lo::ServerThread> osc_server): m_config(config){
}

size_t Delay::initialize(size_t input_channels){
    m_n_input_channels = input_channels;
    m_n_output_channels = input_channels;
    return m_n_output_channels;
}

void Delay::prepare(HostAudioConfig host_audio_config){
    size_t delay_samples = (size_t) host_audio_config.m_host_sample_rate * (m_config->delay_time / 1000.0f); 
    m_ringbuffer.initializeWithPositions(m_n_input_channels, delay_samples);
    m_ringbuffer.clear();
}

void Delay::process(AudioBufferF &buffer, size_t nframes){
    for (size_t channel = 0; channel < m_n_input_channels; channel++){
        for (size_t sample = 0; sample < nframes; sample++){
            
            float delayed_val = m_ringbuffer.popSample(channel);
            m_ringbuffer.pushSample(channel, buffer.getSample(channel, sample));
            buffer.setSample(channel, sample, delayed_val);
        }
    }  
}