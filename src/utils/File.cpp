#include<iostream>
#include<filesystem>
#include<fcntl.h>
#include<libgen.h>
#include<dirent.h>
#include"../../include/utils/File.hpp"
namespace fs{

bool get_src(std::string &_path, std::string &src) {
    char buffer[256];
    FILE* file;
    file = fopen(_path.c_str(), "r");
    if(file == NULL) {
        return false;
    }
    while(fgets(buffer, sizeof(buffer), file) != NULL) {
        src += buffer;
    }
    
    ///@todo line count

    fclose(file);
    if(src.empty()){
        fprintf(stderr, "Error: encountered empty file: %s\n", _path.c_str());
        return false;
    }

    return true;
} 


char* get_dir_name(std::string &path) {
    char *p = (char*)path.c_str();
    return dirname(p);
}

char* get_base_name(std::string &path) {
    char *p = (char*)path.c_str();
    return dirname(p);
}


bool get_root_mod_src(std::string &path, std::string &src, std::string &dirp, std::string &modname) {
    dirp = get_dir_path(path);
    modname = get_base_name(path);
    if(dirp.empty()){
        return false;
    }
    if(!get_src(path,src)){
        return false;
    }
    return true;
}



bool get_sub_mod_src(std::string &path, std::string &modname, std::string &src, std::string &dirp, std::string &absp) {
    //dog/
    //dog/mod.mt
    std::string modpath1 = path.append("/"+modname+"/mod.mt");
    std::string modpath2 = path.append("/"+modname+".mt");
    bool f1 = false,f2 = false;
    if(!(f1 = get_src(modpath1, src))&&!(f2 = get_src(modpath2, src))){
        fprintf(stderr, "error: no module find in scope - %s\n", modname);
        return false;
    }
    if(f1){
        dirp = get_dir_path(modpath1);
        absp = modpath1;
    }else if(f2){
        dirp = get_dir_path(modpath2);
        absp = modpath2;
    }
    return true;
}


bool find_mod(std::string path, std::string modname, std::string &fdpath, std::string &modmtfile) {
    DIR *d;
    struct dirent *dir;
    bool isDir = false;
    bool isfile = false;

    d = opendir(path.c_str());
    if(d == NULL){
        ///@todo can't open directory;
        return false;
    }
    int countf = 0;
    int countd = 0;
    
    while((dir = readdir(d))!=NULL){
        if(strcmp(dir->d_name,modmtfile.c_str()) == 0){
            isfile = true;
            countf++;
        }
        if(strcmp(dir->d_name,modname.c_str()) == 0){
            isDir = true;
            countd++;
        }
    }
    if(!isDir&&!isfile){
        ///@todo there is no such file or dir
        return false;
    }
    if(countf > 1 || countd > 1){
        ///@todo there is more than one file or dir
        return false;
    }
    return true;
}
}