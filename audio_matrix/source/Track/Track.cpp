#include <Track.h>

Track::Track() {
}

Track::~Track() {
}

void Track::initialize(int channels, int frames) {
    buffer.initialize(channels, frames);
}

void Track::process(jack_nframes_t nframes) {
    for (auto& module : modules) {
        module->process(buffer, nframes);
    }
}

AudioBuffer& Track::get_buffer() {
    return buffer;
}

std::vector<Module*>& Track::get_modules() {
    return modules;
}

void Track::add_module(Module* module) {
    modules.push_back(module);
}