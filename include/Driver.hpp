#ifndef DRIVER_H
#define DRIVER_H

namespace driver{

class FrontendDriver{
private:
    
public:
    FrontendDriver();
    ~FrontendDriver();

    bool lexSrc();
    bool parseToken();
};


}

#endif