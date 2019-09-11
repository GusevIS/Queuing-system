#ifndef QUEUINGSYSTEM_H
#define QUEUINGSYSTEM_H

#include <vector>
#include "buffer.h"
#include "device.h"
#include "selectionmanager.h"
#include "setmanager.h"
#include "source.h"

class QueuingSystem
{
public:
    QueuingSystem();

private:
    std::vector<Source> sources;
    std::vector<Device> devices;
    Buffer buffer;
    SelectionManager selectionManager;
    SetManager setManager;
};

#endif // QUEUINGSYSTEM_H
