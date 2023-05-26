#ifndef FRONT_END_H
#define FRONT_END_H
#include<iostream>
#include"../include/parser/Ast.hpp"
#include"../include/parser/type.hpp"
#include"../include/analyzer/Value.hpp"
#include<sstream>
#include<filesystem>
namespace compiler{
namespace frontend{

class InputFile{
private:
    std::ifstream call();
public:
    InputFile(std::string path);
    ~InputFile();

    std::string getSrcString();
    std::string getPath();

};

class ArgParser{
    
};

class ArgParser{

};


class Frontend{
private:

public:
    Frontend();
    ~Frontend();
};


class AstContext{
public:
    enum Check{
        Checked,
        NotChecked,
    };
private:
    ast::Ast* tree;
    std::map<std::string, ast::Ast*>stmts;
    std::map<std::string, Check>isChecked;
public:
    AstContext();
    ~AstContext();

};



}
}

#endif