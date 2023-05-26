static bool ParseExpr2(ParserHelper &P, Ast *&ResultantNode, int &Precedance) {
    KindExpression Expr;
    if(!ParseExpression(P,ResultantNode,Precedance, Expr)){
        return false;
    }

    if(P.check(AS)) {
        if(Expr == KExpr_ArrayInit || Expr == KExpr_StructInit || Expr == KExpr_DotDot){
            err::out("Arithmetic operation not allowed with - ",P.toks[P.CurrentIndex]);
            return false;
        }
        if(ParseCastExpr(P,ResultantNode)) {
            return false;
        }
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