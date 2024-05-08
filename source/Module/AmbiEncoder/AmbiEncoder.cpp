#include <AmbiEncoder.h>

AmbiEncoder::AmbiEncoder(AmbiEncoderConfigPtr config, std::shared_ptr<lo::ServerThread> osc_server): m_config(config){
    if (m_config->osc_controllable && osc_server != nullptr){
        std::cout << "[info] Registering function on path " << m_config->osc_path << std::endl;
        osc_server->add_method(m_config->osc_path, "ifff", osc_pos_callback, this);
    } else if(m_config->osc_controllable && osc_server == nullptr){
        std::cout << "[error] osc_server is null, cannot add ambi encoder callback method" << std::endl;
    }

    m_sh.initalize(m_config->order);
}

size_t AmbiEncoder::initialize(size_t input_channels) {
    m_n_input_channels = input_channels;
    int power_order = m_config->order + 1;
    m_n_output_channels = power_order * power_order;

    // initialize position arrays and containers for spherical harmonics
    // for (size_t i = 0; i < input_channels; i++) {
    //     m_position.push_back({0,0,1});
    //     m_sh_containers.push_back(std::vector<float>(m_n_output_channels));
    //     m_sh_containers_prev.push_back(std::vector<float>(m_n_output_channels));
    //     m_position_changed.push_back(true);
    // }
    m_position = std::vector<PositionAED>(m_n_input_channels, {0,0,1});
    m_sh_containers = std::vector<std::vector<float>>(m_n_input_channels, std::vector<float>(m_n_output_channels, 0));
    m_sh_containers_prev = std::vector<std::vector<float>>(m_n_input_channels, std::vector<float>(m_n_output_channels, 0));
    m_position_changed = std::vector<bool>(m_n_input_channels, true);

    return m_n_output_channels;
}

void AmbiEncoder::prepare(HostAudioConfig host_audio_config){
    m_buffer.initialize(m_n_output_channels, host_audio_config.m_host_buffer_size);
}

void AmbiEncoder::process(AudioBufferF &buffer, size_t nframes){
    m_buffer.clear();
    // TODO implement smoothing, saving of previous SH buffer
    for (size_t in_channel = 0; in_channel < m_n_input_channels; in_channel++) {
        if (m_position_changed[in_channel]){
            m_position_changed[in_channel] = false;
            update_spherical_harmonics(in_channel);
            // std::cout << "source " << in_channel <<" changed: " << m_sh_containers_prev[in_channel][0] << "->" << m_sh_containers[in_channel][0] << std::endl;

            for (size_t out_channel = 0; out_channel < m_n_output_channels; out_channel++){
                float sh_prev = m_sh_containers_prev[in_channel][out_channel];
                float sh_new = m_sh_containers[in_channel][out_channel];
                float stepsize = (sh_new-sh_prev) / nframes;
                for (size_t j = 0; j < nframes; j++){
                    float sample = m_buffer.getSample(out_channel, j) + buffer.getSample(in_channel, j) * (sh_prev + j *stepsize);
                    m_buffer.setSample(out_channel, j, sample);
                }

                // update sh in prev container
                m_sh_containers_prev[in_channel][out_channel] = sh_new;
            }
        } else {
            for (size_t out_channel = 0; out_channel < m_n_output_channels; out_channel++){
                for (size_t j = 0; j < nframes; j++){
                    float sample = m_buffer.getSample(out_channel, j) + buffer.getSample(in_channel, j) * m_sh_containers_prev[in_channel][out_channel];
                    m_buffer.setSample(out_channel, j, sample);
                }
            }
        }        
    }

    for (size_t out_channel = 0; out_channel < m_n_output_channels; out_channel++){
        for (size_t j = 0; j < nframes; j++){
            buffer.setSample(out_channel, j, m_buffer.getSample(out_channel, j));
        }
    }
 
}

void AmbiEncoder::set_aed(size_t channel, float azimuth, float elevation, float distance){
    if (!(channel >= 0 && channel < m_n_input_channels)){
        return;
    }
    std::cout << "updated position for source " << channel << std::endl;
    m_position[channel].azim = azimuth;
    m_position[channel].elev = elevation;
    m_position[channel].dist = distance;
    m_position_changed[channel] = true;
    // TODO set m_is_changed = True
}



void AmbiEncoder::update_spherical_harmonics(size_t channel){
    m_sh.update_spherical_harmonics(m_position[channel].azim, m_position[channel].elev, m_position[channel].dist, m_sh_containers[channel]);
    
    // TODO
}


int AmbiEncoder::osc_pos_callback([[ maybe_unused ]] const char *path,[[ maybe_unused ]] const char *types, lo_arg **argv, int argc, [[ maybe_unused ]] lo_message data, void *user_data){
    // std::ignore = path;
    // std::ignore = types;
    // std::ignore = data;
    if(argc==4){
        AmbiEncoder* ambi_encoder = (AmbiEncoder*) user_data;
        ambi_encoder->set_aed(argv[0]->i, argv[1]->f, argv[2]->f, argv[3]->f);
        return 0;
    }
    return -1;
}