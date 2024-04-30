#ifndef AMBIENCODER_H
#define AMBIENCODER_H

#include <vector>
#include <Module.h>
#include <AmbiEncoderConfig.h>

class AmbiEncoder : public Module{
    public:
        AmbiEncoder() = delete;
        AmbiEncoder(AmbiEncoderConfigPtr config, std::shared_ptr<lo::ServerThread> osc_server);
        ~AmbiEncoder() = default;

        size_t initialize(size_t input_channels) override;
        void process(AudioBufferF &buffer, size_t nframes) override;

        void set_azimuth(size_t channel, float azimuth);
        void set_elevation(size_t channel, float elevation);
        void set_distance(size_t channel, float distance);
        void set_aed(size_t channel, float azimuth, float elevation, float distance);

    private:
        AmbiEncoderConfigPtr m_config;
        std::vector<float> m_azimuth, m_elevation, m_distance;
        static int osc_gain_callback(const char *path, const char *types, lo_arg **argv, int argc, lo_message data, void *user_data);


};


#endif //AMBIENCODER_H