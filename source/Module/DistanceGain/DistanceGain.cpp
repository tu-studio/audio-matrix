#include <DistanceGain.h>
#include <Common.h>
#define DISTANCE_GAIN_INITIAL_GAIN 1.0

DistanceGain::DistanceGain(DistanceGainConfigPtr config, std::shared_ptr<lo::ServerThread> osc_server): m_config(config){
    if (m_config == nullptr){
        // TODO what to do?
    } else
    if (m_config->osc_controllable && osc_server != nullptr){
        std::cout << "[info] DistanceGain listening on path " << m_config->osc_path << std::endl;
        osc_server->add_method(m_config->osc_path, "if", distance_osc_callback, this);
    } else if(m_config->osc_controllable && osc_server == nullptr){
        std::cout << "[error] osc_server is null, cannot add DistanceGain callback method" << std::endl;
    }
}

size_t DistanceGain::initialize(size_t input_channels) {
    m_n_input_channels = input_channels;
    m_n_output_channels = input_channels;
    for (size_t i = 0; i < input_channels; i++) {
        m_gain.push_back(DISTANCE_GAIN_INITIAL_GAIN);
        m_prev_gain.push_back(DISTANCE_GAIN_INITIAL_GAIN);
    }
    return m_n_output_channels;
}

void DistanceGain::process(AudioBufferF &buffer, size_t nframes) {
    for (size_t channel = 0; channel < m_n_input_channels; channel++)
    {
        if (m_gain[channel] != m_prev_gain[channel]){
            float gain_prev = m_prev_gain[channel];
            float gain_new = m_gain[channel];
            float stepsize = (gain_new-gain_prev) / nframes;
            for (size_t frame = 0; frame < nframes; frame++){
                float gain = gain_prev + (stepsize * frame);
                buffer.setSample(channel, frame, buffer.getSample(channel,frame) * gain);
            }
            m_prev_gain[channel] = gain_new;

        } else {
            for (size_t frame = 0; frame < nframes; frame++){
                buffer.setSample(channel, frame, buffer.getSample(channel,frame) * m_gain[channel]);
            }
        }
        
    }
    
}

void DistanceGain::set_distance(size_t channel, float distance){
    m_gain[channel] = distance_gain_function(distance);
    std::cout << "[debug] distance: " << distance << ", gain: " << m_gain[channel] << std::endl;
}


int DistanceGain::distance_osc_callback(const char *path, const char *types, lo_arg **argv, int argc, lo_message data, void *user_data){
    std::ignore = path;
    std::ignore = types;
    std::ignore = argc;
    std::ignore = data;
    DistanceGain* gain = (DistanceGain*) user_data;
    // TODO check correct number of arguments
    if(argc==2){
        gain->set_distance(argv[0]->i, argv[1]->f);
        return 0;
    }
    return -1;
}