#pragma once

#include"../parser/Ast.hpp"
// #include"parser/Ast.hpp"
namespace ast{
class AstVisitor{
public:
    virtual ~AstVisitor() = default;

protected:
    virtual bool visit(BlockStmt *AstNode){ return false; }
    virtual bool visit(FunctionDef *AstNode ){ return false; }
    virtual bool visit(WhileLoop *AstNode ){ return false; }
    virtual bool visit(ForLoop *AstNode ){ return false; };
    virtual bool visit(StructStmt *AstNode ){ return false; }
    virtual bool visit(Method *AstNode ){ return false; }
    virtual bool visit(BranchStmt *AstNode ){ return false; }
    virtual bool visit(VarStmt *AstNode ){ return false; }
    virtual bool visit(IfStmt *AstNode ){ return false; }
    virtual bool visit(ReturnStmt *AstNode ){ return false; }
    
    virtual bool visit(GroupedExpr *AstNode ){ return false; }
    virtual bool visit(Expression *AstNode ){ return false; }
    virtual bool visit(ListExpr *AstNode ) { return false; };
    virtual bool visit(FunctionCall *AstNode ){ return false; }
    virtual bool visit(Expression *AstNode ){ return false; }
    virtual bool visit(PrefixExpr *AstNode ){ return false; }
    virtual bool visit(PostfixExpr *AstNode ){ return false; }
    
    virtual bool visit(Array *AstNode ) { return false; }
    virtual bool visit(FnType *AstNode ) { return false; }
    virtual bool visit(PreDefineType *AstNode ){ return false; }
    virtual bool visit(Identifier *AstNode ){ return false; }
    

    virtual bool visit(FloatLiteral *AstNode ){ return false; }
    virtual bool visit(BoolLiteral *AstNode ){ return false; }
    virtual bool visit(NumericLiteral *AstNode ){ return false; }
    virtual bool visit(NullLiteral *AstNode ){ return false; }
    virtual bool visit(StringLiteral *AstNode ){ return false; }
    
    //virtual bool visit(const TypeState   *AstNode ){ return false; }
    //virtual bool visit(const EnumLitral   *AstNode ){ return false; }
};

}