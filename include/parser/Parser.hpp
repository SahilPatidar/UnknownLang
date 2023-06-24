#pragma once

#include<iostream>
#include"Ast.hpp"
#include"../Module.hpp"

using namespace ast;
namespace parser{
    using namespace lex;
 
    class ParserHelper{
    private:
        std::vector<Lexeme> toks;
        uint idx;
    public:
        ParserHelper(std::vector<Lexeme> &_toks)
        :toks(_toks) { idx = 0;}

        inline Lexeme next_l();
        inline Tok next_t();
        inline Token_type next_tt();

        inline Lexeme peek_l();
        inline Tok peek_t();
        inline Token_type peek_tt();

        inline uint getIndex();
        inline void next();
        inline Lexeme at(int i);
        inline Tok at_t(int i);
        inline Token_type at_tt(int i);

        void dump(std::string msg);
        void dump2(std::string msg);

        inline bool check(Token_type tok);
        inline bool checkn(Token_type tok);
        inline bool checkh(Token_type tok);
        inline bool checkNH(Token_type tok, int i);
        int preced(Token_type op);
    };

    class Parser{
    private:
        module::Module modctx;
        module::Mod Mod;
    public:
        Parser(module::Module &_modctx);
        Parser(module::Mod &_modctx);
        ~Parser();

        bool parse();
    private:
        bool ParseBlock(ParserHelper &P, Ast *&ResultantNode, bool isBrace);
        bool ParseMethod(ParserHelper &P, Ast *&ResultantNode);
        bool ParseUseStmt(ParserHelper &P, Ast *&ResultantNode);
        bool ParseWhileStmt(ParserHelper &P, Ast *&ResultantNode);
        bool ParseReturnStmt(ParserHelper &P, Ast *&ResultantNode);
        bool ParseType(ParserHelper &P, Ast *&ResultantNode);
        bool ParseArrayType(ParserHelper &P, Ast *&ResultantNode);
        bool ParseTypeStatm(ParserHelper &P, Ast *&ResultantNode);
        bool ParseCall(ParserHelper &P, Ast *&ResultantNode);
        bool ParseListExpr(ParserHelper &P, Ast *&ResultantNode);
        bool ParseCastExpr(ParserHelper &P, Ast *&ResultantNode);
        bool ParsePrefixExpr(ParserHelper &P, Ast *&ResultantNode);
        bool ParseExpr(ParserHelper &P, Ast *&ResultantNode);
        bool ParseExpr1(ParserHelper &P, Ast *&ResultantNode, int Precedance);
        bool ParseExpr2(ParserHelper &P, Ast *&ResultantNode);
        bool ParseExpr3(ParserHelper &P, Ast *&ResultantNode);
        bool ParseExpr4(ParserHelper &P, Ast *&ResultantNode);
        bool ParseExpr5(ParserHelper &P, Ast *&ResultantNode);
        bool ParseExpr6(ParserHelper &P, Ast *&ResultantNode);

        bool ParseGenericParam(ParserHelper &P, GenericParam *&ResultantNode);
        bool ParseTypePath(ParserHelper &P, Ast *&ResultantNode);

        bool ParsePath(ParserHelper &P, Ast *&ResultantNode);
        bool ParseExpression(ParserHelper &P, Ast *&ResultantNode);
        bool ParseLiteral(ParserHelper &P, Ast *&ResultantNode);
        bool ParseParenExpr(ParserHelper &P, Ast *&ResultantNode);
        bool ParseIdentifier(ParserHelper &P, Ast *&ResultantNode);
        bool ParseDotArrowExpr(ParserHelper &P, Ast *&ResultantNode);
        bool ParseStructExpr(ParserHelper &P, Ast *&ResultantNode);
        bool ParseArrayIndexExpr(ParserHelper &P, Ast *&ResultantNode);
        bool ParseBinaryExpr(ParserHelper &P, Ast *&ResultantNode, int prev_prece);
        bool ParseSpecificType(ParserHelper &P, Ast *&ResultantNode);
        bool ParseEnumStmt(ParserHelper &P, Ast *&ResultantNode);
        bool ParseStructStmt(ParserHelper &P, Ast *&ResultantNode);
        bool ParseFuncDef(ParserHelper &P, Ast *&ResultantNode);
        bool ParseIfStmt(ParserHelper &P, Ast *&ResultantNode);
        bool ParseForStmt(ParserHelper &P, Ast *&ResultantNode);
        bool ParseForInStmt(ParserHelper &P, Ast *&ResultantNode);
        bool ParseExtern(ParserHelper &P, Ast *&ResultantNode);
        bool ParseMod(ParserHelper &P, Ast *&ResultantNode);
        bool ParseVarStmt(ParserHelper &P, Ast *&ResultantNode);
    };

}
