#include <OSCServer.h>

OSCServer::OSCServer(int port) {
    this->port = port;
    try { 
        st = std::make_shared<lo::ServerThread>(port);
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}

OSCServer::~OSCServer() {
    st->stop();
    st.reset();
}

void OSCServer::start() {
    std::cout << "OSCServer Listening on port " << port << std::endl;
    st->start();
}

std::shared_ptr<lo::ServerThread> OSCServer::get_server_thread() {
    return st;
}