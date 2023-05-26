
#include"../../include/parser/Parser.hpp"
#include"../../include/parser/Ast.hpp"
#include"../../include/Error.hpp"
// #include"parser/Parser.hpp"
// #include"Error.hpp"

namespace parser
{
using namespace ast;


bool Parser::ParseBlock(ParserHelper &P, Ast *&ResultantNode, bool isBrace){
    std::vector<Ast*>stmts;
    if(isBrace&&!P.checkn(LBRACE)){
        err::out("expected '{' found",P.toks[P.CurrentIndex]);
        return false;
    }
    while(!P.check(FEOF)&&(!isBrace&&!P.check(RBRACE))) {
        bool isSemiCol = false;
        switch(P.CurrentToken){ 
        case IF:
            if(!ParseIfStmt(P, ResultantNode)){ return false; }
            break;
        case FOR:
            if(P.checkNH(IN, 2)){
                if(!ParseForInStmt(P, ResultantNode)){ return false; }
            }else{
                if(!ParseForStmt(P, ResultantNode)){ return false; }
            }
            break;
        case FN:
        {
            if(!ParseFuncDef(P, ResultantNode)){
                return false;
            }
        }
            break;
        case STRUCT:
        {
            if(!ParseStructStmt(P, ResultantNode)){
                return false;
            }
        }
            break;
        case METHOD:
        {
            if(!ParseMethod(P, ResultantNode)){
                return false;
            }
        }
            break;
        case ENUM:
        {
            if(!ParseEnumStmt(P, ResultantNode)){
                return false;
            }
        }
            break;
        case WHILE:
            if(!ParseWhileStmt(P, ResultantNode)){ return false;}
            break;
        case LET:
            if(!ParseVarStmt(P, ResultantNode)){ return false; }
            isSemiCol = true;
            break;

        case RETURN:
            if(!ParseReturnStmt(P, ResultantNode)){ return false; }
            isSemiCol = true;
            break;

        case CONTINUE:
        case BREAK:
            stmts.push_back(BranchStmt::Create(P.toks[P.CurrentIndex]));
            P.next();
            isSemiCol = true;
            break;
        case TYPE:
            if(!ParseTypeStatm(P, ResultantNode)){ return false; }
            isSemiCol = true;
            break;
        case IDEN:
            if(!ParseExpr(P, ResultantNode)) { return false;}
            isSemiCol = true;
            break;
        default:
            
            break;
        }
        if(isSemiCol&&!P.checkn(SCOL)){
            err::out("expected ';' found - ",P.toks[P.CurrentIndex]);
            return false;
        }
        stmts.push_back(ResultantNode);
    }
    if(isBrace&&!P.checkn(RBRACE)){
        err::out("expected '}' found",P.toks[P.CurrentIndex]);
        return false;
    }
    ResultantNode = BlockStmt::Create(stmts);
    return true;
}


bool Parser::ParseMethod(ParserHelper &P, Ast *&ResultantNode) {
    tokt tok = P.toks[P.CurrentIndex];
    Ast* Assoc;
    std::vector<Ast*> Impl;
    P.next();
    if(!P.check(IDEN)){
        err::out("expected 'identifier' with method statement found - ",P.toks[P.CurrentIndex]);
        return false;
    }
    tokt Name = P.toks[P.CurrentIndex];
    P.next();
    if(P.check(FOR)){
        ///@todo
    }
    if(!P.checkn(LBRACE)) {
        err::out("expected '{' found - ",P.toks[P.CurrentIndex]);
        return false;
    }

    while(!P.check(FN)) {
        if(!ParseFuncDef(P, ResultantNode)) {
            return false;
        }
        Impl.push_back(ResultantNode);
    }

    if(!P.checkn(RBRACE)) {
        err::out("expected '}' found - ",P.toks[P.CurrentIndex]);
        return false;
    }
    ResultantNode = Method::Create(tok, Name, Assoc, Impl);
    return true;
}


bool Parser::ParseWhileStmt(ParserHelper &P, Ast *&ResultantNode) {
    tokt tok = P.toks[P.CurrentIndex];
    Ast* Cond;
    P.next();
    if(P.check(LBRACE)){
        err::out("expected condition expression found - ",P.toks[P.CurrentIndex]);
        return false;
    }
    if(!ParseExpr(P, ResultantNode)) {
        return false;
    }
    Cond = ResultantNode;
    bool isBrace = true;
    if(!ParseBlock(P,ResultantNode,isBrace)){
        return false;
    }
    ResultantNode = WhileLoop::Create(tok,Cond,ResultantNode);
    return true;
}


bool Parser::ParseReturnStmt(ParserHelper &P, Ast *&ResultantNode) {
    P.dump(__func__);
    tokt tok = P.toks[P.CurrentIndex];
    Ast* val;
    P.next();

    if(!P.check(SCOL)){
        if(!ParseExpr(P, ResultantNode)) {
            return false;
        }
        val = ResultantNode;
    }
    if(!P.checkn(SCOL)){
            err::out("expected ';' found -> ",P.toks[P.CurrentIndex]);
            return false;
    }

    P.next();
    ResultantNode = ReturnStmt::Create(tok, val);
    P.dump2(__func__);

    return true;
}


bool Parser::ParseType(ParserHelper &P, Ast *&ResultantNode) {
    //Ast* type;
    P.dump(__func__);
    switch(P.CurrentToken){
        case LBRACK:
            if(!ParseArrayType(P,ResultantNode)){
                return false;
            }
            break;
        case I8:
        case I16:
        case I32:
        case I64:
        case UI8:
        case UI16:
        case UI32:
        case UI64:
        case F32:
        case F64:
        case BOOL:
            ResultantNode = PreDefineType::Create(P.toks[P.CurrentIndex]);
            P.next();
            break;
        case CONST:
        {
            tokt tok = P.toks[P.CurrentIndex];
            Token_type Op = P.CurrentToken;
            bool isType = true;
            P.next();
            if(!ParseType(P,ResultantNode)){
                return false;
            }
            ResultantNode = PrefixExpr::Create(tok,Op,ResultantNode,isType,0, NodeConstTy);

        }
            break;
        case FN:
        {
            tokt tok = P.toks[P.CurrentIndex];
            P.next();
            if(!P.check(LPAREN)){
                err::out("expected '{' found -> ",P.toks[P.CurrentIndex]);
                return false;
            }else{
                P.next();
                std::vector<Ast*>ty;
                while(!P.check(RPAREN)){
                    if(!ParseType(P,ResultantNode)) {
                        return false;
                    }
                    ty.push_back(ResultantNode);
                    if(P.checkn(COMMA)) {
                        break;
                    }
                }
                if(!P.checkn(RPAREN)) {
                    err::out("expected ')' found -> ",P.toks[P.CurrentIndex]);
                    return false;
                }
                Ast* ret;
                if(P.checkn(ARROW)){
                    if(ParseType(P, ResultantNode)){
                        ret = ResultantNode;
                    }
                }
                ResultantNode = FnType::Create(tok, ty, ret);

            }
        }
            break;
        case IDEN:
            if(!ParseSpecificType(P,ResultantNode)){
                return false;
            }
            break;
        case STAR:
        {
            bool isType  = true;
            tokt tok  = P.toks[P.CurrentIndex];
            Token_type op = P.CurrentToken;
            int DefCount = 0;
            P.next();
            while(P.checkn(STAR)){
                DefCount++;
            }
            if(!ParseType(P,ResultantNode)){
                return false;
            }
            ResultantNode = PrefixExpr::Create(tok,op,ResultantNode,isType,DefCount,NodePtr);
        }
            break;
        case SELF:
        {   
            if(!ParseIdentifier(P,ResultantNode)) {
                return false;
            }
        }
        break;
        default:
            err::out("expected 'type' found ->",P.toks[P.CurrentIndex]);
            return false;
    }
            P.dump2(__func__);
    return true;
}


bool Parser::ParseArrayType(ParserHelper &P, Ast *&ResultantNode) {
    tokt tok = P.toks[P.CurrentIndex];
    std::vector<Ast*>size;
    P.next();
    if(P.check(RBRACK)) {
        err::out("expected ']' found ->",P.toks[P.CurrentIndex]);
        return false;
    }
    while(P.check(RBRACK)) {
        if(!ParseExpr(P, ResultantNode)) {
            return false;
        }
        size.push_back(ResultantNode);
        if(!P.check(SCOL)) {
            break;
        }
    }
    if(!P.checkn(RBRACK)) {
        err::out("expected ']' found ->",P.toks[P.CurrentIndex]);
        return false;
    }
    if(!ParseType(P,ResultantNode)){
        return false;
    }
    ResultantNode = Array::Create(tok,size,ResultantNode);
    return true;
}


bool Parser::ParseTypeStatm(ParserHelper &P, Ast *&ResultantNode){
    tokt tok = P.toks[P.CurrentIndex];
    P.next();
    if(!P.check(IDEN)){
        err::out("expected 'identifier' found - ",P.toks[P.CurrentIndex]);
        return false;
    }
    tokt Ident = P.toks[P.CurrentIndex];
    P.next();
    if(!P.checkn(ASSN)){
        err::out("expected '=' found - ",P.toks[P.CurrentIndex]);
        return false;
    }

    if(!ParseType(P,ResultantNode)){
        return false;
    }
    ResultantNode = TypeStmt::Create(tok, Ident, ResultantNode);
    return true;
}


bool Parser::ParseCall(ParserHelper &P, Ast *&ResultantNode) {
    tokt tok = P.toks[P.CurrentIndex];
    Ast* sig = ResultantNode;
    std::vector<Ast*>args;

    if(!P.checkn(LPAREN)){
        err::out("expected '(' found - ", P.toks[P.CurrentIndex]);
        return false;
    }

    while(!P.check(FEOF)){
        if(!ParseExpr(P, ResultantNode)) {
            return false;
        }

        args.push_back(ResultantNode);
        if(!args.back()){
            return false;
        }

        if(!P.checkn(COMMA)) {
            break;
        }
    }

    if(!P.checkn(RPAREN)){
        err::out("expected ')' found - ", P.toks[P.CurrentIndex]);
        return false;
    }
    ResultantNode = FunctionCall::Create(tok,sig,args);
    return true;
}


bool Parser::ParseListExpr(ParserHelper &P, Ast *&ResultantNode) {
    tokt tok = P.toks[P.CurrentIndex];
    P.next();
    std::vector<Ast*>List;
    while(!P.check(RBRACK)) {
        if(!ParseExpr(P, ResultantNode)){
            return false;
        }
        List.push_back(ResultantNode);
        if(!P.checkn(COMMA)){
            break;
        }
    }
    if(!P.checkn(RBRACK)){
        err::out("expected ']' found - ", P.toks[P.CurrentIndex]);
        return false;
    }
    ResultantNode = ListExpr::Create(tok, List);
    return true;
}


bool Parser::ParseCastExpr(ParserHelper &P, Ast *&ResultantNode) {
    Ast* LHS = ResultantNode;
    tokt tok = P.toks[P.CurrentIndex];
    Token_type op = P.CurrentToken;
    Ast* RHS;
    while(P.checkn(AS)){
        if(!ParseType(P,ResultantNode)){
            return false;
        }
        ResultantNode = Expression::Create(LHS,op,RHS,NodeAsExpr);
    }
    return true;
}


bool Parser::ParsePrefixExpr(ParserHelper &P, Ast *&ResultantNode, KindExpression Expr) {
    tokt tok = P.toks[P.CurrentIndex];
    Token_type op = P.CurrentToken;
    NodeCategory NodeKind;
    bool isType = false;
    int DefCount = 0;
    if(P.checkn(STAR)) {
        DefCount = 0;
        while(P.checkn(STAR)){
            DefCount++;
        }
        if(P.check(AND_OP)){
            if(!ParsePrefixExpr(P,ResultantNode,Expr)){
                return false;
            }
        }else if(P.check(IDEN) || P.check(LPAREN)){
            if(!ParseExpr3(P,ResultantNode,Expr)){
                return false;
            }
        }else {
            err::out("invalid expression with operator - ", P.toks[P.CurrentIndex]);
            return false;
        }
        NodeKind = NodePtr;
    }else if(P.checkn(AND_OP)) {
        if(P.check(IDEN)||P.check(LPAREN)){
            if(!ParseExpr3(P,ResultantNode,Expr)){
                return false;
            }
        }else if(P.check(LBRACK)) {
            if(!ParseListExpr(P,ResultantNode)){
                return false;
            }
        }else {
            err::out("invalid expression with operator - ", P.toks[P.CurrentIndex]);
            return false;
        }
        NodeKind - NodeRef;
    }else if(P.checkn(PLUS)||P.checkn(MINUS)||P.checkn(NOT)||P.checkn(NOT_OP)) {
        if(!P.check(IDEN)&&!P.check(LPAREN)&&!P.isLiteral(P.CurrentToken)){
            err::out("invalid expression with operator - ", P.toks[P.CurrentIndex]);
            return false;
        }
        if(!ParseExpr(P, ResultantNode)){
            return false;
        }
        NodeKind = NodePrefix;
    }
    ResultantNode = PrefixExpr::Create(tok,op,ResultantNode, isType, DefCount, NodeKind);
    return true;
}

bool Parser::ParseExpr(ParserHelper &P, Ast *&ResultantNode) {
    int Precedance = 0;
    if(!ParseExpr1(P,ResultantNode,Precedance)){
        return false;
    }
    return 0;
}








bool Parser::ParseExpr1(ParserHelper &P, Ast *&ResultantNode, int Precedance) { 
    KindExpression Expr;
    if(!ParseExpr2(P, ResultantNode, Expr)){
        return false;
    }
    if(P.ArithmeticOP(P.CurrentToken)) {
        if(Expr == KExpr_ArrayInit || Expr == KExpr_StructInit || Expr == KExpr_DotDot){
            err::out("Arithmetic operation not allowed with - ",P.toks[P.CurrentIndex]);
            return false;
        }
        if(!ParseBinaryExpr(P,ResultantNode,Precedance)) {
            return false;
        }
    }else if(P.LogicalOP(P.CurrentToken)) {
        if(Expr == KExpr_ArrayInit || Expr == KExpr_StructInit || Expr == KExpr_DotDot){
            err::out("Logical operation not allowed with - ",P.toks[P.CurrentIndex]);
            return false;
        }
        if(!ParseBinaryExpr(P,ResultantNode,Precedance)) {
            return false;
        }

    }if(P.ConditionalOP(P.CurrentToken)) {
        if(Expr == KExpr_ArrayInit || Expr == KExpr_StructInit || Expr == KExpr_DotDot){
            err::out("Conditional operation not allowed with - ",P.toks[P.CurrentIndex]);
            return false;
        }
        if(!ParseBinaryExpr(P,ResultantNode,Precedance)) {
            return false;
        }
    }else if(P.AssignOP(P.CurrentToken)) {
        if(Expr == KExpr_ArrayInit || Expr == KExpr_StructInit || Expr == KExpr_DotDot){
            err::out("Assignment operation not allowed with - ",P.toks[P.CurrentIndex]);
            return false;
        }
        if(!ParseBinaryExpr(P,ResultantNode,Precedance)) {
            return false;
        }
    }
    return true;
}

bool Parser::ParseExpr2(ParserHelper &P, Ast *&ResultantNode, KindExpression &Expr) { 
    if(!ParseExpr3(P, ResultantNode, Expr)){
        return false;
    }

    if(P.check(AS)) {

    }
    
    return true;
}

bool Parser::ParseExpr3(ParserHelper &P, Ast *&ResultantNode, KindExpression &Expr) { 
    if(P.check(STAR)) {

    }else if(P.check(AND_OP)) {

    }else if(P.check(PLUS)||P.check(MINUS)) {

    }if(!ParseExpr3(P, ResultantNode, Expr)) {
        return false;
    }

    return true;
}






bool Parser::ParseExpr4(ParserHelper &P, Ast *&ResultantNode, KindExpression &Expr) {
    if(!ParseExpr5(P,ResultantNode, Expr)){
        return false;
    }

    if(P.check(DOT) || P.check(ARROW)){
        if(Expr == KExpr_StructInit || Expr == KExpr_Literal ||
            Expr == KExpr_ArrayInit || Expr == KExpr_DotDot || 
            Expr == KExpr_Binary) {
            err::out("Member Access operation not allowed with - ",P.toks[P.CurrentIndex]);
            return false;
        }
        if(!ParseDotArrowExpr(P, ResultantNode, Expr)) {
            return false;
        }
    }
 
    
    return true;
}

bool Parser::ParseExpr5(ParserHelper &P, Ast *&ResultantNode, KindExpression &Expr) {
    if(!ParseExpression(P, ResultantNode, Expr)){
        return false;
    }

    if(P.check(LPAREN) || P.check(LBRACK)){
        if(Expr == KExpr_StructInit || Expr == KExpr_Literal ||
            Expr == KExpr_ArrayInit || Expr == KExpr_DotDot || 
            Expr == KExpr_Binary) {
            err::out("Member Access operation not allowed with - ",P.toks[P.CurrentIndex]);
            return false;
        }

         while(!P.check(FEOF)&&(P.check(LBRACK)||P.check(LPAREN))){
            if(P.check(LPAREN)) {
                if(!ParseCall(P,ResultantNode)){
                    return false;
                }
                Expr = KExpr_Call;
            }else if(P.check(LBRACK)) {
                if(!ParseArrayIndexExpr(P,ResultantNode)){
                    return false;
                }
                Expr = KExpr_ArrayAccess;
            }
        }
        
    }
 
    
    return true;
}

bool Parser::ParseExpression(ParserHelper &P, Ast *&ResultantNode, KindExpression &Expr) {
    switch (P.CurrentToken)
    {
    case IDEN:
        switch (P.checkh(P.CurrentToken))
        {
        case DOTDOT:
        {
            tokt tok = P.toks[P.CurrentIndex];
            Token_type op = P.CurrentToken;
            ResultantNode = Expression::Create(ResultantNode, op, nullptr, NodeDotDotExpr);
            P.next();
            Expr = KExpr_DotDot;
        }
        break;
        case LT:
        case LBRACE:
            if(!ParseStructExpr(P,ResultantNode)) {
                return false;
            }
            Expr = KExpr_StructInit;
            break;
        default:
            if(!ParseIdentifier(P,ResultantNode)){
                return false;
            }
            Expr = KExpr_Identifier;
            break;
        }
        break;
    case LBRACK:
        if(!ParseListExpr(P,ResultantNode)) {
                return false;
        }
        Expr = KExpr_ArrayInit;
        break;
    case LPAREN:
    {
        if(!ParseParenExpr(P,ResultantNode, Expr)) {
            return false;
        }
        Expr = KExpr_Grouped;
    }   
    break;
    case INT:
    {
        ResultantNode = NumericLiteral::Create(P.toks[P.CurrentIndex]);
        P.next();
        Expr = KExpr_Literal;
    }
    break;
    case FLOAT:
    {
        ResultantNode = FloatLiteral::Create(P.toks[P.CurrentIndex]);
        P.next();
        Expr = KExpr_Literal;
    }
    break;
    case STR:
    {    
        ResultantNode = StringLiteral::Create(P.toks[P.CurrentIndex],false);
        P.next();
        Expr = KExpr_Literal;
    }
        break;
    case CHAR:
    {
        ResultantNode = StringLiteral::Create(P.toks[P.CurrentIndex],true);
        P.next();
        Expr = KExpr_Literal;
    }
    break;
    case TRUE:
    case FALSE:
    {
        ResultantNode = BoolLiteral::Create(P.toks[P.CurrentIndex]);
        P.next();
        Expr = KExpr_Literal;
    }
    break;
    default:
        break;
    }
    return true;
}




bool Parser::ParseLiteral(ParserHelper &P, Ast *&ResultantNode) {
    tokt t = P.getTok();
    switch (P.CurrentToken)
    {
    case INT:
    {
        ResultantNode = NumericLiteral::Create(t);
    }
    break;
    case FLOAT:
    {
        ResultantNode = FloatLiteral::Create(t);
    }
    break;
    case STR:
        ResultantNode = StringLiteral::Create(t,true);
        break;
    case CHAR:
        ResultantNode = StringLiteral::Create(t,false);
        break;
    case TRUE:
    case FALSE:
    {
        ResultantNode = BoolLiteral::Create(t);
    }
    break;
    default:
        break;
    }
    P.next();
    return true;
}

bool Parser::ParseParenExpr(ParserHelper &P, Ast *&ResultantNode, KindExpression &Expr) {
    P.next();
    P.dump(__func__);
    Ast* node;
    if(P.check(RPAREN)){
        err::out("expected expression found -> ",P.toks[P.CurrentIndex]);
        return false;
    }
    if(!ParseExpr(P, ResultantNode)){
        return false;
    }
    node = ResultantNode;

    if(!P.checkn(RPAREN)){
        err::out("expected ')' found -> ",P.toks[P.CurrentIndex]);
        return false;
    } 
    ResultantNode = GroupedExpr::Create(ResultantNode);
            P.dump2(__func__);

    return true;
}

bool Parser::ParseIdentifier(ParserHelper &P, Ast *&ResultantNode){
    bool HasSelf = false;
    tokt tok = P.toks[P.CurrentIndex];
    if(P.check(SELF)){
        HasSelf = true;
    }
    ResultantNode = Identifier::Create(tok);
    P.next();
    return true;
}



bool Parser::ParseDotArrowExpr(ParserHelper &P, Ast *&ResultantNode, KindExpression &Expr) {
    while(P.check(DOT)||P.check(ARROW)){
        tokt tok = P.toks[P.CurrentIndex];
        Token_type op = P.CurrentToken;
        Ast* LHS = ResultantNode;
        P.next();
        if(!ParseExpr3(P,ResultantNode,Expr)){
            return false;
        }
        ResultantNode = Expression::Create(LHS, op, ResultantNode, NodeMemExpr);
    }
    return true;
}


bool Parser::ParseStructExpr(ParserHelper &P, Ast *&ResultantNode) {
    P.dump(__func__);      
    bool isDecl = false;
    tokt tok = P.toks[P.CurrentIndex];
    Ast* sig = ResultantNode;
    std::vector<tokt> Temp;
    std::vector<Ast*> Vals;

    if(P.checkn(LT)){
        // if(!isAllowTemp){
        //     err::out("template with extern not allowed ->",P.toks[P.CurrentIndex]);
        //     return false;
        // }
        while(P.check(IDEN)){
            Temp.push_back(P.toks[P.CurrentIndex]);
            if(!P.checkn(COMMA))
                break;
        }
        if(!P.checkn(GT)){
            err::out("expected '>' found ->",P.toks[P.CurrentIndex]);
            return false;
        }
    }

    if(!P.checkn(LBRACE)){
        err::out("expected '{' found -> ",P.toks[P.CurrentIndex]);
        return false;
    }

    while(!P.check(RBRACE)){
        if(P.check(DOT)) {
            
        }
        if(!ParseExpr(P, ResultantNode)){
            return false;
        }
        Vals.push_back(ResultantNode);
        if(!Vals.back()){
            return false;
        }
        if(!P.checkn(COMMA)){
            break;
        }
    }

    if(!P.checkn(RBRACE)) {
        err::out("expected '}' found -> ",P.toks[P.CurrentIndex]);
        return false;
    }
    ResultantNode = PostfixExpr::Create(tok,sig,Temp,Vals,NodeStructDef);
    P.dump2(__func__);
    return true;
}

bool Parser::ParseArrayIndexExpr(ParserHelper &P, Ast *&ResultantNode) {
    tokt tok = P.toks[P.CurrentIndex];
    Ast* Ident = ResultantNode;
    std::vector<Ast*>IndexVal;
    while(!P.check(FEOF)&&!P.check(RBRACK)){
        if(!ParseExpr(P, ResultantNode)){
            return false;
        }
        IndexVal.push_back(ResultantNode);
        if(!P.checkn(SCOL)){
            break;
        }
    }

    if(!P.checkn(RBRACK)){
        err::out("expected ']' found -> ",P.toks[P.CurrentIndex]);
        return false;
    }

    ResultantNode = PostfixExpr::Create(tok,Ident,IndexVal,NodeIndexExpr);
}



bool Parser::ParseBinaryExpr(ParserHelper &P, Ast *&ResultantNode, int prev_prece) {
    P.dump(__func__);
    tokt tok = P.toks[P.CurrentIndex];
    Token_type opr;
    Ast* left;
    if(!ResultantNode){
        if(!ParseExpr1(P,ResultantNode,prev_prece)){
            return false;
        }
        left = ResultantNode;
    }
    left = ResultantNode;

    //2+3*3/4+2*3
    while(P.BinaryOP(P.CurrentToken)) {
        opr = P.CurrentToken;
        int CurPrecedance = P.preced(opr);
        if(prev_prece > CurPrecedance) {
            ResultantNode = left;
            return true;
        }
        P.next();
        ResultantNode = nullptr;
        if(!ParseExpr1(P, ResultantNode,++CurPrecedance)){
            return false;
        }
        left = Expression::Create(left, opr, ResultantNode, NodeBinaryExpr);
    }
    ResultantNode = left;
            P.dump2(__func__);

    return true;
}


bool Parser::ParseSpecificType(ParserHelper &P, Ast *&ResultantNode) {
    tokt tok = P.toks[P.CurrentIndex];
    ParseIdentifier(P,ResultantNode);
    Ast* Name = ResultantNode;
    if(P.checkn(LT)){
        std::vector<tokt>Temp;
        while(P.check(IDEN)){
            Temp.push_back(P.toks[P.CurrentIndex]);
            if(!P.checkn(COMMA))
                break;
        }
        if(!P.checkn(GT)){
            err::out("expected '>' found -",P.toks[P.CurrentIndex]);
            return false;
        }
        ResultantNode = PostfixExpr::Create(Name, Temp);
    }
    return true;
}

bool Parser::ParseEnumStmt(ParserHelper &P, Ast *&ResultantNode) {
    std::vector<tokt>udata;
    std::vector<Ast*>val;
    if(!P.checkn(ENUM)){
        err::out("expected 'enum' keyword found -",P.toks[P.CurrentIndex]);
        return false;
    }
    if(!P.check(IDEN)) {
        err::out("expected 'identifier' found -",P.toks[P.CurrentIndex]);
        return false;
    }
    tokt Name = P.toks[P.CurrentIndex];

    if(!P.checkn(LBRACE)){
        err::out("expected '{' found ->",P.toks[P.CurrentIndex]);
        return false;
    }

    while(P.check(IDEN)){
        udata.push_back(P.toks[P.CurrentIndex]);
        if(P.checkn(ASSN)){
            if(!ParseExpr(P, ResultantNode)){
                return false;
            }
            val.push_back(ResultantNode);
        }
        if(!P.checkn(COMMA)){
            err::out("expected ',' found ->",P.toks[P.CurrentIndex]);
            return false;
        }
    }

    if(!P.checkn(RBRACE)){
        err::out("expected '}' found ->",P.toks[P.CurrentIndex]);
        return false;
    }
    ResultantNode = EnumExpr::Create(Name, udata, val);
    return true;
}


bool Parser::ParseStructStmt(ParserHelper &P, Ast *&ResultantNode) {
    P.dump(__func__);
    tokt tok = P.toks[P.CurrentIndex];
    std::vector<tokt> ElementName;
    std::vector<Ast*> ElementTy;
    std::vector<tokt> Temp;
    if(!P.checkn(STRUCT)) {
        err::out("expected keyword 'struct' found ->",P.toks[P.CurrentIndex]);
        return false;
    }

    if(!P.check(IDEN)) {
        err::out("expected 'identifier' found ->",P.toks[P.CurrentIndex]);
        return false;
    }
    tokt Name = P.toks[P.CurrentIndex];
  
    if(P.checkn(LT)){
        while(P.check(IDEN)){
            Temp.push_back(P.toks[P.CurrentIndex]);
            if(!P.checkn(COMMA))
                break;
        }
        if(!P.checkn(GT)){
            err::out("expected '>' found ->",P.toks[P.CurrentIndex]);
            return false;
        }
    }

    if(!P.check(LBRACE)){
        err::out("expected field expression '{' found -> ",P.toks[P.CurrentIndex]);
        return false;
    }

    while(P.CurrentToken !=  FEOF && P.check(IDEN)){
        ElementName.push_back(P.toks[P.CurrentIndex]);
        if(P.checkn(COL)){
            err::out("expected struct field expression ':' found - ",P.toks[P.CurrentIndex]);
            return false;
        }
        if(!ParseType(P,ResultantNode)){
            err::out("expected struct field expression ': ?type' found - ",P.toks[P.CurrentIndex]);
            return false;
        }
        ElementTy.push_back(ResultantNode);
        if(!P.checkn(COMMA)){
            break;
        }
    }

    if(!P.checkn(RBRACE)){
        err::out("expected '}' found -> ",P.toks[P.CurrentIndex]);
        return false;
    }

    ResultantNode = StructStmt::Create(tok, Name, Temp, ElementName, ElementTy);
    P.dump2(__func__);

    return true;
}




bool Parser::ParseFuncDef(ParserHelper &P, Ast *&ResultantNode) {
     P.dump(__func__);
    tokt tok = P.toks[P.CurrentIndex];
    std::vector<tokt>pN;
    std::vector<Ast*>pTy;
    Ast* RetVal = nullptr;
    Ast* StructVal = nullptr;
    Ast* Block = nullptr;

    if(!P.checkn(FN)) {
        err::out("expected keyword 'fn' found ->\'",P.toks[P.CurrentIndex]);
        return false;
    }
    if(!P.check(IDEN)){
        err::out("expected 'Identifier' with function defination found ->\'",P.toks[P.CurrentIndex]);
        return false;
    }   
    tokt Name = P.toks[P.CurrentIndex];

    if(P.checkn(LPAREN)) {
        err::out("expected function expression (parameters : type) found -> ",P.toks[P.CurrentIndex]);
        return false;
    }
    while(P.check(IDEN)) {
        pN.push_back(P.toks[P.CurrentIndex]);
        if(!P.checkn(SCOL)){
            err::out("expected ':' found -> \'",P.toks[P.CurrentIndex]);
            return false;
        }
        if(!ParseType(P,ResultantNode)) {
            return false;
        }
        pTy.push_back(ResultantNode);
        
        if(!P.checkn(COMMA)) {
            break;
        }
    }

    if(!P.checkn(RPAREN)){
        err::out("expected ')' found -> ",P.toks[P.CurrentIndex]);
        return false;
    }

    if(P.checkn(ARROW)){
        if(!ParseType(P,ResultantNode)){
            err::out("expected 'type' found -> ",P.toks[P.CurrentIndex]);
            return false;
        }
        RetVal = ResultantNode;
    }


    if(P.check(LBRACE)) {
        bool isBrace = true;
        if(!ParseBlock(P, ResultantNode,isBrace)){
            return false;
        }
        Block = ResultantNode;
    }else{
        err::out("expected function body expression '{}' found - ",P.toks[P.CurrentIndex]);
        return false;
    }
    ResultantNode = FunctionDef::Create(tok,Name, pN, pTy, RetVal, Block);
    P.dump2(__func__);
    return true;
}

bool Parser::ParseIfStmt(ParserHelper &P, Ast *&ResultantNode) {
    P.dump(__func__);
    tokt tok = P.toks[P.CurrentIndex];
    if(!P.checkn(IF)){
        err::out("expected 'if' found - ",P.toks[P.CurrentIndex]);   
        return false;
    }
    Ast* condition = nullptr;
    Ast* if_ = nullptr;
    Ast* else_ = nullptr;

    if(P.check(LBRACE)) {
        err::out("expected 'condition' found - ",P.toks[P.CurrentIndex]);
        return false;
    }

    if(!ParseExpr(P, ResultantNode)){
        return false;
    }

    condition = ResultantNode;

    // if(P.checkn(COL)){
    //     //todo
    // }

    bool isBrace = true;
    if(!ParseBlock(P, ResultantNode,isBrace)){
        return false;
    }
    if_ = ResultantNode;

    if(P.checkn(ELSE)) {
        switch(P.CurrentToken){
            case IF:
                if(!ParseIfStmt(P,ResultantNode)){
                    return false;
                }
                else_ = ResultantNode;
                break;
            case LBRACE:
                if(!ParseBlock(P, ResultantNode, isBrace)){
                    return false;
                }
                else_ = ResultantNode;
                break;
            default:
                err::out("invalid else expression ",P.toks[P.CurrentIndex]);
                return false;

        }
    }
    ResultantNode = IfStmt::Create(tok,condition, if_, else_);
            P.dump2(__func__);

    return true;
}

bool Parser::ParseForStmt(ParserHelper &P, Ast *&ResultantNode){
    tokt tok = P.toks[P.CurrentIndex];
    Ast* var = nullptr;
    Ast* cond = nullptr;
    Ast* incr = nullptr;
    Ast* body = nullptr;
    if(!P.checkn(FOR)) {
        err::out("expected keyword 'for' found - ",P.toks[P.CurrentIndex]);
        return false;
    }

    if(P.checkn(SCOL)){
        goto cond;
    }
    if(!P.check(LET)){
        err::out("expected for expression 'identifier' found - ",P.toks[P.CurrentIndex]);
        return false;
    }
    if(!ParseVarStmt(P, ResultantNode)){
        return false;
    }
    var = ResultantNode;
    if(!P.checkn(SCOL)){
        err::out("expected for expression ';' found - ",P.toks[P.CurrentIndex]);
        return false;
    }
cond:
    if(P.checkn(SCOL)){
        goto incr;
    }
    if(!ParseExpr(P, ResultantNode)){
        return false;
    }
    cond = ResultantNode;
    if(!P.checkn(SCOL)){
        err::out("expected for expression ';' found - ",P.toks[P.CurrentIndex]);
        return false;
    }

incr:
    if(P.check(LBRACE)){
        goto incr;
    }
    if(!ParseExpr(P, ResultantNode)){
        return false;
    }
    incr = ResultantNode;
body:
    bool isBrace = true;
    if(!ParseBlock(P,ResultantNode,isBrace)){
        return false;
    }
    body = ResultantNode;
    ResultantNode = ForLoop::Create(tok, var, cond, incr, body);
    return true;
}


// ref from rust lang and scribe
bool Parser::ParseForInStmt(ParserHelper &P, Ast *&ResultantNode) {
    P.dump(__func__);
    tokt tok = P.toks[P.CurrentIndex];
    Ast* in = nullptr;
    Ast* var = nullptr;
    Ast* cond = nullptr;
    Ast* incr = nullptr;
    Ast* body = nullptr;
    bool isBrace = true;
    if(!P.checkn(FOR)) {
        err::out("expected keyword 'for' found - ",P.toks[P.CurrentIndex]);
        return false;
    }

    if(!P.check(IDEN)){
        err::out("expected 'identifier' found - ",P.toks[P.CurrentIndex]);
        return false;
    }
    ParseIdentifier(P,ResultantNode);
    Ast* tempvar = ResultantNode;
    tokt iter_tempvar__tok = P.getTok();


    if(!P.checkn(IN)){
        err::out("expected keyword 'in' found -> ",P.toks[P.CurrentIndex]);
        return false;
    }
    // 0..1 , iter

    if(P.check(LBRACE)){
        err::out("expected 'expression' found -> ",P.toks[P.CurrentIndex]);
        return false;
    }
    if(P.check(INT)){
        Ast *from;
        Ast *to;
        Ast *intlit;

        from = NumericLiteral::Create(P.toks[P.CurrentIndex]);
        P.next();
        if(!P.checkn(DOTDOT)){
            err::out("expected for-in expression '..' found - ",P.toks[P.CurrentIndex]);
            return false;
        }

        if(!P.check(INT)){
            err::out("expected for-in expression integer after '..' found - ",P.toks[P.CurrentIndex]);
            return false;
        }

        to = NumericLiteral::Create(P.toks[P.CurrentIndex]);;
        P.next();

        if(P.checkn(DOTDOT)){
            if(!P.check(INT)){
                err::out("expected for-in expression integer after '..' found - ",P.toks[P.CurrentIndex]);
                return false;
            }
            intlit = NumericLiteral::Create(P.toks[P.CurrentIndex]);;
            P.next();
        }else{
            tokt t = tokt(-1,-1,"1",INT);
            intlit = NumericLiteral::Create(t);
        }
        
        if(!ParseBlock(P,ResultantNode,isBrace)){
            return false;
        }
        body = ResultantNode;
        var = VarStmt::Create(iter_tempvar__tok,tempvar, nullptr, from,false);
        cond = Expression::Create(tempvar, LT, to,NodeBinaryExpr);
        incr = Expression::Create(tempvar, ASSN_PLUS, intlit,NodeBinaryExpr);
        ResultantNode = ForLoop::Create(tok, var, cond, incr, body);
        return true; 

    }else{    
        if(!ParseExpr(P,ResultantNode)) {
            err::out("for-in use only { 1..5..1 , .iter()} -> ",P.getTok());
            return false;
        }
        if(ResultantNode->nodeCategory() != NodeMemExpr){
            err::out("expected 'iterator' expression found -> ",P.getTok());
            return false;
        }
        in = ResultantNode;

        bool isBrace = true;
        if(!ParseBlock(P,ResultantNode,isBrace)){
            return false;
        }
        body = ResultantNode;
        
        ///<iterVar> = <vector.iter()>
        tokt iter_var_tok = tokt(-1, -1, "iterVar", IDEN);
        tokt iter__tempvar__tok = tokt(-1, -1, "_"+tempvar->toString(), IDEN);
        tokt iter_begin_tok = tokt(-1, -1, "begin", IDEN);
        tokt iter_end_tok = tokt(-1, -1, "end", IDEN);
        tokt iter_next_tok = tokt(-1, -1, "next", IDEN);
        tokt iter_at_tok = tokt(-1, -1, "at", IDEN);

        Ast* iter_begin_Iden = Identifier::Create(iter_begin_tok);
        Ast* iter_end_Iden = Identifier::Create(iter_end_tok);
        Ast* iter_next_Iden = Identifier::Create(iter_next_tok);
        Ast* iter_at_Iden = Identifier::Create(iter_at_tok);

        Ast* iter_tempvar_ident = Identifier::Create(iter__tempvar__tok);
        Ast* iter_var_ident = Identifier::Create(iter_var_tok);

        Ast* iter_decl = VarStmt::Create(iter_var_tok, iter_var_ident, nullptr, in, false);
        std::vector<Ast*>args;
        Ast* begin_call = FunctionCall::Create(iter_begin_tok,iter_begin_Iden,args);
        Ast* iter_dot_begin_call = Expression::Create(iter_var_ident, DOT, begin_call, NodeMemExpr);
        var = VarStmt::Create(iter__tempvar__tok, iter_tempvar_ident, nullptr, iter_dot_begin_call,false);

        ///<_temp> != <iterVar.end()>
        Ast* end_call = FunctionCall::Create(iter_end_tok, iter_end_Iden, args);
        Ast* iter_end_call = Expression::Create(iter_var_ident, DOT, end_call, NodeMemExpr);
        cond = Expression::Create(iter_tempvar_ident, NEQL, iter_end_call, NodeBinaryExpr);

        ///<iterVar.next(_temp)>
        args.clear();
        args = {iter_tempvar_ident};
        Ast* nextcall = FunctionCall::Create(iter_next_tok, iter_next_Iden, args);
        Ast* iternext = Expression::Create( iter_var_ident, DOT, nextcall, NodeMemExpr);
        incr = Expression::Create(iter_tempvar_ident, ASSN, iternext, NodeAssnExpr);

        ///<var = _iterVar.at()>
        args.clear();
        args = {iter_tempvar_ident};
        Ast* at_call = FunctionCall::Create(iter_at_tok, iter_at_Iden, args);
        Ast* iter_at_call = Expression::Create(iter_var_ident, DOT, at_call, NodeMemExpr);
        Ast* inside_loop_var_decl = VarStmt::Create(iter_tempvar__tok, tempvar, nullptr, iter_at_call, false);

        
        auto Block = static_cast<BlockStmt*>(body);
        Block->getStmts().insert(Block->getStmts().begin(), inside_loop_var_decl);
        body = Block;
        ResultantNode = ForLoop::Create(tok, var, cond, incr, body);
                P.dump2(__func__);
        std::vector<Ast*>b = {iter_decl, ResultantNode};
        ResultantNode = BlockStmt::Create(b);
    }
    return true;
}


bool Parser::ParseExtern(ParserHelper &P, Ast *&ResultantNode) {
    tokt tok = P.toks[P.CurrentIndex];
    P.next();
    std::vector<Ast*>Header;
    if(!P.checkn(LBRACK)){
        err::out("expected '[' found - ",P.toks[P.CurrentIndex]);
        return false;
    }
    if(!P.check(IDEN)) {
        err::out("expected 'identifier' found - ",P.toks[P.CurrentIndex]);
        return false;
    }
    if(!ParseIdentifier(P,ResultantNode)){
        return false;
    }
    Header.push_back(ResultantNode);
    if(!P.check(STR)) {
        err::out("expected 'string' found - ",P.toks[P.CurrentIndex]);
        return false;
    }
    if(!ParseLiteral(P,ResultantNode)){
        return false;
    }
    Header.push_back(ResultantNode);
    if(!P.checkn(RBRACK)){
        err::out("expected ']' found - ",P.toks[P.CurrentIndex]);
        return false;
    }

    if(P.check(FN)){
        if(!ParseFuncDef(P, ResultantNode)) {
            return false;
        }
    }else if(P.check(STRUCT)){
        if(!ParseStructStmt(P, ResultantNode)) {
            return false;
        }
    }else if(P.check(LET)) {
        if(!ParseVarStmt(P, ResultantNode)) {
            return false;
        }
    }else{
        err::out("expected fn and struct statement block with extern found - ",P.toks[P.CurrentIndex]);
        return false;
    }
    Header.push_back(ResultantNode);

    ResultantNode = Extern::Create(tok, Header);
    return true;
}


bool Parser::ParseVarStmt(ParserHelper &P, Ast *&ResultantNode) {
    tokt tok = P.toks[P.CurrentIndex];
    Ast* Var;
    Ast* Ty;
    Ast* Val;
    bool isty = false;
    bool isval = false;
    bool isuse = false;
    bool isIs = false;
    bool isIn = false;
    bool isConst = false;
    if(!P.checkn(LET)){
        err::out("expected 'let' found -> ",P.toks[P.CurrentIndex]);
        return false;
    }
    if(P.checkn(CONST)){
        isConst = true;
    }

    if(!P.check(IDEN)){
        err::out("expected 'identifier' found -> ",P.toks[P.CurrentIndex]);
        return false;
    }
    ParseIdentifier(P,ResultantNode);
    Var = ResultantNode;

    if(!P.checkn(COL)){
        goto val;
    }
    
    isty = true;
    if(!ParseType(P,ResultantNode)){
        return false;
    }
    Ty = ResultantNode;

val:  
    if(!P.checkn(ASSN)){
        goto done;
    }
    isval = true;
    if(!ParseExpr(P, ResultantNode)){
            return false;
    }
    Val = ResultantNode;
done:
    if(!isval && !isty){
        err::out("invalid variable declaration - no type or value set", P.getTok());
        return false;
    }
    
    if(isConst) {
        if(!isval||!isty) {
            err::out("let-const statements should have type and val "
					" - no 'in' and 'is' allowed", P.getTok());
    
            return false;
        }
    }

    ResultantNode = VarStmt::Create(tok, Var, Ty, Val, isConst);
    return true;
}
} // namespace parser
