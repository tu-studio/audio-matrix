#include <OSCReceiver.h>

OSCReceiver::OSCReceiver() : port(0) {
    std::cout << "OSCReceiver constructor: " << port << std::endl;
}

OSCReceiver::~OSCReceiver() {
    std::cout << "OSCReceiver destructor: " << port << std::endl;
}
