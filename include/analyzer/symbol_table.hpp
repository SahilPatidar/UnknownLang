#pragma once
#include<map>
#include<optional>
#include"../parser/type.hpp"
#include"Value.hpp"
// #include"parser/type.hpp"

using namespace ast;


// template<typename T>
// class SymTable{
//     private:
//     std::map<std::string,T>extrnt;
//     std::map<std::string,T>sym;
//     std::map<std::string,T>Structs;
//     SymTable<T>*p_table;
//     bool islocal = false;
//     public:
//     SymTable(SymTable<Type*>*&table);
//     bool insert(std::string val, T type) {

//         return true;
//     }
//     void setFlag(bool flag);

//     bool contain(std::string key, bool inloc) {

//         return false;
//     }
//     bool contains(std::string key) {

//         return false;
//     }

//     T get(std::string key) {

//     }

//     bool pop() {
//         local.clear();
//     }
// };

struct AstNodeInfo {
    Type* type;
    ast::VALUE* val;
    Ast* Node;
};

class StackVal{
private:
    std::map<std::string, AstNodeInfo>info;
public:
    StackVal();
    ~StackVal();

    bool add(std::string &n, AstNodeInfo &_info) {
        info.insert({n,_info});
    }
    AstNodeInfo* getAllInfo(std::string &n)  { 
        return info.find(n) == info.end()? 0: &info[n]; 
    }
    Type* getTypeInfo(std::string &n)  { 
        return info.find(n) == info.end()? 0: info[n].type; 
    }
    ast::VALUE* getValInfo(std::string &n)  { 
        return info.find(n) == info.end()? 0: info[n].val; 
    }
};

class Stack{
private:
    std::vector<StackVal*>stack;
public:
    Stack();
    ~Stack();

    bool push_back() { stack.emplace_back(); }
    bool pop_back() { stack.pop_back(); }
    bool empty() { stack.empty(); }
    bool add(std::string &n, Type *&type, ast::VALUE *&val, Ast *&Node) {
        stack.back()->add(n, (AstNodeInfo){type, val, Node});
    }
    StackVal* get_back() const { return stack.back()? stack.back(): 0; }
};

/// check undef and replace it;
/// impl grammer
/// assert

class Registry{
private:
    Stack localStack;
    std::vector<FunctionType*>func;
    std::map<std::string, AstNodeInfo> globalDecl;
public:
    Registry();
    ~Registry();
    
    void Push_Stack() {
        localStack.push_back();
    }
    void Pop_Stack() {
        localStack.pop_back();
    }
    void Push_Func(FunctionType *_func) {
        func.push_back(_func);
    }
    void Pop_Func() {
        func.pop_back();
    }
    bool addInfo(std::string n, Type *type, ast::VALUE *val, Ast* Node) {
        if(!localStack.empty()) {
            if(!localStack.add(n,type,val,Node)) {
                return false;
            }
        }else{
            globalDecl.insert({n,(AstNodeInfo){type,val,Node}});
        }
        return true;
    }
    void updateGlobalInfo(std::string n, Type *type, ast::VALUE *val, Ast* Node) {
        globalDecl[n] = (AstNodeInfo){type,val,Node};
    }
    bool ContainsWithNoDef(std::string n) {
        if(Decl.find(n) != Decl.end()){
            return true;
        }
        return false;
    }
    Type* getNoDefDecl(std::string n) {
        return globalDecl.at(n).type;
    }
    void setunDefDecl(std::string n, bool is){

    }
    Stack getStack() { return localStack; }
    FunctionType* getFuncBack() { return func.back(); }
    Type* getType(std::string n);
    Type* getGlobalFunc(std::string n);
    bool contains(std::string n);
    ast::VALUE* getVal(std::string n, bool fromGlobal, bool fromFunc);
    AstNodeInfo* getAstNodeinfo(std::string n, bool fromGlobal, bool fromFunc);
};





