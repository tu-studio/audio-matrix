#ifndef OSCRECEIVER_H
#define OSCRECEIVER_H

#include <iostream>
#include <lo/lo.h>

class OSCReceiver
{
public:
    OSCReceiver();
    ~OSCReceiver();

private:
    int port;
};

#endif // OSCRECEIVER_H