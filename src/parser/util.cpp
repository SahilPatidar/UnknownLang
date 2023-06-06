
#include"parser/Parser.hpp"

namespace parser {
    
    void ParserHelper::next() {
        if(CurrentIndex + 1 < toks.size()) {
            ++CurrentIndex;
        }
    }

    inline bool ParserHelper::checkNH(Token_type tok, int i){
        return at_tt(i) == tok;
    }

    void ParserHelper::dump(std::string msg){
        std::cout<<"step :: "<<"entering in -> "<<msg<<" "<<peek_l().getStr()<<std::endl;
    }
    void ParserHelper::dump2(std::string msg){
        std::cout<<"step :: "<<"returning from -> "<<msg<<" "<<peek_l().getStr()<<std::endl;
    }


    inline bool ParserHelper::check(Token_type tok) {
        return peek_tt() == tok;
    }

    inline bool ParserHelper::checkn(Token_type tok) {
        if(peek_tt() == tok){
            next();
            return true;
        }
        return false;
    }

    inline bool ParserHelper::checkh(Token_type tok) {
        return next_tt() == tok;
    }

    Lexeme ParserHelper::next_l() {
        return (CurrentIndex+1 < toks.size())?toks[CurrentIndex+1]:toks[CurrentIndex];
    }

    Tok ParserHelper::next_t() {
        return next_l().getTok();
    }

    Token_type ParserHelper::next_tt() {
        return next_t().getTokType();
    }

    Lexeme ParserHelper::peek_l() {
        return toks[CurrentIndex];
    }

    Tok ParserHelper::peek_t() {
        return peek_l().getTok();
    }

    Token_type ParserHelper::peek_tt() {
        return peek_t().getTokType();
    }

    Lexeme ParserHelper::at(int i) {
        return i < toks.size()?toks[i]:toks[toks.size()-1];
    }

    Tok ParserHelper::at_t(int i) {
        return at(i).getTok();
    }

    Token_type ParserHelper::at_tt(int i) {
        return at_t(i).getTokType();
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
        case XOR:
            return 4;
        case OR:
            return 3;    
        case CND_AND:
            return 2;  
        case CND_OR:
            return 1;
        case ASN: case ASN_DIV: case ASN_MINUS: case ASN_MOD: 
        case ASN_PLUS: case ASN_STAR: case ASN_AND: case ASN_XOR: 
        case ASN_OR: case ASN_NOT:
        default:
            break;
        }
        return 0;
    }




}