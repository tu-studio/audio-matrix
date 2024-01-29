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

std::shared_ptr<lo::ServerThread> OSCServer::get_server_thread() {
    return st;
}