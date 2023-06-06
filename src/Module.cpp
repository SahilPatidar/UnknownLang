#include"../include/Module.hpp"
#include"../include/utils/File.hpp"
namespace module{




/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// ModuleInfo //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
ModuleInfo CreateModInfoByMod(std::string &path, std::string &modname) {
    std::string src = "";
    std::string dir = path;
    std::string absp = path;
    if(!fs::get_sub_mod_src(path, modname, src, dir, absp)){
        return {};
    }

    return ModuleInfo(absp,dir,src);
}

ModuleInfo CreateModInfoByAbsPath(std::string &path) {
    std::string src = "";
    std::string dir = path;
    std::string absp = path;
    if(!fs::get_root_mod_src(path, src, dir)){
        return {};
    }

    return ModuleInfo(absp,dir,src);
}


}