#ifndef AUDIOMATRIX_H
#define AUDIOMATRIX_H

#include <AudioMatrixConfig.h>
#include <Track.h>
#include <vector>
#include <string>
#include <memory>
#include <ConfigParser.h>
#include <OSCServer.h>

class AudioMatrix
{
public:
    AudioMatrix(std::string config_path);
    ~AudioMatrix();

private:
    std::vector<Track> tracks;
    std::shared_ptr<AudioMatrixConfig> config;
    ConfigParser config_parser;
    OSCServer osc_server;

};




#endif //AUDIOMATRIX_H