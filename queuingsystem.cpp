#include "queuingsystem.h"

QueuingSystem::QueuingSystem()
{
    Source source;
    sources.push_back(source);
    selectionManager(&sources, &buffer);

}
