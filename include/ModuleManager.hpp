#ifndef MODULE_MANAGER_H
#define MODULE_MANAGER_H
#include<iostream>
#include"Frontend.hpp"
namespace compiler::frontend{

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

class Module{
private:
    std::vector<AstContext>mods;
public:

};

class ModuleManager{
private:
    std::vector<AstContext>mods;
public:

};

}

#endif