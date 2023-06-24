#ifndef VALIDATOR_H
#define VALIDATOR_H

#include"AstVisitor.hpp"
// #include"parser/Ast.hpp"
namespace ast{
    
class Validator: public AstVisitor{
public:
    Validator();
    ~Validator();

    bool visit(BlockStmt *AstNode);
    bool visit(FunctionDef *AstNode );
    bool visit(WhileLoop *AstNode );
    bool visit(ForLoop *AstNode );;
    bool visit(StructStmt *AstNode );
    bool visit(Method *AstNode );
    bool visit(BranchStmt *AstNode );
    bool visit(VarStmt *AstNode );
    bool visit(IfStmt *AstNode );
    bool visit(ReturnStmt *AstNode );
    
    bool visit(GroupedExpr *AstNode );
    bool visit(Expression *AstNode );
    bool visit(ListExpr *AstNode ) ;;
    bool visit(FunctionCall *AstNode );
    bool visit(Expression *AstNode );
    bool visit(PrefixExpr *AstNode );
    bool visit(IndexExpr *AstNode );
    
    bool visit(Array *AstNode ) ;
    bool visit(FnType *AstNode ) ;
    bool visit(PreDefineType *AstNode );
    bool visit(Identifier *AstNode );
    

    bool visit(FloatLiteral *AstNode );
    bool visit(BoolLiteral *AstNode );
    bool visit(NumericLiteral *AstNode );
    bool visit(NullLiteral *AstNode );
    bool visit(StringLiteral *AstNode );
    
};

}

#endif