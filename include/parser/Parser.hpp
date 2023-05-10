#pragma once

#include<iostream>
#include"Ast.hpp"

using namespace ast;
namespace parser{
    using namespace lex;

    // class ParserHelper{
    //     tok_t toks;
    //     uint CurIndex;
    //     Token_type CurToken;
    //     tokt next_t();
    //     void next();

    //     void dump(std::string msg);
    //     void dump2(std::string msg);
    //     inline bool check(Token_type tok);
    //     inline bool checkn(Token_type tok);
    //     inline bool checkh(Token_type tok);
    //     inline bool checkNH(Token_type tok, int i);
    //     int preced(Token_type op);
    //     inline bool ArithmeticOP(Token_type tok);
    //     inline bool ConditionalOP(Token_type tok);
    //     inline bool LogicalOP(Token_type tok);
    //     inline bool UnaryOP(Token_type tok);
    //     inline bool AssignOP(Token_type tok);
    //     inline bool isLiteral(Token_type tok);
    //     bool isTerminal(Token_type tok);
    //     inline bool PreDefType(Token_type tok);
    // };

    class Parser {   
    enum KindExpression {
        KExpr_Identifier,
        KExpr_StructInit,
        KExpr_ArrayInit,
        KExpr_ArrayAccess,
        KExpr_DotDot,
        KExpr_DotArrow,
        KExpr_Call,
        KExpr_GroupedDotArrow,
        KExpr_GroupedBinary,
        KExpr_GroupedAcc,
        KExpr_Grouped,
        KExpr_Unary,
        KExpr_Self
    };
        tok_t toks;
        uint CurrentIndex;
        // std::string path;
        // std::string file_name;
        Token_type CurrentToken;
        tokt next_t();
        void next();
        AstPtr ResultantNode;
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

        bool ParseBlock(bool isBrace);
        bool ParseReturnStmt();
        bool ParseType();
        bool ParseArrayType();
        bool ParseIdentifier();
        bool ParseTypeStatm();
        bool ParseCall();
        bool ParseListExpr();
        bool ParseCastExpr();
        bool ParsePrefixExpr(KindExpression Expr);
        bool ParseExpr();
        bool ParseExpr2(int &Precedance);
        bool ParseExpression(int &Precedance, KindExpression &Expr);
        bool ParseParenExpr();
        bool ParsePrePostExpr(KindExpression &Expr);
        bool ParseIdentPostExpr(KindExpression &Expr);
        bool ParseDotArrowExpr(KindExpression &Expr);
        bool ParseStructExpr();
        bool ParseArrayIndexExpr();
        bool ParseParenExpr();
        bool ParseBinaryExpr(int prev_prece);
        bool ParseSpecificType();
        bool ParseEnumStmt();
        bool ParseStructStmt();
        bool ParseFuncDef();
        bool ParseIfStmt();
        bool ParseForStmt();
        bool ParseForInStmt();
        bool ParseExtern();
        bool ParseVarStmt();
        bool ParseLiteral();
        bool ParseWhileStmt();
        
        public:
        Parser(tok_t &_toks, int index)
        : toks(_toks), CurrentIndex(index), CurrentToken(toks[CurrentIndex].tok_type)
        {}
        ~Parser() {}
        AstPtr parse();
    };

}

