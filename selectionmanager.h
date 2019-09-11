#ifndef SELECTIONMANAGER_H
#define SELECTIONMANAGER_H

#include <vector>
#include "device.h"
#include "buffer.h"

class SelectionManager
{
public:
    SelectionManager(const Buffer *newBuffer,const std::vector<Device> *devices);
    void chooseDevice();
    void selectRequest();

private:
    Buffer *buffer;
    std::vector<Device> *devicesVector;

};

#endif // SELECTIONMANAGER_H
