#pragma once
#include"AstVisitor.hpp"
#include"../parser/type.hpp"
#include"symbol_table.hpp"
#include"Value.hpp"
// #include"analyzer/AstVisitor.hpp"
// #include"parser/type.hpp"
// #include"analyzer/symbol_table.hpp"

using namespace ast;
namespace analyzer{

class Symbol{
private:
    Type *type;
    ast::VALUE *value;
public:
    Symbol(Type *&_type, ast::VALUE *&_value)
    :type(_type), value(_value) {}

};

// class Stack{
// private:
//     std::vector<std::string>VarId;
//     std::vector<Symbol*>Layer;
// public:
//     Stack();

//     void PushLayer(Symbol *&sym);
//     void PopLayer(Symbol *&sym);
// };

class Stack{
private:
    std::vector<std::string>VarId;
    std::vector<AstPtr>Stmt;
public:
    Stack();

    void PushStmt(Symbol *&sym);
    void PopStmt(Symbol *&sym);
};

class StaticAnalyzer: public AstVisitor {
private:
    Stack *StmtStack;
    
    bool visit(BlockStmt &AstNode );
    bool visit(FunctionDef& AstNode );
    bool visit(WhileLoop& AstNode );
    bool visit(ForLoop& AstNode );
    bool visit(StructStmt& AstNode );
    bool visit(EnumExpr &AstNode );
    bool visit(BranchStmt& AstNode );
    bool visit(VarStmt& AstNode );
    bool visit(IfStmt& AstNode );
    bool visit(ReturnStmt& AstNode );
    
    bool visit(GroupedExpr& AstNode );
    bool visit(Expression& AstNode );
    bool visit(ListExpr& AstNode ) ;
    bool visit(FunctionCall& AstNode );
    bool visit(PostfixExpr& AstNode );
    bool visit(PrefixExpr& AstNode );

    bool visit(Array& AstNode ) ;
    bool visit(PreDefineType& AstNode );
    bool visit(Identifier& AstNode );

    bool visit(FloatLiteral& AstNode );
    bool visit(BoolLiteral& AstNode );
    bool visit(NumericLiteral& AstNode );
    bool visit(NullLiteral& AstNode );
    bool visit(StringLiteral& AstNode );
public:
    StaticAnalyzer();
};



}


