#ifndef MODULE_MANAGER_H
#define MODULE_MANAGER_H
#include<iostream>
#include<map>
#include<stack>
#include"Driver.hpp"
#include"parser/Ast.hpp"
#include"parser/Parser.hpp"
#include"utils/File.hpp"
#include"analyzer/TypeChecker.hpp"
namespace module{



/*
    lib.mt
        |           |---- mod3
        |---- mod2--|
        |           |---- mod4
        |
        |           |---- mod5
        |---- mod2--|
                    |---- mod6
                    
*/


class ModuleItems{
private:
    std::map<std::string, ast::Ast*>moditem;
public:
    ModuleItems();
    ~ModuleItems();

    bool addItem(std::string s, ast::Ast *&node);
    ast::Ast* getItem(std::string &n) { 
        return moditem.find(n) == moditem.end()? 0: moditem[n];
    }
};

struct ModuleInfo{
    std::string path;
    std::string dirpath;
    std::string src;
    std::string modname;
};


class SubMod{
private:
    std::map<std::string, Module>mod;
public:
    SubMod();
    ~SubMod();

    bool addSubMod();
    Module* getMod(std::string &n) { 
        return mod.find(n) == mod.end()? 0: &mod[n];
    }
};


class Module{
private:
    std::map<std::string, Module>submods;    
    ModuleInfo modinfo;
    std::vector<lex::Lexeme>lexeme;
    ast::Ast* tree;
    ModuleItems moditems;
public:
    Module();
    Module(std::map<std::string, Module>&_submods,
            ModuleInfo &_modinfo,
            ast::Ast *&_tree,
            ModuleItems &_moditems)
    :submods(_submods),modinfo(_modinfo),tree(_tree),moditems(_moditems){}

    ~Module();
    std::map<std::string, Module> getSubMods() const { return submods; }
    Module* getSubMod(std::string &s) const;

    /// module info
    std::string getDirPath() const { return modinfo.dirpath; }
    std::string getPath() const { return modinfo.path; }
    std::string getSrc() const { return modinfo.src; }

    bool TypePass() {
        analyzer::TypeChecker typecheck(*this);
        if(typecheck.CheckType()){
            ///@todo
            return false;
        }
    }
    std::string getModId() const { return modinfo.modname; }
    ast::Ast* getModItems(std::string &s) const;
};  




struct Mod{
    std::map<std::string, Module>submod;
    ModuleInfo modinfo;
    ast::Ast* tree;
    std::vector<lex::Lexeme>toks;
    ModuleItems moditem;
};



class ModuleBuilder{
private:
    Mod M;
public:
    ModuleBuilder();
    ~ModuleBuilder();

    bool LexSrc() {
        Lexer L;
        int i = M.modinfo.src.size();
        if(!L.tokenizer(M.modinfo.src, M.toks, 0, i)){
            return false;
        }
        true;
    }
    bool ParseToken() {
        parser::Parser p(M);
        if(!p.parse()){
            return false;
        }
        return true;
    }
    bool buildMod(std::string &p, std::string modname, Module &mod){
        if(CreateModInfoByMod(p, modname)) {
            return false;
        }
        if(!LexSrc()){
            return false;
        }
        if(!ParseToken()){
            return false;
        }
        Module temp_mod(M.submod, M.modinfo, M.tree, M.moditem);
        mod = std::move(temp_mod);   
    }

    bool buildMainMod(std::string &p,Module &mod){
        if(!CreateModInfoByAbsPath(p)){
            return false;
        }
        if(M.modinfo.src.size() == 0){
            return false;
        }
        if(!LexSrc()){
            return false;
        }
        if(!ParseToken()){
            return false;
        }
        Module temp_mod(M.submod, M.modinfo, M.tree, M.moditem);
        mod =  std::move(temp_mod);
    }

    bool CreateModInfoByMod(std::string &path, std::string &modname) {
        std::string src = "";
        std::string dir = path;
        std::string absp = path;
        if(!fs::get_sub_mod_src(path, modname, src, dir, absp)){
            return false;
        }

        M.modinfo = (ModuleInfo){absp,dir,src,modname};
    }

    bool CreateModInfoByAbsPath(std::string &path) {
        std::string src = "";
        std::string dir = path;
        std::string absp = path;
        std::string modname = "";
        if(!fs::get_root_mod_src(path, src, dir, modname)){
            return false;
        }

        M.modinfo = (ModuleInfo){absp,dir,src, modname};
    }

};


}

#endif