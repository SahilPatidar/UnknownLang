#pragma once
#include<map>
#include<optional>
#include"../parser/type.hpp"
#include"Value.hpp"
// #include"parser/type.hpp"

using namespace ast;

const enum VarTy{
    TypeDecl,
    VarDecl,
    Lit
};
const enum State{
    Initialized,
    UnInitialized,
};

struct AstNodeInfo {
    Type* type;
    ast::VALUE* val;
    Ast* Node;
    VarTy instTy;
    State state;
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
    bool isempty() { info.empty(); }
    Type* getTypeInfo(std::string &n)  { 
        return info.find(n) == info.end()? 0: info[n].type; 
    }
    ast::VALUE* getValInfo(std::string &n)  { 
        return info.find(n) == info.end()? 0: info[n].val; 
    }
};

class Stack{
private:
    std::vector<StackVal>stack;
public:
    Stack();
    ~Stack();

    bool push_back() { stack.emplace_back(); }
    bool pop_back() { stack.pop_back(); }
    bool empty() { stack.empty(); }
    bool add(std::string &n, Type *&type, ast::VALUE *&val, Ast *&Node) {
        stack.back().add(n, (AstNodeInfo){type, val, Node});
    }
    bool Has(std::string &n);
    AstNodeInfo* get_all_info();
    Type* get_type();
    ast::VALUE* get_val();
    Ast* get_node();
};


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
    Stack getStack() { return localStack; }
    FunctionType* getFuncBack() { return func.back(); }
    Type* getType(std::string n);
    Type* getGlobalFunc(std::string n);
    bool contains(std::string n);
    ast::VALUE* getVal(std::string n, bool fromGlobal, bool fromFunc);
    AstNodeInfo* getAstNodeinfo(std::string n);
};





