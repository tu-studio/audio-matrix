#include <JackClient.h>

JackClient::JackClient([[ maybe_unused ]] int argc, char *argv[]) {

    const char *client_name;
    const char *server_name = NULL;
    int options = JackNullOption;

    // strrchr: locate last occurrence of character in string
    client_name = strrchr ( argv[0], '/' );
    if ( client_name == 0 ) {
        client_name = argv[0];
    }
    else {
        client_name++;
    }

    /* open a client connection to the JACK server */
    m_client = jack_client_open ( client_name, (jack_options_t) options, &m_status, server_name );
    if ( m_client == NULL ) {
        fprintf ( stderr, "jack_client_open() failed, "
                  "status = 0x%2.0x\n", m_status );
        if ( m_status & JackServerFailed )
        {
            fprintf ( stderr, "Unable to connect to JACK server\n" );
        }
        exit ( 1 );
    }
    if ( m_status & JackServerStarted )
    {
        fprintf ( stderr, "JACK server started\n" );
    }
    if ( m_status & JackNameNotUnique )
    {
        client_name = jack_get_client_name ( m_client );
        fprintf ( stderr, "unique name `%s' assigned\n", client_name );
    }

    audio_matrix = new AudioMatrix(SIMPLE_WFS_MIXER_CONFIG_PATH);

    // create input and output ports
    size_t n_input_channels = audio_matrix->get_n_input_channels();
    size_t n_output_channels = audio_matrix->get_n_output_channels(); 

    input_ports = ( jack_port_t** ) calloc ( n_input_channels, sizeof ( jack_port_t* ) );
    output_ports = ( jack_port_t** ) calloc ( n_output_channels, sizeof ( jack_port_t* ) );

    for (int i = 0; i < n_input_channels; i++ ) {
        std::string port_name = "input_" + std::to_string(i);
        input_ports[i] = jack_port_register (m_client, port_name.c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
        if( input_ports[i] == NULL )
        {
            fprintf ( stderr, "no more JACK ports available\n" );
            exit ( 1 );
        }
    }

    for (int i = 0; i < n_output_channels; i++ ) {
        std::string port_name = audio_matrix->get_ouput_port_name(i);
        output_ports[i] = jack_port_register (m_client, port_name.c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0 );
        if( output_ports[i] == NULL ) {
            fprintf ( stderr, "no more JACK ports available\n" );
            exit ( 1 );
        }
    }

    // tell the JACK server to call `process()' whenever there is work to be done.
    // before setting the process callback, we need to allocate the input and output ports. Why? this is not exactly clear to me. Is the method `process' called immediately after setting the process callback?
    jack_set_process_callback (m_client, process, this);

    // tell the JACK server to call `jack_shutdown()' if it ever shuts down, either entirely, or if it just decides to stop calling us.
    jack_on_shutdown (m_client, shutdown, this);

    // registers a function to be called when the maximum buffer size changes
    jack_set_buffer_size_callback(m_client, buffer_size_callback, this);

    // registers a function to be called when the sample rate changes
    jack_set_sample_rate_callback(m_client, sample_rate_callback, this);

}

JackClient::~JackClient() {
    jack_client_close(m_client);
    // ports need to be freed after the client is closed, otherwise we can run into setfaults
    free (input_ports);
    free (output_ports);
    delete audio_matrix;
    delete[] in;
    delete[] out;
    std::cout << "JackClient destructed!" << std::endl;
}

void JackClient::prepare() {
    prepare(HostAudioConfig(jack_get_buffer_size(m_client), jack_get_sample_rate(m_client)));
}

void JackClient::prepare(HostAudioConfig config) {
    // TODO: shall the client be deactivated before preparing?

    audio_matrix->prepare(config);

    size_t n_input_channels = audio_matrix->get_n_input_channels();
    size_t n_output_channels = audio_matrix->get_n_output_channels(); 

    delete[] in;
    delete[] out;

    // allocate memory for the input and output buffers
    // TODO can this be done more elegantly?
    in = new jack_default_audio_sample_t*[n_input_channels];
    out = new jack_default_audio_sample_t*[n_output_channels];
    for (int i = 0; i < n_input_channels; i++ ) {
        in[i] = new jack_default_audio_sample_t[config.m_host_buffer_size];
    }
    for (int i = 0; i < n_output_channels; i++ ) {
        out[i] = new jack_default_audio_sample_t[config.m_host_buffer_size];
    }

    // Tell the JACK server that we are ready to roll. Our process() callback will start running now.
    if (jack_activate(m_client)) {
        fprintf ( stderr, "cannot activate client" );
        exit ( 1 );
    }
}

int JackClient::process(jack_nframes_t nframes, void *arg) {
    JackClient* jack_client = static_cast<JackClient*>(arg);

    for (int i = 0; i < jack_client->audio_matrix->get_n_input_channels(); i++ ) {
        jack_client->in[i] = (jack_default_audio_sample_t *) jack_port_get_buffer(jack_client->input_ports[i], nframes);
    }
    for (int i = 0; i < jack_client->audio_matrix->get_n_output_channels(); i++ ) {
        jack_client->out[i] = (jack_default_audio_sample_t *) jack_port_get_buffer (jack_client->output_ports[i], nframes);
    }

    jack_client->audio_matrix->process((float**) jack_client->in, (float**) jack_client->out, (size_t) nframes);

    return 0;
}

void JackClient::shutdown(void *arg) {
    std::cout << "Jack server has shut down, exiting ..." << std::endl;
    delete static_cast<JackClient*>(arg);
}

int JackClient::buffer_size_callback(jack_nframes_t nframes, void *arg) {
    JackClient* jack_client = static_cast<JackClient*>(arg);
    jack_client->prepare(HostAudioConfig(nframes, jack_get_sample_rate(jack_client->m_client)));
    std::cout << "buffer size changed to " << nframes << std::endl;
    return 0;
}

int JackClient::sample_rate_callback(jack_nframes_t nframes, void *arg) {
    JackClient* jack_client = static_cast<JackClient*>(arg);
    jack_client->prepare(HostAudioConfig(jack_get_buffer_size(jack_client->m_client), nframes));
    std::cout << "sample rate changed to " << nframes << std::endl;
    return 0;
}
