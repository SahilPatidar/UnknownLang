#ifndef MOD_H
#define MOD_H
#include"../Module.hpp"
using namespace module;
namespace modulebuilder{
    
bool getModule(Module &mod, std::string );
bool getSrc();
bool BuildModule();
bool LexSrc();
bool ParseToken();

}
#endif