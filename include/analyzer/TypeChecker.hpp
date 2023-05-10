#pragma once
#include"analyzer/AstVisitor.hpp"
#include"parser/type.hpp"
#include"analyzer/symbol_table.hpp"

namespace analyzer{

using namespace ast;
using TablePtr = std::shared_ptr<SymTable<TypePtr>>;

class TypeChecker: public AstVisitor {
private:
    TablePtr Table;
    TypePtr Type;
    TypePtr retype = NULL;
    TypePtr ResType;
    bool isInField = false;
    std::string identifier(const AstPtr& iden);
    template<typename T>
    T dynamicPtrCast(const AstPtr& iden);
    TablePtr newTable(TablePtr globtable);
    bool isValid(const TypePtr &type1, const TypePtr &type2);
    bool checkTuple(TypePtr type, TypePtr node);
    bool checkMemExpr(MemberExpr& AstNode);

    bool checkStructExpr(MemberExpr node);
    bool tupleAccessExpr(MemberExpr node);

    bool visit(const Program& AstNode);
    bool visit(const BlockStatement& AstNode );
    bool visit(const FunctionDef& AstNode );
    bool visit(const WhileLoop& AstNode );
    bool visit(const ForInLoop& AstNode );
    bool visit(const StructStmt& AstNode );
    bool visit(const BranchStmt& AstNode );
    bool visit(const Tuple& AstNode );
    bool visit(const VarStmt& AstNode );
    bool visit(const IfStatement& AstNode );
    bool visit(const ReturnStmt& AstNode );
    
    bool visit(const BineryExpr& AstNode );
    bool visit(const AssignmentExpr& AstNode );
    bool visit(const ListExpr& AstNode ) ;
    bool visit(const FunctionCall& AstNode );
    bool visit(const Expression& AstNode );
    bool visit(const IndexExpr& AstNode );
    bool visit(const PrefixExper& AstNode );

    bool visit(const PointerExpr& AstNode );
    bool visit(const ArrayType& AstNode ) ;
    bool visit(const PreDefineType& AstNode );
    bool visit(const Identifier& AstNode );


    bool visit(const FloatLiteral& AstNode );
    bool visit(const BoolLiteral& AstNode );
    bool visit(const NumericLiteral& AstNode );
    bool visit(const NullLiteral& AstNode );
    bool visit(const StringLiteral& AstNode );
public:
    TypeChecker();
};
}
