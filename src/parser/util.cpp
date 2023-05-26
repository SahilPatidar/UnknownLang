
#include"parser/Parser.hpp"

namespace parser {
    
    void ParserHelper::next() {
        if(CurrentIndex + 1 < toks.size()) {
            ++CurrentIndex;
            CurrentToken = toks[CurrentIndex].tok_type;
        }
    }

    inline bool ParserHelper::checkNH(Token_type tok, int i){
        return toks[CurrentIndex+i].tok_type == tok;
    }

    void ParserHelper::dump(std::string msg){
        std::cout<<"step :: "<<"entering in -> "<<msg<<" "<<toks[CurrentIndex].data<<std::endl;
    }
    void ParserHelper::dump2(std::string msg){
        std::cout<<"step :: "<<"returning from -> "<<msg<<" "<<toks[CurrentIndex].data<<std::endl;
    }

    inline bool ParserHelper::ArithmeticOP(Token_type tok) {
        return tok == STAR || tok == DIV || tok == MOD ||
                                 tok == MINUS || tok == PLUS;
    } 

    inline bool ParserHelper::ConditionalOP(Token_type tok) {
        return tok == LT || tok == LEQL || tok == GT || 
                tok == GEQL || tok == EQL || tok == NEQL ||
                                    tok == AND ||  tok == OR;
    }  

    inline bool ParserHelper::LogicalOP(Token_type tok) {
        return tok == LSHIFT || tok == RSHIFT || 
                    tok == AND || tok == XOR_OP || 
                        tok == AND_OP || tok == OR_OP;
    }  

    inline bool ParserHelper::BinaryOP(Token_type tok) {
        return ArithmeticOP(tok)||ConditionalOP(tok)||LogicalOP(tok);
    } 

    inline bool ParserHelper::check(Token_type tok) {
        return CurrentToken == tok;
    }

    inline bool ParserHelper::checkn(Token_type tok) {
        if(CurrentToken == tok){
            next();
            return true;
        }
        return false;
    }

    inline bool ParserHelper::checkh(Token_type tok) {
        return next_t().tok_type == tok;
    }

    tokt ParserHelper::next_t() {
        return (CurrentIndex+1 < toks.size())?toks[CurrentIndex+1]:toks[CurrentIndex];
    }

    bool ParserHelper::UnaryOP(Token_type tok) {
        return tok == PLUS|| tok == MINUS||
         tok == STAR|| tok == AND_OP||tok == NOT_OP|| tok == NOT;
    }

    bool ParserHelper::AssignOP(Token_type tok){
        return tok == ASSN|| tok == ASSN_DIV||
         tok == ASSN_MINUS|| tok == ASSN_MOD||tok == ASSN_PLUS|| tok == ASSN_STAR|| 
         tok == AND_ASSN|| tok == XOR_ASSN||tok == OR_ASSN|| tok == NOT_ASSN;
    }
    
    bool ParserHelper::PreDefType(Token_type tok){
        return tok == I8 || tok == I16 ||
         tok == I32 || tok == I64 || tok == UI8 || tok == UI16 ||
         tok == UI32 || tok == UI64 || tok == F32 || tok == F64 ||
         tok == BOOL;
    }

    bool ParserHelper::isTerminal(Token_type tok){
        return tok == RPAREN || tok == COMMA ||
         tok == RBRACE || tok == RBRACK || tok == COL || tok == SCOL;
    }

    int ParserHelper::preced(Token_type op){
        switch (op)
        {
        case LPAREN: case LBRACK:
        case LBRACE: case DOT: case ARROW:
            return 10;
        default:
            break;
        }
        return 0;
    }
    int ParserHelper::preced(Token_type op){
        switch (op)
        {
        case LPAREN: case LBRACK:
        case LBRACE: case DOT: case ARROW:
            return ;
        case AS: 
            return ;

        case STAR: 
        case DIV: 
        case MOD:
            return 10;
        case PLUS:
        case MINUS:
            return 9;
        case LSHIFT:
        case RSHIFT:
            return 8;
        case LT:
        case LEQL:
        case GT:
        case GEQL:
            return 7;
        case EQL:
        case NEQL:
            return 6;
        case AND:
            return 5;
        case XOR_OP:
            return 4;
        case OR:
            return 3;    
        case AND_OP:
            return 2;  
        case OR_OP:
            return 1;
        case ASSN: case ASSN_DIV: case ASSN_MINUS: case ASSN_MOD: 
        case ASSN_PLUS: case ASSN_STAR: case AND_ASSN: case XOR_ASSN: 
        case OR_ASSN: case NOT_ASSN:
        default:
            break;
        }
        return 0;
    }

    bool ParserHelper::isLiteral(Token_type tok) {
        return tok == INT||tok == STR||tok == CHAR|| tok == FLOAT||tok == TRUE||tok == FALSE||tok == NIL;
    }


}