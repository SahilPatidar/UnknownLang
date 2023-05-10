
#include"parser/Parser.hpp"

namespace parser {
    
    void Parser::next() {
        if(CurrentIndex + 1 < toks.size()) {
            ++CurrentIndex;
            CurrentToken = toks[CurrentIndex].tok_type;
        }
    }

    inline bool Parser::checkNH(Token_type tok, int i){
        return toks[CurrentIndex+i].tok_type == tok;
    }

    void Parser::dump(std::string msg){
        std::cout<<"step :: "<<"entering in -> "<<msg<<" "<<toks[CurrentIndex].data<<std::endl;
    }
    void Parser::dump2(std::string msg){
        std::cout<<"step :: "<<"returning from -> "<<msg<<" "<<toks[CurrentIndex].data<<std::endl;
    }

    inline bool Parser::ArithmeticOP(Token_type tok) {
        return tok == STAR || tok == DIV || tok == MOD ||
                                 tok == MINUS || tok == PLUS;
    } 

    inline bool Parser::ConditionalOP(Token_type tok) {
        return tok == LT || tok == LEQL || tok == GT || 
                tok == GEQL || tok == EQL || tok == NEQL ||
                                    tok == AND ||  tok == OR;
    }  

    inline bool Parser::LogicalOP(Token_type tok) {
        return tok == LSHIFT || tok == RSHIFT || 
                    tok == AND || tok == XOR_OP || 
                        tok == AND_OP || tok == OR_OP;
    }  

    inline bool Parser::BinaryOP(Token_type tok) {
        return ArithmeticOP(tok)||ConditionalOP(tok)||LogicalOP(tok);
    } 

    inline bool Parser::check(Token_type tok) {
        return CurrentToken == tok;
    }

    inline bool Parser::checkn(Token_type tok) {
        if(CurrentToken == tok){
            next();
            return true;
        }
        return false;
    }

    inline bool Parser::checkh(Token_type tok) {
        return next_t().tok_type == tok;
    }

    tokt Parser::next_t() {
        return (CurrentIndex+1 < toks.size())?toks[CurrentIndex+1]:toks[CurrentIndex];
    }

    bool Parser::UnaryOP(Token_type tok) {
        return tok == PLUS|| tok == MINUS||
         tok == STAR|| tok == AND_OP||tok == NOT_OP|| tok == NOT;
    }

    bool Parser::AssignOP(Token_type tok){
        return tok == ASSN|| tok == ASSN_DIV||
         tok == ASSN_MINUS|| tok == ASSN_MOD||tok == ASSN_PLUS|| tok == ASSN_STAR|| 
         tok == AND_ASSN|| tok == XOR_ASSN||tok == OR_ASSN|| tok == NOT_ASSN;
    }
    
    bool Parser::PreDefType(Token_type tok){
        return tok == I8 || tok == I16 ||
         tok == I32 || tok == I64 || tok == UI8 || tok == UI16 ||
         tok == UI32 || tok == UI64 || tok == F32 || tok == F64 ||
         tok == BOOL;
    }

    bool Parser::isTerminal(Token_type tok){
        return tok == RPAREN || tok == COMMA ||
         tok == RBRACE || tok == RBRACK || tok == COL || tok == SCOL;
    }

    int Parser::preced(Token_type op){
        switch (op)
        {
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
        default:
            break;
        }
        return 0;
    }

    bool Parser::isLiteral(Token_type tok) {
        return tok == INT||tok == STR||tok == CHAR|| tok == FLOAT||tok == TRUE||tok == FALSE||tok == NIL;
    }


}