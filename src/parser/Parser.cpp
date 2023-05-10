
#include"../../include/parser/Parser.hpp"
#include"../../include/Error.hpp"
// #include"parser/Parser.hpp"
// #include"Error.hpp"

namespace parser
{

AstPtr Parser::parse() {
    bool isBrace = false;
    if(!ParseBlock(isBrace)) {
        return nullptr;
    }
    return ResultantNode;
}

bool Parser::ParseBlock(bool isBrace){
    std::vector<AstPtr>stmts;
    if(isBrace&&!checkn(LBRACE)){
        err::out("expected '{' found",toks[CurrentIndex]);
        return false;
    }
    while(!check(FEOF)&&(!isBrace&&!check(RBRACE))) {
        bool isSemiCol = false;
        switch(CurrentToken){ 
        case IF:
            if(!ParseIfStmt()){ return false; }
            break;
        case FOR:
            if(checkNH(IN, 2)){
                if(!ParseForInStmt()){ return false; }
            }else{
                if(!ParseForStmt()){ return false; }
            }
            break;
        case FN:
        {
            if(!ParseFuncDef()){
                return false;
            }
        }
            break;
        case WHILE:
            if(!ParseWhileStmt()){ return false;}
            break;
        case LET:
            if(!ParseVarStmt()){ return false; }
            isSemiCol = true;
            break;

        case RETURN:
            if(!ParseReturnStmt()){ return false; }
            isSemiCol = true;
            break;

        case CONTINUE:
        case BREAK:
            stmts.push_back(std::make_shared<BranchStmt>(toks[CurrentIndex]));
            next();
            isSemiCol = true;
            break;
        case TYPE:
            if(!ParseTypeStatm()){ return false; }
            isSemiCol = true;
            break;
        case IDEN:
            if(!ParseExpr()) { return false;}
            isSemiCol = true;
            break;
        default:
            
            break;
        }
        if(isSemiCol&&!checkn(SCOL)){
            err::out("expected ';' found - ",toks[CurrentIndex]);
            return false;
        }
        stmts.push_back(ResultantNode);
    }
    if(isBrace&&!checkn(RBRACE)){
        err::out("expected '}' found",toks[CurrentIndex]);
        return false;
    }
    ResultantNode = std::make_shared<BlockStatement>(stmts);
    return true;
}

bool Parser::ParseWhileStmt() {
    tokt tok = toks[CurrentIndex];
    AstPtr Cond;
    next();
    if(check(LBRACE)){
        err::out("expected condition expression found - ",toks[CurrentIndex]);
        return false;
    }
    if(!ParseExpr()) {
        return false;
    }
    Cond = ResultantNode;
    bool isBrace = true;
    if(!ParseBlock(isBrace)){
        return false;
    }
    ResultantNode = std::make_shared<WhileLoop>(tok,Cond,ResultantNode);
    return true;
}


bool Parser::ParseReturnStmt() {
    dump(__func__);
    tokt tok = toks[CurrentIndex];
    AstPtr val;
    next();

    if(!check(SCOL)){
        if(!ParseExpr()) {
            return false;
        }
        val = ResultantNode;
    }
    if(!checkn(SCOL)){
            err::out("expected ';' found -> ",toks[CurrentIndex]);
            return false;
    }

    next();
    ResultantNode = std::make_shared<ReturnStmt>(tok, val);
    dump2(__func__);

    return true;
}


bool Parser::ParseType() {
    //AstPtr type;
    dump(__func__);
    switch(CurrentToken){
        case LBRACK:
            if(!ParseArrayType()){
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
            ResultantNode = std::make_shared<PreDefineType>(toks[CurrentIndex]);
            next();
            break;
        case CONST:
        {
            tokt tok = toks[CurrentIndex];
            Token_type Op = CurrentToken;
            bool isType = true;
            next();
            if(!ParseType()){
                return false;
            }
            ResultantNode = std::make_shared<PrefixExpr>(tok,Op,ResultantNode,isType, NodeConstTy);

        }
            break;
        case FN:
        {
            tokt tok = toks[CurrentIndex];
            next();
            if(!check(LPAREN)){
                err::out("expected '{' found -> ",toks[CurrentIndex]);
                return false;
            }else{
                next();
                std::vector<AstPtr>ty;
                while(!check(RPAREN)){
                    if(!ParseType()) {
                        return false;
                    }
                    ty.push_back(ResultantNode);
                    if(checkn(COMMA)) {
                        break;
                    }
                }
                if(!checkn(RPAREN)) {
                    err::out("expected ')' found -> ",toks[CurrentIndex]);
                    return false;
                }
                AstPtr ret;
                if(checkn(ARROW)){
                    if(ParseType()){
                        ret = ResultantNode;
                    }
                }
                ResultantNode = std::make_shared<FunType>(tok, ty, ret);

            }
        }
            break;
        case IDEN:
            if(!ParseSpecificType()){
                return false;
            }
            break;
        case STAR:
        {
            bool isType  = true;
            tokt tok  = toks[CurrentIndex];
            Token_type op = CurrentToken;
            next();
            if(!ParseType()){
                return false;
            }
            ResultantNode = std::make_shared<PrefixExpr>(tok,op,ResultantNode,isType,NodePtr);
        }
            break;
        case SELF:
        {   
            if(!ParseIdentifier()) {
                return false;
            }
        }
        break;
        default:
            err::out("expected 'type' found ->",toks[CurrentIndex]);
            return false;
    }
            dump2(__func__);
    return true;
}


bool Parser::ParseArrayType() {
    tokt tok = toks[CurrentIndex];
    std::vector<AstPtr>size;
    next();
    if(check(RBRACK)) {
        err::out("expected ']' found ->",toks[CurrentIndex]);
        return false;
    }
    while(check(RBRACK)) {
        if(!ParseExpr()) {
            return false;
        }
        size.push_back(ResultantNode);
        if(!check(SCOL)) {
            break;
        }
    }
    if(!checkn(RBRACK)) {
        err::out("expected ']' found ->",toks[CurrentIndex]);
        return false;
    }
    if(!ParseType()){
        return false;
    }
    ResultantNode = std::make_shared<ArrayTy>(tok,size,ResultantNode);
    return true;
}


bool Parser::ParseTypeStatm(){
    tokt tok = toks[CurrentIndex];
    next();
    if(!check(IDEN)){
        err::out("expected 'identifier' found - ",toks[CurrentIndex]);
        return false;
    }
    tokt Ident = toks[CurrentIndex];
    next();
    if(!checkn(IS)){
        err::out("expected 'is' found - ",toks[CurrentIndex]);
        return false;
    }

    if(!ParseType()){
        return false;
    }
    ResultantNode = std::make_shared<TypeStmt>(tok, Ident, ResultantNode);
    return true;
}


bool Parser::ParseCall() {
    tokt tok = toks[CurrentIndex];
    AstPtr sig = ResultantNode;
    std::vector<AstPtr>args;

    if(!checkn(LPAREN)){
        err::out("expected '(' found - ", toks[CurrentIndex]);
        return false;
    }

    while(!check(FEOF)){
        if(!ParseExpr()) {
            return false;
        }

        args.push_back(ResultantNode);
        if(!args.back()){
            return false;
        }

        if(!checkn(COMMA)) {
            break;
        }
    }

    if(!checkn(RPAREN)){
        err::out("expected ')' found - ", toks[CurrentIndex]);
        return false;
    }
    ResultantNode = std::make_shared<FunctionCall>(tok,sig,args);
    return true;
}


bool Parser::ParseListExpr() {
    tokt tok = toks[CurrentIndex];
    next();
    std::vector<AstPtr>List;
    while(!check(RBRACK)) {
        if(!ParseExpr()){
            return false;
        }
        List.push_back(ResultantNode);
        if(!checkn(COMMA)){
            break;
        }
    }
    if(!checkn(RBRACK)){
        err::out("expected ']' found - ", toks[CurrentIndex]);
        return false;
    }
    ResultantNode = std::make_shared<ListExpr>(tok, List);
    return true;
}


bool Parser::ParseCastExpr() {
    AstPtr LHS = ResultantNode;
    tokt tok = toks[CurrentIndex];
    Token_type op = CurrentToken;
    AstPtr RHS;
    while(checkn(AS)){
        if(!ParseType()){
            return false;
        }
        ResultantNode = std::make_shared<Expression>(tok,LHS,op,RHS,NodeAsExpr);
    }
    return true;
}


bool Parser::ParsePrefixExpr(KindExpression Expr) {
    tokt tok = toks[CurrentIndex];
    Token_type op = CurrentToken;
    NodeCategory NodeKind;
    int DefCount = 0;
    if(checkn(STAR)) {
        DefCount = 0;
        while(checkn(STAR)){
            DefCount++;
        }
        if(check(AND_OP)){
            if(!ParsePrefixExpr(Expr)){
                return false;
            }
        }else if(check(IDEN)||check(LPAREN)){
            if(!ParsePrePostExpr(Expr)){
                return false;
            }
        }else {
            err::out("invalid expression with operator - ", toks[CurrentIndex]);
            return false;
        }
        NodeKind = NodePtr;
    }else if(checkn(AND_OP)) {
        if(check(IDEN)||check(LPAREN)){
            if(!ParsePrePostExpr(Expr)){
                return false;
            }
        }else if(check(LBRACK)) {
            if(!ParseListExpr()){
                return false;
            }
        }else {
            err::out("invalid expression with operator - ", toks[CurrentIndex]);
            return false;
        }
        NodeKind - NodeRef;
    }else if(checkn(PLUS)||checkn(MINUS)||checkn(NOT)||checkn(NOT_OP)) {
        if(!check(IDEN)&&!check(LPAREN)&&!isLiteral(CurrentToken)){
            err::out("invalid expression with operator - ", toks[CurrentIndex]);
            return false;
        }
        if(!ParseExpr()){
            return false;
        }
        NodeKind = NodePrefix;
    }
    
    ResultantNode = std::make_shared<PrefixExpr>(tok,op,ResultantNode,NodeKind);
    return true;
}

bool Parser::ParseExpr() {
    int Precedance = 0;
    if(!ParseExpr2(Precedance)){
        return false;
    }
    return 0;
}
 

bool Parser::ParseExpr2(int &Precedance) {
    KindExpression Expr;
    if(!ParseExpression(Precedance, Expr)){
        return false;
    }

    if(check(AS)) {
        if(Expr == KExpr_ArrayInit || Expr == KExpr_StructInit || Expr == KExpr_DotDot){
            err::out("Arithmetic operation not allowed with - ",toks[CurrentIndex]);
            return false;
        }
        if(ParseCastExpr()) {
            return false;
        }
    }

    if(ArithmeticOP(CurrentToken)) {
        if(Expr == KExpr_ArrayInit || Expr == KExpr_StructInit || Expr == KExpr_DotDot){
            err::out("Arithmetic operation not allowed with - ",toks[CurrentIndex]);
            return false;
        }
        if(!ParseBinaryExpr(Precedance)) {
            return false;
        }
    }else if(LogicalOP(CurrentToken)) {
        if(Expr == KExpr_ArrayInit || Expr == KExpr_StructInit || Expr == KExpr_DotDot){
            err::out("Logical operation not allowed with - ",toks[CurrentIndex]);
            return false;
        }
        if(!ParseBinaryExpr(Precedance)) {
            return false;
        }

    }if(ConditionalOP(CurrentToken)) {
        if(Expr == KExpr_ArrayInit || Expr == KExpr_StructInit || Expr == KExpr_DotDot){
            err::out("Conditional operation not allowed with - ",toks[CurrentIndex]);
            return false;
        }
        if(!ParseBinaryExpr(Precedance)) {
            return false;
        }
    }else if(AssignOP(CurrentToken)) {
        if(Expr == KExpr_ArrayInit || Expr == KExpr_StructInit || Expr == KExpr_DotDot){
            err::out("Assignment operation not allowed with - ",toks[CurrentIndex]);
            return false;
        }
        if(!ParseBinaryExpr(Precedance)) {
            return false;
        }
    }
    return true;
}


bool Parser::ParseExpression(int &Precedance, KindExpression &Expr) {
    switch (CurrentToken)
    {
    case SELF:
    {
        switch (checkh(CurrentToken))
        {
        case ARROW:
        case DOT:
        {
            if(!ParsePrePostExpr(Expr)) {
                return false;
            }
        }
            break;
        default:
            if(!ParseIdentifier()){
                return false;
            }
            Expr = KExpr_Self;
            break;
        }
    }
        break;
    case IDEN:
        switch (checkh(CurrentToken))
        {
        case LPAREN:
        case LBRACK:
        case ARROW:
        case DOT:
        {
            int OldPrecedance = Precedance;
            Precedance = 0;
            if(!ParsePrePostExpr(Expr)) {
                return false;
            }
            Precedance = OldPrecedance;
            break;
        }
        case DOTDOT:
        {
            tokt tok = toks[CurrentIndex];
            Token_type op = CurrentToken;
            ResultantNode = std::make_shared<Expression>(tok, ResultantNode, op, nullptr, NodeDotDotExpr);
            next();
            Expr = KExpr_DotDot;
        }
        break;
        case LT:
        case LBRACE:
            if(!ParseStructExpr()) {
                return false;
            }
            Expr = KExpr_StructInit;
            break;
        default:
            if(!ParseIdentifier()){
                return false;
            }
            Expr = KExpr_Identifier;
            break;
        }
        break;
    case LBRACK:
        if(!ParseListExpr()) {
                return false;
        }
        Expr = KExpr_ArrayInit;
        break;
    case LPAREN:
    {
        if(!ParsePrePostExpr(Expr)) {
            return false;
        }
        Expr = KExpr_Grouped;
    }   
    break;
    case STAR:
    case PLUS:
    case AND_OP:
    case MINUS:
    case NOT:
    case NOT_OP:
        if(!ParsePrefixExpr(Expr)) {
                return false;
        }
        break;
    case INT:
    {
        ResultantNode = std::make_shared<NumericLiteral>(toks[CurrentIndex]);
        next();
    }
    case FLOAT:
    {
        ResultantNode = std::make_shared<FloatLiteral>(toks[CurrentIndex]);
        next();
    }
    case STR:
    case CHAR:
    {
        ResultantNode = std::make_shared<StringLiteral>(toks[CurrentIndex]);
        next();
    }
    case TRUE:
    case FALSE:
    {
        ResultantNode = std::make_shared<BoolLiteral>(toks[CurrentIndex]);
        next();
    }
    default:
        break;
    }
    return true;
}


bool Parser::ParseParenExpr() {
    next();
    dump(__func__);
    AstPtr node;
    if(check(RPAREN)){
        err::out("expected expression found -> ",toks[CurrentIndex]);
        return false;
    }
    if(!ParseExpr()){
        return false;
    }
    node = ResultantNode;

    if(!checkn(RPAREN)){
        err::out("expected ')' found -> ",toks[CurrentIndex]);
        return false;
    } 
    ResultantNode = std::make_shared<GroupedExpr>(ResultantNode);
            dump2(__func__);

    return true;
}

bool Parser::ParseIdentifier(){
    bool HasSelf = false;
    tokt tok = toks[CurrentIndex];
    if(check(SELF)){
        HasSelf = true;
    }
    std::make_shared<Identifier>(tok, HasSelf);
    next();
    return true;
}


bool Parser::ParsePrePostExpr(KindExpression &Expr) {
   bool hasDot = false;
   bool hasPostfix = false;
   bool isParen = false;
   if(check(LPAREN)){
        if(!ParseParenExpr()){
            return false;
        }
        Expr = KExpr_GroupedAcc;
        isParen = true;
   }else if(!ParseIdentifier()){
        return false;
   }
    if(!check(LBRACK)&&!check(LPAREN)) {
        goto dot;
    }
    hasPostfix = true;
    if(!ParseIdentPostExpr(Expr)) {
        return false;
    }
dot:
    if(!check(ARROW)&&!check(DOT)) {
        goto terminate;
    }
    hasDot = true;
    if(!ParseDotArrowExpr(Expr)) {
        return false;
    }
    Expr = KExpr_DotArrow;
terminate:
    
    return true;
}


bool Parser::ParseIdentPostExpr(KindExpression &Expr){
    while(!check(FEOF)&&(check(LBRACK)||check(LPAREN))){
        if(check(LPAREN)) {
            if(!ParseCall()){
                return false;
            }
            Expr = KExpr_Call;
        }else if(check(LBRACK)) {
            if(!ParseArrayIndexExpr()){
                return false;
            }
            Expr = KExpr_ArrayAccess;
        }
    }
    return true;
}


bool Parser::ParseDotArrowExpr(KindExpression &Expr) {
    while(check(DOT)||check(ARROW)){
        tokt tok = toks[CurrentIndex];
        Token_type op = CurrentToken;
        AstPtr LHS = ResultantNode;
        next();
        if(!ParsePrePostExpr(Expr)){
            return false;
        }
        ResultantNode = std::make_shared<Expression>(tok, LHS, op, ResultantNode, NodeMemExpr);
    }
    return true;
}


bool Parser::ParseStructExpr() {
    dump(__func__);      
    bool isDecl = false;
    tokt tok = toks[CurrentIndex];
    AstPtr sig = ResultantNode;
    std::vector<tokt> Temp;
    std::vector<AstPtr> Vals;

    if(checkn(LT)){
        // if(!isAllowTemp){
        //     err::out("template with extern not allowed ->",toks[CurrentIndex]);
        //     return false;
        // }
        while(check(IDEN)){
            Temp.push_back(toks[CurrentIndex]);
            if(!checkn(COMMA))
                break;
        }
        if(!checkn(GT)){
            err::out("expected '>' found ->",toks[CurrentIndex]);
            return false;
        }
    }

    if(!checkn(LBRACE)){
        err::out("expected '{' found -> ",toks[CurrentIndex]);
        return false;
    }

    while(!check(RBRACE)){
        if(!ParseExpr()){
            return false;
        }
        Vals.push_back(ResultantNode);
        if(!Vals.back()){
            return false;
        }
        if(!checkn(COMMA)){
            break;
        }
    }

    if(!checkn(RBRACE)) {
        err::out("expected '}' found -> ",toks[CurrentIndex]);
        return false;
    }
    ResultantNode = std::make_shared<PostfixExpr>(tok,sig,Temp,Vals,NodeStructDef);
    dump2(__func__);
    return true;
}

bool Parser::ParseArrayIndexExpr() {
    tokt tok = toks[CurrentIndex];
    AstPtr Ident = ResultantNode;
    std::vector<AstPtr>IndexVal;
    while(!check(FEOF)&&!check(RBRACK)){
        if(!ParseExpr()){
            return false;
        }
        IndexVal.push_back(ResultantNode);
        if(!checkn(SCOL)){
            break;
        }
    }
    if(!checkn(RBRACK)){
        err::out("expected ']' found -> ",toks[CurrentIndex]);
        return false;
    }
    ResultantNode = std::make_shared<PostfixExpr>(tok,Ident,std::vector<tokt>(),IndexVal,NodeIndexExpr);
}


bool Parser::ParseParenExpr() {
    next();
    dump(__func__);
    AstPtr node;
    if(check(RPAREN)){
        err::out("expected expression found -> ",toks[CurrentIndex]);
        return false;
    }else if(!ParseExpr()){
        return false;
    }
    node = ResultantNode;

    if(CurrentToken != RPAREN){
        err::out("expected ')' found -> ",toks[CurrentIndex]);
        return false;
    } 
    next();
    ResultantNode = std::make_shared<GroupedExpr>(ResultantNode);
            dump2(__func__);

    return true;
}

bool Parser::ParseBinaryExpr(int prev_prece) {
    dump(__func__);
    tokt tok = toks[CurrentIndex];
    Token_type opr;
    AstPtr left;
    if(!ResultantNode){
        if(!ParseExpr2(prev_prece)){
            return false;
        }
        left = ResultantNode;
    }
    left = ResultantNode;

    //2+3*3/4+2*3
    while(BinaryOP(CurrentToken)) {
        opr = CurrentToken;
        int CurPrecedance = preced(opr);
        if(prev_prece > CurPrecedance) {
            ResultantNode = left;
            return true;
        }
        next();
        ResultantNode = nullptr;
        if(!ParseExpr2(CurPrecedance += 1)){
            return false;
        }
        left = std::make_shared<Expression>(tok, left, opr, ResultantNode, NodeBinaryExpr);
    }
    ResultantNode = left;
            dump2(__func__);

    return true;
}


bool Parser::ParseSpecificType() {
    tokt tok = toks[CurrentIndex];
    ParseIdentifier();
    AstPtr Name = ResultantNode;
    if(checkn(LT)){
        std::vector<tokt>Temp;
        while(check(IDEN)){
            Temp.push_back(toks[CurrentIndex]);
            if(!checkn(COMMA))
                break;
        }
        if(!checkn(GT)){
            err::out("expected '>' found -",toks[CurrentIndex]);
            return false;
        }
        ResultantNode = std::make_shared<PostfixExpr>();
    }
    return true;
}

bool Parser::ParseEnumStmt() {
    std::vector<tokt>udata;
    std::vector<AstPtr>val;
    if(!checkn(ENUM)){
        err::out("expected 'enum' keyword found -",toks[CurrentIndex]);
        return false;
    }
    if(!checkn(LBRACE)){
        err::out("expected '{' found ->",toks[CurrentIndex]);
        return false;
    }

    while(check(IDEN)){
        udata.push_back(toks[CurrentIndex]);
        if(checkn(ASSN)){
            if(!ParseExpr()){
                return false;
            }
            val.push_back(ResultantNode);
        }
        if(!checkn(COMMA)){
            err::out("expected ',' found ->",toks[CurrentIndex]);
            return false;
        }
    }

    if(!checkn(RBRACE)){
        err::out("expected '}' found ->",toks[CurrentIndex]);
        return false;
    }
    ResultantNode = std::make_shared<EnumExpr>(udata,val);
    return true;
}

bool Parser::ParseStructStmt() {
    dump(__func__);
    bool isDecl = false;
    tokt tok = toks[CurrentIndex];
    std::vector<tokt> ElementName;
    std::vector<AstPtr> ElementTy;
    std::vector<tokt> Temp;
    if(!checkn(STRUCT)) {
        err::out("expected keyword 'struct' found ->",toks[CurrentIndex]);
        return false;
    }

  
    if(checkn(LT)){
        while(check(IDEN)){
            Temp.push_back(toks[CurrentIndex]);
            if(!checkn(COMMA))
                break;
        }
        if(!checkn(GT)){
            err::out("expected '>' found ->",toks[CurrentIndex]);
            return false;
        }
    }

    if(!check(SCOL)) {
        isDecl = true;
        goto done;
    }


    if(!check(LBRACE)){
        err::out("expected field expression '{' found -> ",toks[CurrentIndex]);
        return false;
    }

    while(CurrentToken !=  FEOF && check(IDEN)){
        ElementName.push_back(toks[CurrentIndex]);
        if(checkn(COL)){
            err::out("expected struct field expression ':' found - ",toks[CurrentIndex]);
            return false;
        }
        if(!ParseType()){
            err::out("expected struct field expression ': ?type' found - ",toks[CurrentIndex]);
            return false;
        }
        ElementTy.push_back(ResultantNode);
        if(!checkn(COMMA)){
            break;
        }
    }

    if(!checkn(RBRACE)){
        err::out("expected '}' found -> ",toks[CurrentIndex]);
        return false;
    }


done:
    ResultantNode = std::make_shared<StructStmt>(tok, Temp, ElementName, ElementTy, isDecl);
    dump2(__func__);

    return true;
}




bool Parser::ParseFuncDef() {
     dump(__func__);
    // tokt tok = toks[CurrentIndex];
    std::vector<tokt>pN;
    std::vector<AstPtr>pTy;
    AstPtr RetVal = nullptr;
    AstPtr StructVal = nullptr;
    AstPtr Block = nullptr;
    bool isDecl = false;

    if(!checkn(FN)) {
        err::out("expected keyword 'fn' found ->\'",toks[CurrentIndex]);
        return false;
    }
    if(!check(IDEN)){
        err::out("expected 'Identifier' with function defination found ->\'",toks[CurrentIndex]);
        return false;
    }   
    tokt Name = toks[CurrentIndex];

    if(checkn(LPAREN)) {
        err::out("expected function expression (parameters : type) found -> ",toks[CurrentIndex]);
        return false;
    }
    while(check(IDEN)) {
        pN.push_back(toks[CurrentIndex]);
        if(!checkn(IS)&&!checkn(SCOL)){
            err::out("expected { 'is',':' } found -> \'",toks[CurrentIndex]);
            return false;
        }
        if(!ParseType()) {
            return false;
        }
        pTy.push_back(ResultantNode);
        
        if(!checkn(COMMA)) {
            break;
        }
    }

    if(!checkn(RPAREN)){
        err::out("expected ')' found -> ",toks[CurrentIndex]);
        return false;
    }

    if(checkn(ARROW)){
        if(!ParseType()){
            err::out("expected 'type' found -> ",toks[CurrentIndex]);
            return false;
        }
        RetVal = ResultantNode;
    }

    if(check(SCOL)){
        isDecl = true;
        goto done;
    }

    if(checkn(FOR)){
        if(!check(IDEN)){
            err::out("fn-for statements can only have struct type  - ",toks[CurrentIndex]);
            return false;
        }
        if(!ParseSpecificType()) {
            return false;
        }
        StructVal = ResultantNode;
    }

    if(check(LBRACE)) {
        bool isBrace = true;
        if(!ParseBlock(isBrace)){
            return false;
        }
        Block = ResultantNode;
    }else{
        err::out("expected function body expression '{}' found - ",toks[CurrentIndex]);
        return false;
    }
done:
    ResultantNode = std::make_shared<FunctionDef>(Name, pN, pTy, StructVal, RetVal, Block, isDecl);
    dump2(__func__);
    return true;
}

bool Parser::ParseIfStmt() {
    dump(__func__);
    tokt tok = toks[CurrentIndex];
    if(!checkn(IF)){
        err::out("expected 'if' found - ",toks[CurrentIndex]);   
        return false;
    }
    AstPtr condition = nullptr;
    AstPtr if_ = nullptr;
    AstPtr else_ = nullptr;

    if(check(LBRACE)) {
        err::out("expected 'condition' found - ",toks[CurrentIndex]);
        return false;
    }

    if(!ParseExpr()){
        return false;
    }

    condition = ResultantNode;

    // if(checkn(COL)){
    //     //todo
    // }

    bool isBrace = true;
    if(!ParseBlock(isBrace)){
        return false;
    }
    if_ = ResultantNode;

    if(checkn(ELSE)) {
        switch(CurrentToken){
            case IF:
                if(!ParseIfStmt()){
                    return false;
                }
                else_ = ResultantNode;
                break;
            case LBRACE:
                if(!ParseBlock(isBrace)){
                    return false;
                }
                else_ = ResultantNode;
                break;
            default:
                err::out("invalid else expression ",toks[CurrentIndex]);
                return false;

        }
    }
    ResultantNode = std::make_shared<IfStatement>(tok,condition, if_, else_);
            dump2(__func__);

    return true;
}

bool Parser::ParseForStmt(){
    tokt tok = toks[CurrentIndex];
    AstPtr var = nullptr;
    AstPtr cond = nullptr;
    AstPtr incr = nullptr;
    AstPtr body = nullptr;
    if(!checkn(FOR)) {
        err::out("expected keyword 'for' found - ",toks[CurrentIndex]);
        return false;
    }

    if(checkn(SCOL)){
        goto cond;
    }
    if(!check(LET)){
        err::out("expected for expression 'identifier' found - ",toks[CurrentIndex]);
        return false;
    }
    if(!ParseVarStmt()){
        return false;
    }
    var = ResultantNode;
    if(!checkn(SCOL)){
        err::out("expected for expression ';' found - ",toks[CurrentIndex]);
        return false;
    }
cond:
    if(checkn(SCOL)){
        goto incr;
    }
    if(!ParseExpr()){
        return false;
    }
    cond = ResultantNode;
    if(!checkn(SCOL)){
        err::out("expected for expression ';' found - ",toks[CurrentIndex]);
        return false;
    }

incr:
    if(check(LBRACE)){
        goto incr;
    }
    if(!ParseExpr()){
        return false;
    }
    incr = ResultantNode;
body:
    bool isBrace = true;
    if(!ParseBlock(isBrace)){
        return false;
    }
    body = ResultantNode;
    ResultantNode = std::make_shared<ForLoop>(tok, var, cond, incr, body);
    return true;
}


// ref from rust lang and scribe
bool Parser::ParseForInStmt() {
    dump(__func__);
    tokt tok = toks[CurrentIndex];
    AstPtr in = nullptr;
    AstPtr var = nullptr;
    AstPtr cond = nullptr;
    AstPtr incr = nullptr;
    AstPtr body = nullptr;
    if(!checkn(FOR)) {
        err::out("expected keyword 'for' found - ",toks[CurrentIndex]);
        return false;
    }

    if(!check(IDEN)){
        err::out("expected 'identifier' found - ",toks[CurrentIndex]);
        return false;
    }
    ParseIdentifier();
    AstPtr tempvar = ResultantNode;



    if(!checkn(IN)){
        err::out("expected keyword 'in' found -> ",toks[CurrentIndex]);
        return false;
    }
    // 0..1 , iter

    if(check(LBRACE)){
        err::out("expected 'expression' found -> ",toks[CurrentIndex]);
        return false;
    }

    if(!check(INT)){
        goto iter;
    }
    if(!ParseLiteral()){
        return false;
    }
    AstPtr from = ResultantNode;
    if(!checkn(DOTDOT)){
        err::out("expected for-in expression '..' found - ",toks[CurrentIndex]);
        return false;
    }
    if(!check(INT)||!ParseLiteral()){
        err::out("expected for-in expression 'integer' after '..' found - ",toks[CurrentIndex]);
        return false;
    }
    AstPtr to = ResultantNode;

    AstPtr intlit = nullptr;
    if(checkn(DOTDOT)){
       if(!check(INT)||!ParseLiteral()){
            err::out("expected for-in expression integer after '..' found - ",toks[CurrentIndex]);
            return false;
        }
        intlit = ResultantNode;
    }else{
        intlit = std::make_shared<NumericLiteral>(tokt(-1,-1,"1",INT));
    }
    bool isBrace = true;
    if(!ParseBlock(isBrace)){
        return false;
    }
    body = ResultantNode;
    var = std::make_shared<VarStmt>(nullptr,tempvar, nullptr, from);
    cond = std::make_shared<Expression>(tempvar, LT, to,NodeBinaryExpr);
    incr = std::make_shared<Expression>(tempvar, ASSN_PLUS, intlit,NodeBinaryExpr);
    ResultantNode = std::make_shared<ForLoop>(tok, var, cond, incr, body);
    return true; 
iter:    
    if(!ParseExpr()) {
        err::out("for-in use only { 1..5..1 , .iter()} -> ",toks[CurrentIndex]);
        return false;
    }
    if(ResultantNode->nodeCategory() != NodeMemExpr){
        err::out("expected 'iterator' expression found -> ",toks[CurrentIndex]);
        return false;
    }
    in = ResultantNode;

    bool isBrace = true;
     if(!ParseBlock(isBrace)){
        return false;
    }
    body = ResultantNode;
    
    ///<iterVar> = <vector.iter()>
    tokt iterdecltok = tokt(-1, -1, "iterVar", IDEN);
    tokt iterTempVartok = tokt(-1, -1, "_"+tempvar->toString(), IDEN);
    tokt iterbegintok = tokt(-1, -1, "begin", IDEN);
    tokt iterendtok = tokt(-1, -1, "end", IDEN);
    tokt iternexttok = tokt(-1, -1, "next", IDEN);
    tokt iterattok = tokt(-1, -1, "at", IDEN);

    AstPtr iterbeginIden = std::make_shared<Identifier>(iterbegintok);
    AstPtr iterendIden = std::make_shared<Identifier>(iterendtok);
    AstPtr iternextIden = std::make_shared<Identifier>(iternexttok);
    AstPtr iteratIden = std::make_shared<Identifier>(iterattok);

    AstPtr iterTempVarName = std::make_shared<Identifier>(iterTempVartok);
    AstPtr iterDeclName = std::make_shared<Identifier>(iterdecltok);

    AstPtr iterdecl = std::make_shared<VarStmt>(nullptr, iterDeclName, nullptr, in, false);

    AstPtr begincall = std::make_shared<FunctionCall>(iterbegintok,iterbeginIden,std::vector<AstPtr>());
    AstPtr iterBegin = std::make_shared<Expression>(iterdecltok,iterDeclName, DOT, begincall, NodeMemExpr);
    var = std::make_shared<VarStmt>(iterTempVartok, iterTempVarName, nullptr, iterBegin);

    ///<_temp> != <iterVar.end()>
    AstPtr endcall = std::make_shared<FunctionCall>(iterendtok, iterendIden, std::vector<AstPtr>());
    AstPtr iterend = std::make_shared<Expression>(iterdecltok, iterDeclName, DOT, endcall, NodeMemExpr);
    cond = std::make_shared<Expression>(iterTempVarName, NEQL, iterend, NodeBinaryExpr);

    ///<iterVar.next(_temp)>
    AstPtr nextcall = std::make_shared<FunctionCall>(iternexttok, iternextIden, std::vector<AstPtr>(1,iterDeclName));
    AstPtr iternext = std::make_shared<Expression>(iterdecltok, iterDeclName, DOT, nextcall, NodeMemExpr);
    incr = std::make_shared<Expression>(iterTempVarName, ASSN, iternext, NodeAssnExpr);

    ///<var = _iterVar.at()>
    AstPtr atcall = std::make_shared<FunctionCall>(iterattok, iteratIden, std::vector<AstPtr>(1,iterDeclName));
    AstPtr iterat = std::make_shared<Expression>(iterdecltok, iterDeclName, DOT, atcall);
    AstPtr insideloop = std::make_shared<Expression>(tempvar, ASSN, iterat, NodeAssnExpr);

     
    auto Block = std::dynamic_pointer_cast<BlockStatement>(body);
    Block->getStmts().insert(Block->getStmts().begin(), insideloop);
    body = Block;
    ResultantNode = std::make_shared<ForLoop>(tok, var, cond, incr, body);
            dump2(__func__);
    ResultantNode = std::make_shared<BlockStatement>(std::vector<AstPtr>{iterdecl, ResultantNode});
    return true;
}


bool Parser::ParseExtern() {
    tokt tok = toks[CurrentIndex];
    next();
    std::vector<AstPtr>Header;
    if(!checkn(LBRACK)){
        err::out("expected '[' found - ",toks[CurrentIndex]);
        return false;
    }
    if(!check(IDEN)) {
        err::out("expected 'identifier' found - ",toks[CurrentIndex]);
        return false;
    }
    if(!ParseIdentifier()){
        return false;
    }
    Header.push_back(ResultantNode);
    if(!check(STR)) {
        err::out("expected 'string' found - ",toks[CurrentIndex]);
        return false;
    }
    if(!ParseLiteral()){
        return false;
    }
    Header.push_back(ResultantNode);
    if(!checkn(RBRACK)){
        err::out("expected ']' found - ",toks[CurrentIndex]);
        return false;
    }
    if(check(SCOL)){
        goto end;
    }
    if(check(FN)){
        if(!ParseFuncDef()) {
            return false;
        }
    }else if(check(STRUCT)){
        if(!ParseStructStmt()) {
            return false;
        }
    }else{
        err::out("expected fn and struct statement block with extern found - ",toks[CurrentIndex]);
        return false;
    }
    Header.push_back(ResultantNode);
end:
    ResultantNode = std::make_shared<Extern>(tok, Header);
    return true;
}


bool Parser::ParseVarStmt() {
    tokt tok = toks[CurrentIndex];
    AstPtr Var;
    AstPtr Ty;
    AstPtr Val;
    bool isty = false;
    bool isval = false;
    bool isuse = false;
    bool isIs = false;
    bool isIn = false;
    bool isConst = false;
    if(!checkn(LET)){
        err::out("expected 'let' found -> ",toks[CurrentIndex]);
        return false;
    }
    if(checkn(CONST)){
        isConst = true;
    }

    if(!check(IDEN)){
        err::out("expected 'identifier' found -> ",toks[CurrentIndex]);
        return false;
    }
    ParseIdentifier();
    Var = ResultantNode;

    if(!checkn(COL)){
        goto is;
    }
    
    isty = true;
    if(!ParseType()){
        return false;
    }
    Ty = ResultantNode;
// is:
//     if(!checkn(IS)){
//         goto val;
//     }
//     isIs = true;
//     // bool isExtern = false;
//     if(check(EXTERN)) {
//         if(!ParseExtern()){
//             return false;
//         }
//         // isExtern = true;
//     }else if(check(STRUCT)) {
//         if(!ParseStructStmt()){
//             return false;
//         }
//     }else if(check(ENUM)) {
//         if(!ParseEnumStmt()){
//             return false;
//         }
//     }else{
//         err::out("let-is statements can only have statement "
// 					"(eg. function, struct, enum, extern, import decalaration ) - ", toks[CurrentIndex]);
//         return false;
//     }
//     Val = ResultantNode;

val:  

    if(!checkn(ASSN)){
        goto done;
    }
    isval = true;
    if(!ParseExpr()){
            return false;
    }
    Val = ResultantNode;
done:
    if(!isval && !isty){
        err::out("invalid variable declaration - no type or value set", tok);
        return false;
    }
    if(isuse){
        if(isval){
            err::out("let-use statements can only have path "
					"(../../..) - no assignment allowed", tok);
    
            return false;
        }
        if(isty){
            err::out("let-use statements can only have path "
					"(../../..) - no type allowed", tok);
    
            return false;
        }
        if(isIs||isIn){
            err::out("let-use statements can only have path "
					"(../../..) - no 'in' and 'is' allowed", tok);
    
            return false;
        }
    } 
    
    // if(isConst) {
    //     if(isIs) {
    //         err::out("let-const statements can only have type and val "
	// 				" - no 'in' and 'is' allowed", tok);
    
    //         return false;
    //     }
    // }

    // if(isIs){
    //     if(isval){
    //         err::out("let-is statements can only have statement "
	// 				"(function, struct, enum definitions) - no assignment allowed", tok);
    
    //         return false;
    //     }
    //     if(isty){
    //         err::out("let-is statements can only have statement "
	// 				"(function, struct, enum definitions) - no type allowed", tok);
    
    //         return false;
    //     }
    // }


    ResultantNode = std::make_shared<VarStmt>(tok, Var, Ty, Val, isConst);
    return true;
}
} // namespace parser
