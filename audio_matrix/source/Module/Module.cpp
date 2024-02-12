#include <Module.h>

Module::Module() {
}

Module::~Module() {
}

void Module::prepare([[ maybe_unused ]] HostAudioConfig host_audio_config) {
}

std::string Module::get_name() {
    return name;
}
