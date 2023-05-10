#pragma once

#include"../parser/Ast.hpp"
// #include"parser/Ast.hpp"
namespace ast{
class AstVisitor{
    public:
    virtual ~AstVisitor() = default;

    virtual bool visit(const Program& astnode ){ return false; }
    virtual bool visit(const BlockStatement& astnode ){ return false; }
    virtual bool visit(const FunctionDef& astnode ){ return false; }
    virtual bool visit(const WhileLoop& astnode ){ return false; }
    virtual bool visit(const ForInLoop& astnode ){ return false; };
    virtual bool visit(const StructStmt& astnode ){ return false; }
    virtual bool visit(const BranchStmt& astnode ){ return false; }
    virtual bool visit(const Tuple& astnode ){ return false; }
    virtual bool visit(const VarStmt& astnode ){ return false; }
    virtual bool visit(const IfStatement& astnode ){ return false; }
    virtual bool visit(const ReturnStmt& astnode ){ return false; }
    
    virtual bool visit(const BineryExpr& astnode ){ return false; }
    virtual bool visit(const AssignmentExpr& astnode ){ return false; }
    virtual bool visit(const ListExpr& astnode ) { return false; };
    virtual bool visit(const FunctionCall& astnode ){ return false; }
    virtual bool visit(const Expression& astnode ){ return false; }
    virtual bool visit(const IndexExpr& astnode ){ return false; }
    virtual bool visit(const PrefixExper& astnode ){ return false; }
    virtual bool visit(const Parameter& astnode ){ return false; }
    
    virtual bool visit(const PointerExpr& astnode ){ return false; }
    virtual bool visit(const ArrayType& astnode ) { return false; }
    virtual bool visit(const PreDefineType& astnode ){ return false; }
    virtual bool visit(const Identifier& astnode ){ return false; }
    

    virtual bool visit(const FloatLiteral& astnode ){ return false; }
    virtual bool visit(const BoolLiteral& astnode ){ return false; }
    virtual bool visit(const NumericLiteral& astnode ){ return false; }
    virtual bool visit(const NullLiteral& astnode ){ return false; }
    virtual bool visit(const StringLiteral& astnode ){ return false; }
    
    //virtual bool visit(const TypeState& astnode ){ return false; }
    //virtual bool visit(const EnumLitral& astnode ){ return false; }
};

}