#define _DEBUG

#include <ctime>
#include "queuingsystem.hpp"

int main(int argc, char *argv[])
{
    srand(static_cast<unsigned int>(time(0)));
    QueuingSystem queuingSystem(4, 3, 3, 3, 8);

#ifdef _DEBUG
    std::cout << "system online" << std::endl;
#endif

    queuingSystem.startSystem();

#ifdef _DEBUG
    std::cout << "system offline" << std::endl;
#endif

    return 0;
}
