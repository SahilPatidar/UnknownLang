#include<iostream>
#include"../../include/parser/Ast.hpp"
#include"../../include/analyzer/AstVisitor.hpp"
namespace ast{


// bool BlockStmt::accept(AstVisitor &visitor) {
//     return visitor.visit(this);
// }

bool ForLoop::accept(AstVisitor &visitor) {
    return visitor.visit(this);
}

bool FunctionDef::accept(AstVisitor &visitor) {
    return visitor.visit(this);
}

bool VarStmt::accept(AstVisitor &visitor) {
    return visitor.visit(this);
}

bool BranchStmt::accept(AstVisitor &visitor) {
    return visitor.visit(this);
}

bool StructStmt::accept(AstVisitor &visitor) {
    return visitor.visit(this);
}


bool ReturnStmt::accept(AstVisitor &visitor) {
    return visitor.visit(this);
}

bool IfStmt::accept(AstVisitor &visitor) {
    return visitor.visit(this);
}


bool BoolLiteral::accept(AstVisitor &visitor) {
    return visitor.visit(this);
}

bool NumericLiteral::accept(AstVisitor &visitor) {
    return visitor.visit(this);
}

bool NullLiteral::accept(AstVisitor &visitor) {
    return visitor.visit(this);
}

bool StringLiteral::accept(AstVisitor &visitor) {
    return visitor.visit(this);
}

bool FloatLiteral::accept(AstVisitor &visitor) {
    return visitor.visit(this);
}


bool Expression::accept(AstVisitor &visitor) {
    return visitor.visit(this);
}

bool PostfixExpr::accept(AstVisitor &visitor) {
    return visitor.visit(this);
}


bool PrefixExpr::accept(AstVisitor &visitor) {
    return visitor.visit(this);
}


bool ListExpr::accept(AstVisitor &visitor) {
    return visitor.visit(this);
}

bool Array::accept(AstVisitor &visitor) {
    return visitor.visit(this);
}

bool FunctionCall::accept(AstVisitor &visitor) {
    return visitor.visit(this);
}
  
// bool IndexExpr::accept(AstVisitor &visitor) {
//     return visitor.visit(this);
// }

bool Extern::accept(AstVisitor &visitor) {
    return visitor.visit(this);
}
  
bool Method::accept(AstVisitor &visitor) {
    return visitor.visit(this);
}

bool PreDefineType::accept(AstVisitor &visitor) {
    return visitor.visit(this);
}


}