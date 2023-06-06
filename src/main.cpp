#include<iostream>
#include<vector>
#include<fstream>
#include<string.h>
#include<sstream>
#include<map>
#include<filesystem>
#include"parser/parser.hpp"
#include"parser/Ast.hpp"
#include"lex/lex.hpp"
#include"../../include/utils/File.hpp"
#include"../../include/Module.hpp"

using namespace module;
void compile(){
  
}

int main(int argc, char *argv[]){
    // if(argc < 0){
    //     printf("too few args");
    //     return EXIT_FAILURE;
    // }else{
      std::string path;
      ModuleInfo::CreateModInfoByAbsPath(path);
      compile();
         
   // }
}