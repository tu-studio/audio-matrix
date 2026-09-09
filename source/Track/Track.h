#ifndef TRACK_H
#define TRACK_H

#include <AudioBuffer.h>
#include <Module.h>
#include <vector>
#include <TrackConfig.h>

#include <Gain.h>
#include <AmbiEncoder.h>
#include <Sum.h>
#include <Filter.h>
#include <DistanceGain.h>
#include <Delay.h>

#include <OSCServer.h>
#include <HostAudioConfig.h>

class Track {

public:
    Track(const TrackConfig& config, std::shared_ptr<lo::ServerThread> osc_server);
    ~Track();

    size_t initialize(size_t n_input_channels, size_t output_channel_offset = 0);
    void prepare(HostAudioConfig host_audio_config);
    void process(float** in, float** out, size_t nframes);

    size_t get_output_channel_offset() { return m_output_channel_offset; }
    size_t get_n_output_channels() { return m_n_output_channels; }

    std::string get_name() { return m_config.name; }
    AudioBufferF& get_buffer() { return m_buffer; }
    std::vector<std::shared_ptr<Module>>& get_modules() { return m_modules; }

    template <typename ConfigClass, typename ModuleClass>
    void createAndAddModule(const std::shared_ptr<ModuleConfig>& module_config, 
                            const std::shared_ptr<lo::ServerThread>& osc_server) {
    if (auto config_cast = std::dynamic_pointer_cast<ConfigClass>(module_config)) {
        m_modules.push_back(std::make_shared<ModuleClass>(config_cast, osc_server));
    }
}
    
private:
    AudioBufferF m_buffer;
    std::vector<std::shared_ptr<Module>> m_modules;
    size_t m_output_channel_offset = 0;
    size_t m_n_input_channels = 0;
    size_t m_n_output_channels = 0;
    size_t m_max_n_channels = 0;
    const TrackConfig& m_config; 
};

using TrackPtr = std::unique_ptr<Track>;

#endif // TRACK_H
