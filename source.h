#ifndef SOURCE_H
#define SOURCE_H

struct RequestNumber
{
    int sourceNumber;
    int serialRequestNumber;
};

class Source
{
public:
    Source();

private:
    RequestNumber requestNumber;
    int generationTime;
};

#endif // SOURCE_H
