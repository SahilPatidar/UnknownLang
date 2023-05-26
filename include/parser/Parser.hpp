#pragma once

#include<iostream>
#include"Ast.hpp"

using namespace ast;
namespace parser{
    using namespace lex;


     enum KindExpression {
        KExpr_Identifier,
        KExpr_StructInit,
        KExpr_ArrayInit,
        KExpr_ArrayAccess,
        KExpr_DotDot,
        KExpr_DotArrow,
        KExpr_Call,
        KExpr_Grouped,
        KExpr_Unary,
        KExpr_Binary,
        KExpr_Self,
        KExpr_Literal,
    };

    class ParserHelper{
    public:
        tok_t toks;
        uint CurrentIndex;
        // std::string path;
        // std::string file_name;
        Token_type CurrentToken;
        tokt next_t();
        tokt getTok();
        inline uint getIndex();
        inline uint getCurToken();
        inline void next();

        void dump(std::string msg);
        void dump2(std::string msg);

        inline bool check(Token_type tok);
        inline bool checkn(Token_type tok);
        inline bool checkh(Token_type tok);
        inline bool checkNH(Token_type tok, int i);
        int preced(Token_type op);
        inline bool ArithmeticOP(Token_type tok);
        inline bool ConditionalOP(Token_type tok);
        inline bool LogicalOP(Token_type tok);
        inline bool UnaryOP(Token_type tok);
        inline bool BinaryOP(Token_type tok);
        inline bool AssignOP(Token_type tok);
        inline bool isLiteral(Token_type tok);
        bool isTerminal(Token_type tok);
        inline bool PreDefType(Token_type tok);
    };

    class Parser{
    private:
        
        bool ParseBlock(ParserHelper &P, Ast *&ResultantNode, bool isBrace);
        bool ParseMethod(ParserHelper &P, Ast *&ResultantNode);
        bool ParseWhileStmt(ParserHelper &P, Ast *&ResultantNode);
        bool ParseReturnStmt(ParserHelper &P, Ast *&ResultantNode);
        bool ParseType(ParserHelper &P, Ast *&ResultantNode);
        bool ParseArrayType(ParserHelper &P, Ast *&ResultantNode);
        bool ParseTypeStatm(ParserHelper &P, Ast *&ResultantNode);
        bool ParseCall(ParserHelper &P, Ast *&ResultantNode);
        bool ParseListExpr(ParserHelper &P, Ast *&ResultantNode);
        bool ParseCastExpr(ParserHelper &P, Ast *&ResultantNode);
        bool ParsePrefixExpr(ParserHelper &P, Ast *&ResultantNode, KindExpression Expr);
        bool ParseExpr(ParserHelper &P, Ast *&ResultantNode);
        bool ParseExpr1(ParserHelper &P, Ast *&ResultantNode, int Precedance);
        bool ParseExpr2(ParserHelper &P, Ast *&ResultantNode, KindExpression &Expr);
        bool ParseExpr3(ParserHelper &P, Ast *&ResultantNode, KindExpression &Expr);
        bool ParseExpr4(ParserHelper &P, Ast *&ResultantNode, KindExpression &Expr);
        bool ParseExpr5(ParserHelper &P, Ast *&ResultantNode, KindExpression &Expr);
        bool ParseExpression(ParserHelper &P, Ast *&ResultantNode, KindExpression &Expr);
        bool ParseLiteral(ParserHelper &P, Ast *&ResultantNode);
        bool ParseParenExpr(ParserHelper &P, Ast *&ResultantNode, KindExpression &Expr);
        bool ParseIdentifier(ParserHelper &P, Ast *&ResultantNode);
        bool ParseDotArrowExpr(ParserHelper &P, Ast *&ResultantNode, KindExpression &Expr);
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
        bool ParseVarStmt(ParserHelper &P, Ast *&ResultantNode);
    };

}

