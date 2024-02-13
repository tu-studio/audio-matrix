#ifndef JACK_CLIENT_H
#define JACK_CLIENT_H

#include <jack/jack.h>
#include <AudioMatrix.h>

class JackClient {

public:
    JackClient() = delete;
    JackClient([[ maybe_unused ]] int argc, char *argv[]);
    ~JackClient();

    void prepare();
    void prepare(HostAudioConfig host_audio_config);

    static int process (jack_nframes_t nframes, void *arg);
    static void shutdown (void *arg);
    static int buffer_size_callback(jack_nframes_t nframes, void *arg);
    static int sample_rate_callback(jack_nframes_t nframes, void *arg);
    
private:

    jack_client_t* m_client;
    jack_status_t m_status;
    jack_port_t **input_ports;
    jack_port_t **output_ports;
    jack_default_audio_sample_t **in;
    jack_default_audio_sample_t **out;

    AudioMatrix* audio_matrix;
};



#endif // JACK_CLIENT_H