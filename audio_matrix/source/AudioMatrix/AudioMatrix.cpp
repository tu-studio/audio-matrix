#include <AudioMatrix.h>

AudioMatrix::AudioMatrix(std::string config_path): config_parser(config_path)
{
    config = config_parser.get_config();
    osc_server = OSCServer(config->port);
    for (TrackConfig track : config->tracks){
        Track new_track(track);
        tracks.push_back(new_track);
    }
}

AudioMatrix::~AudioMatrix()
{
}