#ifndef AMBIENCODER_H
#define AMBIENCODER_H

#include <vector>
#include <Module.h>
#include <AmbiEncoderConfig.h>
#include <HostAudioConfig.h>
#include <SphericalHarmonics.h>
#include <CustomAtomic.h>

struct PositionAED {
    float azim;
    float elev;
    float dist;
};

class AmbiEncoder : public Module{
    public:
        AmbiEncoder() = delete;
        AmbiEncoder(std::shared_ptr<AmbiEncoderConfig> config, std::shared_ptr<lo::ServerThread> osc_server);
        ~AmbiEncoder() = default;

        size_t initialize(size_t input_channels) override;
        void prepare(HostAudioConfig host_audio_config) override;
        void process(AudioBufferF &buffer, size_t nframes) override;

        // void set_azimuth(size_t channel, float azimuth);
        // void set_elevation(size_t channel, float elevation);
        // void set_distance(size_t channel, float distance);
        void set_aed(size_t channel, float azimuth, float elevation, float distance);


    private:

        void update_spherical_harmonics(size_t channel);
        static int osc_pos_callback(const char *path, const char *types, lo_arg **argv, int argc, lo_message data, void *user_data);

        AmbiEncoderConfigPtr m_config;
        std::vector<PositionAED> m_position;
        
        AudioBufferF m_buffer;
        float m_nearfield_size;
        
        SphericalHarmonics m_sh;
        std::vector<std::vector<float>> m_sh_containers;
        std::vector<std::vector<float>> m_sh_containers_prev;
        std::vector<atomic_bool> m_position_changed;

};


#endif //AMBIENCODER_H
