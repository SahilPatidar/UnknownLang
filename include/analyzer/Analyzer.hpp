#pragma once

#include<iostream>
#include"analyzer/AstVisitor.hpp"
namespace analyzer{
using namespace ast;
class AstAnalyzer: public AstVisitor {
    private:
    bool mainFunc = false;
    bool checkType(ast::NodeCategory node);
    bool visit(const Program& astnode);
    bool visit(const BlockStatement& astnode );
    bool visit(const FunctionDef& astnode );
    bool visit(const ForInLoop& astnode );
    bool visit(const WhileLoop& astnode );
    bool visit(const StructStmt& astnode );
    bool visit(const BranchStmt& astnode );
    bool visit(const VarStmt& astnode );
    bool visit(const IfStatement& astnode );
    bool visit(const ReturnStmt& astnode );

    bool visit(const BineryExpr& astnode );
    bool visit(const AssignmentExpr& astnode );
    bool visit(const ListExpr& astnode ) ;
    bool visit(const FunctionCall& astnode );
    bool visit(const MemberExpr& astnode );
    bool visit(const IndexExpr& astnode );
    bool visit(const PrefixExper& astnode );
    bool visit(const Parameter& astnode );

    bool visit(const PointerExpr& astnode );
    bool visit(const ArrayType& astnode ) ;
    bool visit(const Tuple& astnode ) ;
    bool visit(const PreDefineType& astnode );
    bool visit(const Identifier& astnode );

    bool visit(const FloatLiteral& astnode );
    bool visit(const BoolLiteral& astnode );
    bool visit(const NumericLiteral& astnode );
    bool visit(const NullLiteral& astnode );
    bool visit(const StringLiteral& astnode );

// bool visit(const TypeState& astnode );
// bool visit(const EnumLitral& astnode );
// bool visit(const NewState& astnode );
// bool visit(const FreeState& astnode );
};

}