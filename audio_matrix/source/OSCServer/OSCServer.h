#ifndef OSCSERVER_H
#define OSCSERVER_H

#include <iostream>
#include <lo/lo.h>
#include <lo/lo_cpp.h>

class OSCServer
{
public:
    OSCServer(int port = 58010);
    ~OSCServer();

    std::shared_ptr<lo::ServerThread> get_server_thread();

private:
    std::shared_ptr<lo::ServerThread> st;
    int port;
};

#endif // OSCSERVER_H