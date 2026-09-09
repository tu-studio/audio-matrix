#include <OSCServer.h>
#include <cstdlib>
#include <iostream>

void osc_error_handler(int num, const char *msg, const char *path) {
    std::cerr << "liblo error " << num << " in " << path << ": " << msg << std::endl;
}

OSCServer::OSCServer(int port) : port(port) {
    st = std::make_shared<lo::ServerThread>(port, osc_error_handler);

    if (!st->is_valid()) {
        std::cerr << "OSC Server initialization failed for port " << port << std::endl;
        std::cerr << "Aborting" << std::endl;
        std::abort();
    }
}

OSCServer::~OSCServer() {
    // TODO: hier scheint es manchmal probleme beim destructen zu geben
    st->stop();
    st.reset();
}

void OSCServer::start() {
    std::cout << "[info] OSCServer Listening on port " << port << std::endl;
    st->start();
}

std::shared_ptr<lo::ServerThread> OSCServer::get_server_thread() {
    return st;
}
