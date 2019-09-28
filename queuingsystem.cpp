#include "queuingsystem.h"

QueuingSystem::QueuingSystem(int numberOfSources, int workIntensity, int numberOfDevices, int serviceIntensity, int bufferSize)
{
    Source source;
    sources.push_back(source);
    selectionManager(&sources, &buffer);

}
