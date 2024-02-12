#include <AudioMatrix.h>

AudioMatrix::AudioMatrix(std::string config_path): config_parser(config_path)
{
    config = config_parser.get_config();
    for (TrackConfig track : config->tracks){
        Track new_track(track);


        
    }
}

AudioMatrix::~AudioMatrix()
{
}