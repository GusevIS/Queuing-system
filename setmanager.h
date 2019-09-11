#ifndef SETMANAGER_H
#define SETMANAGER_H

#include <vector>
#include "source.h"
#include "buffer.h"

class SetManager
{
public:
    SetManager(Buffer newBuffer, std::vector<Source> sourcesVector);
    void refuseRequest() const;
    void receiveRequest();

private:
    Buffer buffer;
    std::vector<Source> sources;
};

#endif // SETMANAGER_H
