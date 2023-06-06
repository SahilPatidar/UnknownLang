#pragma once
#include"../../include/analyzer/Validator.hpp"

namespace ast{

bool Validator::visit(BlockStmt *AstNode) {
    for(int i = 0, siz = AstNode->getStmts().size(); i < siz; i++) {
        switch (AstNode->getStmts()[i]->nodeCategory())
        {
        case NodeStructStm:
        case NodeEnum:
        case NodeImpl:
        case NodeExtern:
        case NodeFNStm:
        case NodeImport:
        case NodeTypeStm:
        {
            if(!AstNode->getStmts()[i]->accept(*this)){
                return false;
            }
        }
            break;
        default:
            ///@todo
            break;
        }
    }
    return true;
}

bool Validator::visit(FunctionDef *AstNode ) {
    
}

bool Validator::visit(WhileLoop *AstNode ) {

}

bool Validator::visit(ForLoop *AstNode ) {

}

bool Validator::visit(StructStmt *AstNode ) {

}

bool Validator::visit(Method *AstNode ) {

}

bool Validator::visit(BranchStmt *AstNode ) {
    return true;
}

bool Validator::visit(VarStmt *AstNode ) {
    if(!AstNode->getVarName()->accept(*this)){
        return false;
    }
    if(!AstNode->getType()->accept(*this)){
        return false;
    }
    if(!AstNode->getVal()->accept(*this)){
        return false;
    }
    return true;
}

bool Validator::visit(IfStmt *AstNode ) {
    if(!AstNode->getCondV()->accept(*this)){
        return false;
    }
    if(!AstNode->getIfBlock()->accept(*this)){
        return false;
    }
    if(!AstNode->getElBlock()->accept(*this)){
        return false;
    }
    return true;
}

bool Validator::visit(ReturnStmt *AstNode ) {
    if(!AstNode->getRetValue()->accept(*this)){
        return false;
    }
    return true;
}


bool Validator::visit(GroupedExpr *AstNode ) {
    if(!AstNode->getExpression()->accept(*this)){
        return false;
    }
    return true;

}

bool Validator::visit(Expression *AstNode ) {
    switch (AstNode->getOp().getTokType())
    {
    case PLUS:
    case MINUS:
    case STAR:
    case MOD:
    case DIV:
    {
        switch (AstNode->getLhs()->nodeCategory())
        {
        case NodeGroupExpr:
        case NodeAsExpr:
        case NodeBinaryExpr:
        case NodeIndexExpr:
        case NodeIdent:
        case NodeCallExpr:
        case NodeNumLit:
        case NodeFloatLit:
        case NodeMemExpr:
        case NodeStrLit:
        case NodeDeref:
        {
            if(!AstNode->getLhs()->accept(*this)){
                return false;
            }
        }
            break;
        
        default:
            break;
        }
        switch (AstNode->getRhs()->nodeCategory())
        {
        case NodeGroupExpr:
        case NodeAsExpr:
        case NodeBinaryExpr:
        case NodeIndexExpr:
        case NodeIdent:
        case NodeCallExpr:
        case NodeNumLit:
        case NodeFloatLit:
        case NodeMemExpr:
        case NodeStrLit:
        case NodeDeref:
        {
            if(!AstNode->getLhs()->accept(*this)){
                return false;
            }
        }
            break;
        
        default:
            break;
        }
    }
        break;
    case AS:
    {
        switch (AstNode->getLhs()->nodeCategory())
        {
        case NodeGroupExpr:
        case NodeBinaryExpr:
        case NodeIndexExpr:
        case NodeIdent:
        case NodeCallExpr:
        case NodeMemExpr:
        case NodeNumLit:
        case NodeDeref:
        case NodeFloatLit:
        {
            if(!AstNode->getLhs()->accept(*this)){
                return false;
            }
        }
            break;
        
        default:
            break;
        }
        switch (AstNode->getRhs()->nodeCategory())
        {
        case NodeGroupExpr:
        case NodeAsExpr:
        case NodeIdent:
        case I8:
        case I16:
        case I32:
        case I64:
        case I128:
        case UI8:
        case UI16:
        case UI32:
        case UI64:
        case UI128:
        case F32:
        case F64:
        case BOOL:
        {
            if(!AstNode->getLhs()->accept(*this)){
                return false;
            }
        }
            break;
        
        default:
            break;
        }
    }
        break;
    case DOT:
    case ARROW:
    {
        switch (AstNode->getLhs()->nodeCategory())
        {
        case NodeGroupExpr:
        case NodeBinaryExpr:
        case NodeIndexExpr:
        case NodeIdent:
        case NodeCallExpr:
        case NodeMemExpr:
        {
            if(!AstNode->getLhs()->accept(*this)){
                return false;
            }
        }
            break;
        
        default:
            break;
        }
        switch (AstNode->getRhs()->nodeCategory())
        {
        case NodeGroupExpr:
        case NodeAsExpr:
        case NodeIdent:
        case I8:
        case I16:
        case I32:
        case I64:
        case I128:
        case UI8:
        case UI16:
        case UI32:
        case UI64:
        case UI128:
        case F32:
        case F64:
        case BOOL:
        {
            if(!AstNode->getLhs()->accept(*this)){
                return false;
            }
        }
            break;
        
        default:
            break;
        }
    }
        break;
    case COLCOL:
    {

    }
        break;
    
    default:
        break;
        return false;
    }
    return true;
}

bool Validator::visit(ListExpr *AstNode ) {

}

bool Validator::visit(FunctionCall *AstNode ) {

}

bool Validator::visit(Expression *AstNode ) {

}

bool Validator::visit(PrefixExpr *AstNode ) {
    switch (AstNode->getOp().getTokType())
    {
    case STAR:
    {
        switch (AstNode->getBase()->nodeCategory())
        {
        case NodeIndexExpr:
        case NodeGroupExpr:
        case NodeIdent:
        case NodeCallExpr:
        case NodeMemExpr:
        {
            if(!AstNode->getBase()->accept(*this)){
                return false;
            }
        }
            break;
        
        default:
            ///@todo
            break;
        }
    }
        break;
    case AND:
    {
        switch (AstNode->getBase()->nodeCategory())
        {
        case NodeListExpr:
        case NodeIndexExpr:
        case NodeIdent:
        case NodeCallExpr:
        case NodeMemExpr:
        case NodeGroupExpr:
        {
            if(!AstNode->getBase()->accept(*this)){
                return false;
            }
        }
            break;
        
        default:
            break;
        }
    }
        break;
    case PLUS:
    case MINUS:
    {
        switch (AstNode->getBase()->nodeCategory())
        {
        case NodeFloatLit:
        case NodeNumLit:
        case NodeIdent:
        case NodeCallExpr:
        case NodeMemExpr:
        case NodeIndexExpr:
        case NodeGroupExpr:
        {
            if(!AstNode->getBase()->accept(*this)){
                return false;
            }
        }
            break;
        
        default:
            break;
        }
    }
        break;
    default:
        break;
    }
}

bool Validator::visit(PostfixExpr *AstNode ) {

}


bool Validator::visit(Array *AstNode )  {

}

bool Validator::visit(FnType *AstNode )  {

}

bool Validator::visit(PreDefineType *AstNode ) {
    return true;
}

bool Validator::visit(Identifier *AstNode ) {
    return true;
}



bool Validator::visit(FloatLiteral *AstNode ) {
    return true;
}

bool Validator::visit(BoolLiteral *AstNode ) {
    return true;
}

bool Validator::visit(NumericLiteral *AstNode ) {
    return true;
}

bool Validator::visit(NullLiteral *AstNode ) {
    return true;
}

bool Validator::visit(StringLiteral *AstNode ) {
    return true;

}



}
