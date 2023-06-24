
#include"../../include/parser/Parser.hpp"
#include"../../include/parser/Ast.hpp"
#include"../../include/Error.hpp"
// #include"parser/Parser.hpp"
// #include"Error.hpp"

namespace parser
{
using namespace ast;


bool Parser::ParseBlock(ParserHelper &P, Ast *&ResultantNode, bool isGlob){
    std::vector<Ast*>stmts;
    if(!isGlob&&!P.checkn(LBRACE)){
        err::out("expected '{' found",P.peek_l());
        return false;
    }
    while(!P.check(FEOF)&&!P.check(RBRACE)) {
        bool isSemiCol = false;
        switch(P.peek_tt()){ 
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
        case MOD:
        {
            if(!ParseMod(P, ResultantNode)){ return false; }
            isSemiCol = true;
        }
        break;
        case WHILE:
            if(!ParseWhileStmt(P, ResultantNode)){ return false; }
            break;
        case LET:
        {
            if(isGlob) { 
                err::out("consider using `const` or `static` instead of `let` for global variables",P.peek_l());
                return false;
            }
            if(!ParseVarStmt(P, ResultantNode)){ return false; }
            isSemiCol = true;
        }
            break;
        case CONST:
        {
            if(!ParseVarStmt(P, ResultantNode)){ return false; }
            isSemiCol = true;
        }
            break;
        case STATIC:
        {
            if(!ParseVarStmt(P, ResultantNode)){ return false; }
            isSemiCol = true;
        }
            break;

        case RETURN:
            if(!ParseReturnStmt(P, ResultantNode)){ return false; }
            isSemiCol = true;
            break;

        case CONTINUE:
        case BREAK:
            stmts.push_back(BranchStmt::Create(P.peek_l()));
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
            err::out("expected ';' found - ",P.peek_l());
            return false;
        }
        stmts.push_back(ResultantNode);
    }
    if(!isGlob&&!P.checkn(RBRACE)){
        err::out("expected '}' found",P.peek_l());
        return false;
    }
    ResultantNode = BlockStmt::Create(stmts);
    return true;
}


bool Parser::ParseUseStmt(ParserHelper &P, Ast *&ResultantNode) {
    Lexeme L = P.peek_l();
    P.next();
    if(!P.check(IDEN)||!ParseIdentifier(P,ResultantNode)) {
        return false;
    }
    if(!ParsePath(P,ResultantNode)) {
        return false;
    }
    if(!ResultantNode){
        return false;
    }
    ResultantNode = UseStmt::Create(L,ResultantNode);
    return true;
}

bool Parser::ParsePath(ParserHelper &P, Ast *&ResultantNode) {
    Tok T = P.peek_t();
    while(P.checkn(COLCOL)){
        Ast* L = ResultantNode;
        
        if(!P.check(IDEN)||!ParseIdentifier(P,ResultantNode)){
            err::out("expected 'identifier' found",P.peek_l());
            return false;
        }else{
            err::out("expected `{ ident, '{}' }` found",P.peek_l());
            return false;
        }
        T = P.peek_t();
        ResultantNode = Expression::Create(L,T,ResultantNode,NodeSimplePath);
    }

    if(!P.checkn(LBRACE)) {
        err::out("expected `{ ident, '{}' }` found",P.peek_l());
        return false;
    }
    Ast *L = ResultantNode;
    std::vector<Ast*>Expr;
    while(P.check(IDEN)){
        if(!ParseIdentifier(P,ResultantNode)) {
            return false;
        }
        // if(P.check(AS)){
        //     if(!Parse)
        // }
        if(!P.checkn(COMMA)){
            break;
        }
        Expr.push_back(ResultantNode);
    }
    if(!P.checkn(RBRACE)){
        err::out("expected '}' found",P.peek_l());
        return false;
    }
    ResultantNode = BlockStmt::Create(Expr);
    ResultantNode = Expression::Create(L,T,ResultantNode,NodeSimplePath);
    return true;
}

bool Parser::ParseMod(ParserHelper &P, Ast *&ResultantNode) {
    P.next();
    if(!P.check(IDEN)) {
        err::out("expected `mod identifier` found",P.peek_l());
        return false;
    }
    
    module::ModuleBuilder mb;
    std::string path = Mod.modinfo.dirpath;
    module::Module mod; 
    mb.buildMod(path, P.peek_l().getStr(), mod);
    Mod.submod.insert({mod.getModId(),mod});
    return true;
}

// bool Parser::ParseTypePath(ParserHelper &P,Ast *&ResultantNode){
//     Tok T = P.peek_t();
//     while(P.checkn(COLCOL)){
//         Ast* L = ResultantNode;
//         if(P.check(IDEN)&&P.checkh(LT)){
//             break;
//         }
//         if(!P.check(IDEN)||!ParseIdentifier(P,ResultantNode)){
//             err::out("expected 'identifier' found",P.peek_l());
//             return false;
//         }
//         T = P.peek_t();
//         ResultantNode = Expression::Create(L,T,ResultantNode,NodeTypePath);
//     }
//     if(P.check(IDEN)&&P.checkh(LT)){
//         auto L = ResultantNode;
//         if(!ParseSpecificType(P,ResultantNode)){
//             return false;
//         }
//         ResultantNode = Expression::Create(L,T,ResultantNode,NodeTypePath);
//     }
//     return true;
// }


bool Parser::ParseMethod(ParserHelper &P, Ast *&ResultantNode) {
    Lexeme tok = P.peek_l();
    Ast* Assoc;
    std::vector<Ast*> Impl;
    P.next();
    if(!P.check(IDEN)){
        err::out("expected 'identifier' with method statement found - ",P.peek_l());
        return false;
    }
    
    if(!ParseType(P,ResultantNode)){
        return false;
    }

    Ast* Name = ResultantNode;
    P.next();
    if(P.check(FOR)){
        ///@todo
    }
    if(!P.checkn(LBRACE)) {
        err::out("expected '{' found - ",P.peek_l());
        return false;
    }

    while(!P.check(FN)) {
        if(!ParseFuncDef(P, ResultantNode)) {
            return false;
        }
        Impl.push_back(ResultantNode);
    }

    if(!P.checkn(RBRACE)) {
        err::out("expected '}' found - ",P.peek_l());
        return false;
    }
    ResultantNode = Method::Create(tok, Name, Assoc, Impl);
    return true;
}


bool Parser::ParseWhileStmt(ParserHelper &P, Ast *&ResultantNode) {
    Lexeme tok = P.peek_l();
    Ast* Cond;
    P.next();
    if(P.check(LBRACE)){
        err::out("expected condition expression found - ",P.peek_l());
        return false;
    }
    if(!ParseExpr(P, ResultantNode)) {
        return false;
    }
    Cond = ResultantNode;
    if(!ParseBlock(P,ResultantNode,false)){
        return false;
    }
    ResultantNode = WhileLoop::Create(tok,Cond,ResultantNode);
    return true;
}


bool Parser::ParseReturnStmt(ParserHelper &P, Ast *&ResultantNode) {
    P.dump(__func__);
    Lexeme tok = P.peek_l();
    Ast* val;
    P.next();

    if(!P.check(SCOL)){
        if(!ParseExpr(P, ResultantNode)) {
            return false;
        }
        val = ResultantNode;
    }
    if(!P.checkn(SCOL)){
            err::out("expected ';' found -> ",P.peek_l());
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
    switch(P.peek_tt()){
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
            ResultantNode = PreDefineType::Create(P.peek_l());
            P.next();
            break;
        case CONST:
        {// *const i32
            Lexeme tok = P.peek_l();
            Tok Op = P.peek_t();
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
            Lexeme tok = P.peek_l();
            P.next();
            if(!P.check(LPAREN)){
                err::out("expected '{' found -> ",P.peek_l());
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
                    err::out("expected ')' found -> ",P.peek_l());
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
            if(ParseIdentifier(P,ResultantNode)){
                return false;
            }
            if(P.check(LT)&&!ParseSpecificType(P,ResultantNode)){
                return false;
            }
            break;
        case STAR:
        {
            bool isType  = true;
            Lexeme tok  = P.peek_l();
            Tok op = P.peek_t();
            int DefCount = 0;
            P.next();
            while(P.checkn(STAR)){
                DefCount++;
            }
            if(!ParseType(P,ResultantNode)){
                return false;
            }
            ResultantNode = PrefixExpr::Create(tok,op,ResultantNode,isType,DefCount,NodePtrTy);
        }
            break;
        // case SELF:
        // {   
        //     if(!ParseIdentifier(P,ResultantNode)) {
        //         return false;
        //     }
        // }
        break;
        default:
            err::out("expected 'type' found ->",P.peek_l());
            return false;
    }
            P.dump2(__func__);
    return true;
}


bool Parser::ParseArrayType(ParserHelper &P, Ast *&ResultantNode) {
    Lexeme tok = P.peek_l();
    std::vector<Ast*>size;
    P.next();
    if(P.check(RBRACK)) {
        err::out("expected ']' found ->",P.peek_l());
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
        err::out("expected ']' found ->",P.peek_l());
        return false;
    }
    if(!ParseType(P,ResultantNode)){
        return false;
    }
    ResultantNode = Array::Create(tok,size,ResultantNode);
    return true;
}


bool Parser::ParseTypeStatm(ParserHelper &P, Ast *&ResultantNode){
    Lexeme tok = P.peek_l();
    P.next();
    if(!P.check(IDEN)){
        err::out("expected 'identifier' found - ",P.peek_l());
        return false;
    }
    Lexeme Ident = P.peek_l();
    P.next();
    if(!P.checkn(ASN)){
        err::out("expected '=' found - ",P.peek_l());
        return false;
    }

    if(!ParseType(P,ResultantNode)){
        return false;
    }
    ResultantNode = TypeStmt::Create(tok, Ident, ResultantNode);
    return true;
}


bool Parser::ParseCall(ParserHelper &P, Ast *&ResultantNode) {
    Lexeme tok = P.peek_l();
    Ast* sig = ResultantNode;
    std::vector<Ast*>args;

    if(!P.checkn(LPAREN)){
        err::out("expected '(' found - ", P.peek_l());
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
        err::out("expected ')' found - ", P.peek_l());
        return false;
    }
    ResultantNode = FunctionCall::Create(tok,sig,args);
    return true;
}


bool Parser::ParseListExpr(ParserHelper &P, Ast *&ResultantNode) {
    Lexeme tok = P.peek_l();
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
        err::out("expected ']' found - ", P.peek_l());
        return false;
    }
    ResultantNode = ListExpr::Create(tok, List);
    return true;
}


bool Parser::ParseCastExpr(ParserHelper &P, Ast *&ResultantNode) {
    Ast* LHS = ResultantNode;
    Lexeme tok = P.peek_l();
    Tok op = P.peek_t();
    Ast* RHS;
    while(P.checkn(AS)){
        if(!ParseType(P,ResultantNode)){
            return false;
        }
        ResultantNode = Expression::Create(LHS,op,RHS,NodeAsExpr);
    }
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
    if(!ParseExpr2(P, ResultantNode)){
        return false;
    }
    Tok t = P.peek_t();
    if(t.IsBinaryOP()||t.IsAssignOP()) {

        if(!ParseBinaryExpr(P,ResultantNode,Precedance)) {
            return false;
        }
    }
    return true;
}



bool Parser::ParseExpr2(ParserHelper &P, Ast *&ResultantNode) { 
    if(!ParseExpr3(P, ResultantNode)){
        return false;
    }

    if(P.check(AS)) {
        if(!ParseCastExpr(P,ResultantNode)){
            return false;
        }
    }
    
    return true;
}



bool Parser::ParseExpr3(ParserHelper &P, Ast *&ResultantNode) { 
    Lexeme tok = P.peek_l();
    Tok op = P.peek_t();
    if(op.IsUnaryOP()){
        if(P.check(STAR)) {
            int defCount = 1;
            while(P.checkn(STAR)){
                defCount++;
            }
            if(!ParseExpr3(P,ResultantNode)){
                return false;
            }
            ResultantNode = PrefixExpr::Create(tok, op, ResultantNode, false, defCount, NodeDeref);
        }else if(P.check(AND)) {
            if(!ParseExpr3(P,ResultantNode)){
                return false;
            }
            ResultantNode = PrefixExpr::Create(tok, op, ResultantNode, false, 0, NodeRef);
        }else if(P.check(PLUS)||P.check(MINUS)) {
            if(!ParseExpr3(P,ResultantNode)){
                return false;
            }
            ResultantNode = PrefixExpr::Create(tok, op, ResultantNode, false, 0, NodePrefix);
        }
    }else if(!ParseExpr4(P, ResultantNode)) {
        return false;
    }

    return true;
}





bool Parser::ParseExpr4(ParserHelper &P, Ast *&ResultantNode) {
    if(!ParseExpr5(P, ResultantNode)){
        return false;
    }

    if(P.check(LPAREN) || P.check(LBRACK)){
         while(!P.check(FEOF)&&(P.check(LBRACK)||P.check(LPAREN))){
            if(P.check(LPAREN)) {
                if(!ParseCall(P,ResultantNode)){
                    return false;
                }
            }else if(P.check(LBRACK)) {
                if(!ParseArrayIndexExpr(P,ResultantNode)){
                    return false;
                }
            }
        }
        
    }else if(P.checkh(DOTDOT)) {
            ///@todo
    }else if(P.check(COL)&&(P.checkh(LT)||P.checkh(LBRACE))) {
        if(!ParseStructExpr(P,ResultantNode)) {
            return false;
        }
    }
    
    
    return true;
}


bool Parser::ParseExpr5(ParserHelper &P, Ast *&ResultantNode) {
    if(!ParseExpr6(P,ResultantNode)){
        return false;
    }

    if(P.check(DOT) || P.check(ARROW)){
        if(!ParseDotArrowExpr(P, ResultantNode)) {
            return false;
        }
    }
 
    
    return true;
}


bool Parser::ParseExpr6(ParserHelper &P, Ast *&ResultantNode) {
    if(!ParseExpression(P,ResultantNode)){
        return false;
    }

    Tok Op = P.peek_t();
    while(P.checkn(COLCOL)) {
        Ast* Lhs = ResultantNode;

        if(!ParseExpression(P,ResultantNode)){
            return false;
        }
        
        ResultantNode = Expression::Create(Lhs, Op, ResultantNode, NodePath);
    }

    return true;
}


bool Parser::ParseExpression(ParserHelper &P, Ast *&ResultantNode) {
    Lexeme L = P.peek_l();
    if(P.check(IDEN)||P.check(SELF)){
        if(!ParseIdentifier(P,ResultantNode)){
            return false;
        }
    }else if(P.check(LBRACK)) {
        if(!ParseListExpr(P,ResultantNode)) {
            return false;
        }
    }else if(P.check(LPAREN)) {
        if(!ParseParenExpr(P,ResultantNode)) {
            return false;
        }
    }else if(P.peek_t().isLiteral()){
        if(!ParseLiteral(P,ResultantNode)){
            return false;
        }
    }

    return true;
}




bool Parser::ParseLiteral(ParserHelper &P, Ast *&ResultantNode) {
    Lexeme t = P.peek_l();
    switch (P.peek_tt())
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

bool Parser::ParseParenExpr(ParserHelper &P, Ast *&ResultantNode) {
    P.next();
    P.dump(__func__);
    Ast* node;
    if(P.check(RPAREN)){
        err::out("expected expression found -> ",P.peek_l());
        return false;
    }
    if(!ParseExpr(P, ResultantNode)){
        return false;
    }
    node = ResultantNode;

    if(!P.checkn(RPAREN)){
        err::out("expected ')' found -> ",P.peek_l());
        return false;
    } 
    ResultantNode = GroupedExpr::Create(ResultantNode);
            P.dump2(__func__);

    return true;
}

bool Parser::ParseIdentifier(ParserHelper &P, Ast *&ResultantNode){
    bool HasSelf = false;
    Lexeme tok = P.peek_l();
    if(P.check(SELF)){
        HasSelf = true;
    }
    ResultantNode = Identifier::Create(tok);
    P.next();
    return true;
}



bool Parser::ParseDotArrowExpr(ParserHelper &P, Ast *&ResultantNode) {
    while(P.check(DOT)||P.check(ARROW)){
        Lexeme tok = P.peek_l();
        Tok op = P.peek_t();
        Ast* LHS = ResultantNode;
        P.next();
        if(!ParseExpr6(P,ResultantNode)){
            return false;
        }
        ResultantNode = Expression::Create(LHS, op, ResultantNode, NodeMemExpr);
    }
    return true;
}


bool Parser::ParseStructExpr(ParserHelper &P, Ast *&ResultantNode) {
    P.dump(__func__);      
    bool isDecl = false;
    Lexeme tok = P.peek_l();
    Ast* sig = ResultantNode;
    GenericParam* Temp;
    std::vector<Ast*> Iden;
    std::vector<Ast*> Vals;

    if(P.checkn(LT)){
        if(!ParseGenericParam(P, Temp)) {
            return false;
        }
    }

    if(!P.checkn(LBRACE)){
        err::out("expected '{' found -> ",P.peek_l());
        return false;
    }

    while(!P.check(RBRACE)){
        if(!P.check(IDEN)){
            err::out("expected 'identifier' found - ",P.peek_l());
            return false;
        }
        if(!ParseIdentifier(P,ResultantNode)){
            return false;
        }
        Iden.push_back(ResultantNode);
        P.next();
        if(!P.check(COL)){
            err::out("expected ':' found - ",P.peek_l());
            return false;
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
        err::out("expected '}' found -> ",P.peek_l());
        return false;
    }
    ResultantNode = StructStmt::Create(tok,sig, Temp, Iden,Vals, NodeStructDef);
    P.dump2(__func__);
    return true;
}

bool Parser::ParseArrayIndexExpr(ParserHelper &P, Ast *&ResultantNode) {
    Lexeme tok = P.peek_l();
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
        err::out("expected ']' found -> ",P.peek_l());
        return false;
    }

    ResultantNode = IndexExpr::Create(tok,Ident,IndexVal);
}



bool Parser::ParseBinaryExpr(ParserHelper &P, Ast *&ResultantNode, int prev_prece) {
    P.dump(__func__);
    Lexeme tok = P.peek_l();
    Ast* left;
    if(!ResultantNode){
        if(!ParseExpr1(P,ResultantNode,prev_prece)){
            return false;
        }
        left = ResultantNode;
    }
    left = ResultantNode;

    //2+3*3/4+2*3
    Tok opr = P.peek_t();
    while(opr.IsBinaryOP()||opr.IsAssignOP()) {
        int CurPrecedance = P.preced(opr.getTokType());
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

bool Parser::ParseGenericParam(ParserHelper &P, GenericParam *&ResultantNode){
    if(P.checkn(LT)){
        std::vector<Lexeme>Temp;
        while(P.check(IDEN)){
            Temp.push_back(P.peek_l());
            if(!P.checkn(COMMA))
                break;
        }
        if(!P.checkn(GT)){
            err::out("expected '>' found -",P.peek_l());
            return false;
        }

        ResultantNode = GenericParam::Create(Temp);
    }
    return true;
}

bool Parser::ParseSpecificType(ParserHelper &P, Ast *&ResultantNode) {
    Lexeme tok = P.peek_l();
    Ast* Expr = ResultantNode;
    GenericParam* param;
    if(P.check(LT)){
        if(!ParseGenericParam(P,param)){
            return false;
        }

        ResultantNode = UserDefinedTy::Create(tok,Expr,param);
    }
    return true;
}

bool Parser::ParseEnumStmt(ParserHelper &P, Ast *&ResultantNode) {
    std::vector<Lexeme>udata;
    std::vector<Ast*>val;
    if(!P.checkn(ENUM)){
        err::out("expected 'enum' keyword found -",P.peek_l());
        return false;
    }
    if(!P.check(IDEN)) {
        err::out("expected 'identifier' found -",P.peek_l());
        return false;
    }
    Lexeme Name = P.peek_l();

    if(!P.checkn(LBRACE)){
        err::out("expected '{' found ->",P.peek_l());
        return false;
    }

    while(P.check(IDEN)){
        udata.push_back(P.peek_l());
        if(P.checkn(ASN)){
            if(!ParseExpr(P, ResultantNode)){
                return false;
            }
            val.push_back(ResultantNode);
        }
        if(!P.checkn(COMMA)){
            err::out("expected ',' found ->",P.peek_l());
            return false;
        }
    }

    if(!P.checkn(RBRACE)){
        err::out("expected '}' found ->",P.peek_l());
        return false;
    }
    ResultantNode = EnumExpr::Create(Name, udata, val);
    return true;
}


bool Parser::ParseStructStmt(ParserHelper &P, Ast *&ResultantNode) {
    P.dump(__func__);
    Lexeme tok = P.peek_l();
    std::vector<Ast*> ElementName;
    std::vector<Ast*> ElementTy;
    if(!P.checkn(STRUCT)) {
        err::out("expected keyword 'struct' found ->",P.peek_l());
        return false;
    }

    if(!P.check(IDEN)) {
        err::out("expected 'identifier' found ->",P.peek_l());
        return false;
    }

    Ast* Ident;
    if(!ParseIdentifier(P, Ident)){
        return false;
    }
  
    GenericParam* param;
    if(!ParseGenericParam(P,param)){
        return false;
    }

    if(!P.check(LBRACE)){
        err::out("expected field expression '{' found -> ",P.peek_l());
        return false;
    }

    while(P.peek_tt() !=  FEOF && P.check(IDEN)){
        // Identifier* ident = Identifier::Create();
        Ast* ident;
        if(!ParseIdentifier(P,ident)){
            return false;
        }
        ElementName.push_back(ident);
        if(P.checkn(COL)){
            err::out("expected struct field expression ':' found - ",P.peek_l());
            return false;
        }
        if(!ParseType(P,ResultantNode)){
            err::out("expected struct field expression ': ?type' found - ",P.peek_l());
            return false;
        }
        ElementTy.push_back(ResultantNode);
        if(!P.checkn(COMMA)){
            break;
        }
    }

    if(!P.checkn(RBRACE)){
        err::out("expected '}' found -> ",P.peek_l());
        return false;
    }

    ResultantNode = StructStmt::Create(tok, Ident, param, ElementName, ElementTy, NodeStructStm);
    P.dump2(__func__);

    return true;
}




bool Parser::ParseFuncDef(ParserHelper &P, Ast *&ResultantNode) {
     P.dump(__func__);
    Lexeme tok = P.peek_l();
    std::vector<Ast*>pN;
    std::vector<Ast*>pTy;
    Ast* RetVal = nullptr;
    Ast* StructVal = nullptr;
    Ast* Block = nullptr;

    if(!P.checkn(FN)) {
        err::out("expected keyword 'fn' found ->\'",P.peek_l());
        return false;
    }
    if(!P.check(IDEN)){
        err::out("expected 'Identifier' with function defination found ->\'",P.peek_l());
        return false;
    }   
    Lexeme Name = P.peek_l();

    if(P.checkn(LPAREN)) {
        err::out("expected function expression (parameters : type) found -> ",P.peek_l());
        return false;
    }
    while(P.check(IDEN)) {

        if(!ParseIdentifier(P,ResultantNode)){
            return false;
        }

        pN.push_back(ResultantNode);
        if(!P.checkn(SCOL)){
            err::out("expected ':' found -> \'",P.peek_l());
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
        err::out("expected ')' found -> ",P.peek_l());
        return false;
    }

    if(P.checkn(ARROW)){
        if(!ParseType(P,ResultantNode)){
            err::out("expected 'type' found -> ",P.peek_l());
            return false;
        }
        RetVal = ResultantNode;
    }


    if(P.check(LBRACE)) {
        if(!ParseBlock(P, ResultantNode,false)){
            return false;
        }
        Block = ResultantNode;
    }else{
        err::out("expected function body expression '{}' found - ",P.peek_l());
        return false;
    }
    ResultantNode = FunctionDef::Create(tok,Name, pN, pTy, RetVal, Block);
    Mod.moditem.addItem(Name.getStr(),ResultantNode);
    P.dump2(__func__);
    return true;
}

bool Parser::ParseIfStmt(ParserHelper &P, Ast *&ResultantNode) {
    P.dump(__func__);
    Lexeme tok = P.peek_l();
    if(!P.checkn(IF)){
        err::out("expected 'if' found - ",P.peek_l());   
        return false;
    }
    Ast* condition = nullptr;
    Ast* if_ = nullptr;
    Ast* else_ = nullptr;

    if(P.check(LBRACE)) {
        err::out("expected 'condition' found - ",P.peek_l());
        return false;
    }

    if(!ParseExpr(P, ResultantNode)){
        return false;
    }

    condition = ResultantNode;

    // if(P.checkn(COL)){
    //     //todo
    // }


    if(!ParseBlock(P, ResultantNode,false)){
        return false;
    }
    if_ = ResultantNode;

    if(P.checkn(ELSE)) {
        switch(P.peek_tt()){
            case IF:
                if(!ParseIfStmt(P,ResultantNode)){
                    return false;
                }
                else_ = ResultantNode;
                break;
            case LBRACE:
                if(!ParseBlock(P, ResultantNode, false)){
                    return false;
                }
                else_ = ResultantNode;
                break;
            default:
                err::out("invalid else expression ",P.peek_l());
                return false;

        }
    }
    ResultantNode = IfStmt::Create(tok,condition, if_, else_);
            P.dump2(__func__);

    return true;
}

bool Parser::ParseForStmt(ParserHelper &P, Ast *&ResultantNode){
    Lexeme tok = P.peek_l();
    Ast* var = nullptr;
    Ast* cond = nullptr;
    Ast* incr = nullptr;
    Ast* body = nullptr;
    if(!P.checkn(FOR)) {
        err::out("expected keyword 'for' found - ",P.peek_l());
        return false;
    }

    if(P.checkn(SCOL)){
        goto cond;
    }
    if(!P.check(LET)){
        err::out("expected for expression 'identifier' found - ",P.peek_l());
        return false;
    }
    if(!ParseVarStmt(P, ResultantNode)){
        return false;
    }
    var = ResultantNode;
    if(!P.checkn(SCOL)){
        err::out("expected for expression ';' found - ",P.peek_l());
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
        err::out("expected for expression ';' found - ",P.peek_l());
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
    if(!ParseBlock(P,ResultantNode,false)){
        return false;
    }
    body = ResultantNode;
    ResultantNode = ForLoop::Create(tok, var, cond, incr, body);
    return true;
}


// ref from rust lang and scribe
bool Parser::ParseForInStmt(ParserHelper &P, Ast *&ResultantNode) {
    P.dump(__func__);
    Lexeme tok = P.peek_l();
    Ast* in = nullptr;
    Ast* var = nullptr;
    Ast* cond = nullptr;
    Ast* incr = nullptr;
    Ast* body = nullptr;
    // bool isBrace = true;
    if(!P.checkn(FOR)) {
        err::out("expected keyword 'for' found - ",P.peek_l());
        return false;
    }

    if(!P.check(IDEN)){
        err::out("expected 'identifier' found - ",P.peek_l());
        return false;
    }

    Lexeme iter_tempvar__tok = P.peek_l();
    if(!ParseIdentifier(P,ResultantNode)){
        return false;
    }
    Ast* ident = ResultantNode;


    if(!P.checkn(IN)){
        err::out("expected keyword 'in' found -> ",P.peek_l());
        return false;
    }
    // 0..1 , iter

    if(P.check(LBRACE)){
        err::out("expected 'expression' found -> ",P.peek_l());
        return false;
    }
    if(P.check(INT)){
        Ast *from;
        Ast *to;
        Ast *intlit;
        from = NumericLiteral::Create(P.peek_l());
        P.next();
        if(!P.checkn(DOTDOT)){
            err::out("expected for-in expression '..' found - ",P.peek_l());
            return false;
        }

        if(!P.check(INT)){
            err::out("expected for-in expression integer after '..' found - ",P.peek_l());
            return false;
        }
        to = NumericLiteral::Create(P.peek_l());;
        P.next();

        if(P.checkn(DOTDOT)){
            if(!P.check(INT)){
                err::out("expected for-in expression integer after '..' found - ",P.peek_l());
                return false;
            }

            intlit = NumericLiteral::Create(P.peek_l());;
            P.next();
        }else{
            Lexeme t = Lexeme("1",INT);
            intlit = NumericLiteral::Create(t);
        }
        
        if(!ParseBlock(P,ResultantNode,false)){
            return false;
        }
        body = ResultantNode;
        var = VarStmt::Create(iter_tempvar__tok,ident, nullptr, from,false);
        cond = Expression::Create(ident, LT, to,NodeBinaryExpr);
        incr = Expression::Create(ident, ASN_PLUS, intlit,NodeBinaryExpr);
        ResultantNode = ForLoop::Create(tok, var, cond, incr, body);
        return true; 

    }else{    
        if(!ParseExpr(P,ResultantNode)) {
            err::out("for-in use only { 1..5..1 , .iter()} -> ",P.peek_t());
            return false;
        }
        if(ResultantNode->nodeCategory() != NodeMemExpr){
            err::out("expected 'iterator' expression found -> ",P.peek_t());
            return false;
        }
        in = ResultantNode;

        bool isBrace = true;
        if(!ParseBlock(P,ResultantNode,isBrace)){
            return false;
        }
        body = ResultantNode;
        
        ///<iterVar> = <vector.iter()>
        Lexeme iter_var_tok = Lexeme("iterVar", IDEN);
        Lexeme iter__tempvar__tok = Lexeme("_"+ident->toString(), IDEN);
        Lexeme iter_begin_tok = Lexeme("begin", IDEN);
        Lexeme iter_end_tok = Lexeme("end", IDEN);
        Lexeme iter_next_tok = Lexeme("next", IDEN);
        Lexeme iter_at_tok = Lexeme("at", IDEN);

        Ast* iter_begin_Iden = Identifier::Create(iter_begin_tok);
        Ast* iter_end_Iden = Identifier::Create(iter_end_tok);
        Ast* iter_next_Iden = Identifier::Create(iter_next_tok);
        Ast* iter_at_Iden = Identifier::Create(iter_at_tok);

        Ast* iter_tempvar_ident = Identifier::Create(iter__tempvar__tok);
        Ast* iter_var_ident = Identifier::Create(iter_var_tok);

        Ast* iter_decl = VarStmt::Create(iter_var_tok, iter_var_ident, nullptr, in, false);

        Ast* begin_call = FunctionCall::Create(iter_begin_tok,iter_begin_Iden,{});
        Ast* iter_dot_begin_call = Expression::Create(iter_var_ident, DOT, begin_call, NodeMemExpr);
        var = VarStmt::Create(iter__tempvar__tok, iter_tempvar_ident, nullptr, iter_dot_begin_call,false);

        ///<_temp> != <iterVar.end()>
        Ast* end_call = FunctionCall::Create(iter_end_tok, iter_end_Iden, {});
        Ast* iter_end_call = Expression::Create(iter_var_ident, DOT, end_call, NodeMemExpr);
        cond = Expression::Create(iter_tempvar_ident, NEQL, iter_end_call, NodeBinaryExpr);


        ///<iterVar.next(_temp)>
        Ast* nextcall = FunctionCall::Create(iter_next_tok, iter_next_Iden, {iter_tempvar_ident});
        Ast* iternext = Expression::Create( iter_var_ident, DOT, nextcall, NodeMemExpr);
        incr = Expression::Create(iter_tempvar_ident, ASN, iternext, NodeAssnExpr);


        ///<var = _iterVar.at()>
        Ast* at_call = FunctionCall::Create(iter_at_tok, iter_at_Iden, {iter_tempvar_ident});
        Ast* iter_at_call = Expression::Create(iter_var_ident, DOT, at_call, NodeMemExpr);
        Ast* inside_loop_var_decl = VarStmt::Create(iter_tempvar__tok, ident, nullptr, iter_at_call, false);

        
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
    Lexeme tok = P.peek_l();
    P.next();
    std::vector<Ast*>Header;
    if(!P.checkn(LBRACK)){
        err::out("expected '[' found - ",P.peek_l());
        return false;
    }
    if(!P.check(IDEN)) {
        err::out("expected 'identifier' found - ",P.peek_l());
        return false;
    }
    if(!ParseIdentifier(P,ResultantNode)){
        return false;
    }
    Header.push_back(ResultantNode);
    if(!P.check(STR)) {
        err::out("expected 'string' found - ",P.peek_l());
        return false;
    }
    if(!ParseLiteral(P,ResultantNode)){
        return false;
    }
    Header.push_back(ResultantNode);
    if(!P.checkn(RBRACK)){
        err::out("expected ']' found - ",P.peek_l());
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
        err::out("expected fn and struct statement block with extern found - ",P.peek_l());
        return false;
    }
    Header.push_back(ResultantNode);

    ResultantNode = Extern::Create(tok, Header);
    return true;
}


bool Parser::ParseVarStmt(ParserHelper &P, Ast *&ResultantNode) {
    Lexeme tok = P.peek_l();
    Ast* Var;
    Ast* Ty;
    Ast* Val;
    bool isty = false;
    bool isval = false;
    bool isConst = false;
    bool isImmut = false;
    bool isLet = false;
    bool isStatic = false;
    if(P.checkn(LET)){
        isLet = true;
    }else if(P.checkn(STATIC)){
        isConst = true;
    }else if(P.checkn(CONST)){
        isConst = true;
    }else{
        err::out("expected {const, let, static} found - ",P.peek_l());
        return false;
    }

    if(!P.check(IDEN)){
        err::out("expected 'identifier' found - ",P.peek_l());
        return false;
    }
    if(!ParseIdentifier(P,Var)){
        return false;
    }

    if(!P.checkn(COL)){
        goto val;
    }
    
    isty = true;
    if(!ParseType(P,Ty)){
        return false;
    }
    Ty = ResultantNode;

val:  
    if(!P.checkn(ASN)){
        goto done;
    }
    isval = true;
    if(!ParseExpr(P, Val)){
            return false;
    }

done:
    if(!isval && !isty){
        err::out("invalid variable declaration - no type or value set", P.peek_t());
        return false;
    }
    
    if(isConst) {
        if(!isval||!isty) {
            err::out("missing type for `const` item - ", P.peek_t());
            err::out("free constant item without body - ", P.peek_t());
    
            return false;
        }
        if(!isval) {
            err::out("free constant item without body - ", P.peek_t());
    
            return false;
        }
        if(isImmut) {
            err::out("const globals cannot be used with - ", P.peek_t());
    
            return false;
        }
        if(!isty) {
            err::out("missing type for `const` item - ", P.peek_t());
    
            return false;
        }
    }
    
    if(isStatic) {
        if(!isval||!isty) {
            err::out("missing type for `static` item - ", P.peek_t());
            err::out("free constant item without body - ", P.peek_t());

            return false;
        }
        if(!isty) {
            err::out("missing type for `static` item - ", P.peek_t());
    
            return false;
        }
        if(!isval) {
            err::out("free static item without body - ", P.peek_t());
    
            return false;
        }
    }


    ResultantNode = VarStmt::Create(tok, Var, Ty, Val, isConst);
    return true;
}
} // namespace parser
