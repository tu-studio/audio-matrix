/** @file thru_client.c
 *
 * @brief This simple through client demonstrates the basic features of JACK
 * as they would be used by many applications.
 */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <iostream>
#include <jack/jack.h>
#include <lo/lo.h>
#include <lo/lo_cpp.h>
#include <Track.h>
#include <algorithm>
#include <OSCServer.h>
#include <Gain.h>
#include <AudioMatrix.h>

jack_port_t **input_ports;
jack_port_t **output_ports;
jack_client_t *client;
std::shared_ptr<Track> wfs;
std::shared_ptr<OSCServer> osc_server;

static void signal_handler ( int sig )
{
    jack_client_close ( client );
    fprintf ( stderr, "signal received, exiting ...\n" );
    exit ( 0 );
}

/**
 * The process callback for this JACK application is called in a
 * special realtime thread once for each audio cycle.
 *
 * This client follows a simple rule: when the JACK transport is
 * running, copy the input port to the output.  When it stops, exit.
 */

// int
// process ( jack_nframes_t nframes, void *arg )
// {
    // int i;
    // jack_default_audio_sample_t **in, **out;

    // for ( i = 0; i < INPUT_PORTS; i++ ) {
    //     in[i] = (jack_default_audio_sample_t *) jack_port_get_buffer ( input_ports[i], nframes );
    // }
    // for ( i = 0; i < OUTPUT_PORTS; i++ ) {
    //     out[i] = (jack_default_audio_sample_t *) jack_port_get_buffer ( output_ports[i], nframes );
    // }

    // wfs->process((float**) in, (float**) out, (size_t) nframes);

    //     memcpy ( out, wfs->get_buffer().get_channel_pointer(i), nframes * sizeof ( jack_default_audio_sample_t ) );


    // return 0;
// }

// int buffer_size_callback(jack_nframes_t nframes, void *arg) {
//     std::cout << "buffer size changed" << std::endl;
//     wfs->get_buffer().clear();
//     wfs->initialize(INPUT_PORTS, nframes);
//     return 0;
// }

/**
 * JACK calls this shutdown_callback if the server ever shuts down or
 * decides to disconnect the client.
 */
void
jack_shutdown ( void *arg )
{
    free ( input_ports );
    free ( output_ports );
    exit ( 1 );
}


void cleanup () {
    std::cout << "cleanup" << std::endl;
}

int
main ( int argc, char *argv[] )
{
    std::cout << "This is the simple WFS config path " << SIMPLE_WFS_MIXER_CONFIG_PATH << std::endl;
    int i;
    const char **ports;
    const char *client_name;
    const char *server_name = NULL;
    int options = JackNullOption;
    jack_status_t status;

    // strrchr: locate last occurrence of character in string
    client_name = strrchr ( argv[0], '/' );
    if ( client_name == 0 ) {
        client_name = argv[0];
    }
    else {
        client_name++;
    }

    /* open a client connection to the JACK server */

    client = jack_client_open ( client_name, (jack_options_t) options, &status, server_name );
    if ( client == NULL )
    {
        fprintf ( stderr, "jack_client_open() failed, "
                  "status = 0x%2.0x\n", status );
        if ( status & JackServerFailed )
        {
            fprintf ( stderr, "Unable to connect to JACK server\n" );
        }
        exit ( 1 );
    }
    if ( status & JackServerStarted )
    {
        fprintf ( stderr, "JACK server started\n" );
    }
    if ( status & JackNameNotUnique )
    {
        client_name = jack_get_client_name ( client );
        fprintf ( stderr, "unique name `%s' assigned\n", client_name );
    }

    /* tell the JACK server to call `process()' whenever
       there is work to be done.
    */

    AudioMatrix* audioMatrix = new AudioMatrix(SIMPLE_WFS_MIXER_CONFIG_PATH);
    
    // jack_set_process_callback ( client, process, audioMatrix );

    /* tell the JACK server to call `jack_shutdown()' if
       it ever shuts down, either entirely, or if it
       just decides to stop calling us.
    */

    jack_on_shutdown ( client, jack_shutdown, 0 );

    // registers a function to be called when the maximum buffer size changes
    // jack_set_buffer_size_callback(client, buffer_size_callback, audioMatrix);

    audioMatrix->initialize();

    audioMatrix->prepare(HostAudioConfig(jack_get_sample_rate(client), jack_get_buffer_size(client)));

    /* create two ports pairs*/
    // call
    size_t n_input_channels = audioMatrix->get_n_input_channels();
    size_t n_output_channels = audioMatrix->get_n_output_channels(); 

    input_ports = ( jack_port_t** ) calloc ( n_input_channels, sizeof ( jack_port_t* ) );
    output_ports = ( jack_port_t** ) calloc ( n_output_channels, sizeof ( jack_port_t* ) );

    for ( i = 0; i < n_input_channels; i++ )
    {
        std::string port_name = "input_" + std::to_string(i);
        input_ports[i] = jack_port_register ( client, port_name.c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0 );
        if( input_ports[i] == NULL )
        {
            fprintf ( stderr, "no more JACK ports available\n" );
            exit ( 1 );
        }
    }

    for ( i = 0; i < n_output_channels; i++ )
    {
        std::string port_name = audioMatrix->get_ouput_port_name(i);
        output_ports[i] = jack_port_register ( client, port_name.c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0 );
        if( output_ports[i] == NULL ) {
            fprintf ( stderr, "no more JACK ports available\n" );
            exit ( 1 );
        }
    }

    /* Tell the JACK server that we are ready to roll.  Our
     * process() callback will start running now. */

    if (jack_activate(client)) {
        fprintf ( stderr, "cannot activate client" );
        exit ( 1 );
    }

    /* Connect the ports.  You can't do this before the client is
     * activated, because we can't make connections to clients
     * that aren't running.  Note the confusing (but necessary)
     * orientation of the driver backend ports: playback ports are
     * "input" to the backend, and capture ports are "output" from
     * it.
     */

    // ports = jack_get_ports ( client, NULL, NULL, JackPortIsPhysical|JackPortIsOutput );
    // if ( ports == NULL )
    // {
    //     fprintf ( stderr, "no physical capture ports\n" );
    //     exit ( 1 );
    // }

    // for ( i = 0; i < 2; i++ )
    //     if ( jack_connect ( client, ports[i], jack_port_name ( input_ports[i] ) ) )
    //         fprintf ( stderr, "cannot connect input ports\n" );

    // free ( ports );

    // ports = jack_get_ports ( client, NULL, NULL, JackPortIsPhysical|JackPortIsInput );
    // if ( ports == NULL )
    // {
    //     fprintf ( stderr, "no physical playback ports\n" );
    //     exit ( 1 );
    // }

    // for ( i = 0; i < 2; i++ )
    //     if ( jack_connect ( client, jack_port_name ( output_ports[i] ), ports[i] ) )
    //         fprintf ( stderr, "cannot connect input ports\n" );

    // free ( ports );

    /* install a signal handler to properly quits jack client */
#ifdef WIN32
    signal ( SIGINT, signal_handler );
    signal ( SIGABRT, signal_handler );
    signal ( SIGTERM, signal_handler );
#else
    signal ( SIGQUIT, signal_handler );
    signal ( SIGTERM, signal_handler );
    signal ( SIGHUP, signal_handler );
    signal ( SIGINT, signal_handler );
#endif

    atexit(cleanup);
    
    /* keep running until the transport stops */

    while (1)
    {
#ifdef WIN32
        Sleep ( 1000 );
#else
        sleep ( 1 );
#endif
    }

    jack_client_close ( client );
    exit ( 0 );
}