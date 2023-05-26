#ifndef ERROR_H
#define ERROR_H
#include"lex/lex.hpp"
#include"parser/Ast.hpp"
// #include"lex/lex.hpp"

namespace err{
    void out(std::string msg, lex::tokt tok){
        std::cerr<<"error:: "<<tok.line+1<<":"<<tok.pos<<" { "<<msg<<" \' "<<tok.data<<" \'"<<" } "<<std::endl;
        //exit(1);
    }

    void dumperr(std::string errmsg, lex::tokt tok, std::string code) {

    }
}

#endif
