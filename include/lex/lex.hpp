#pragma once


//#include <cstdio>
#include <iostream>
#include <vector>

namespace lex
{
    enum Token_type
    {
        INT,
        STR,
        CHAR,
        FLOAT,
        NIL,
        IDEN,

        EXTERN,
        WHILE,
        AS,
        IN,
        MOD,
        TYPE,
        CONST,
        STATIC,
        FN,
        FOR,
        LET,
        RETURN,
        CONTINUE,
        BREAK,
        TRUE,
        FALSE,
        SELF,
        IF,
        ELSE,
        STRUCT,
        METHOD,
        ENUM,
        BOOL,
        I8,
        I16,
        I32,
        I64,
        I128,
        UI8,
        UI16,
        UI32,
        UI64,
        UI128,
        F32,
        F64,

        ASN,
        PLUS,
        MINUS,
        STAR,
        DIV,
        MODULO,
        ASN_PLUS,
        ASN_STAR,
        ASN_MINUS,
        ASN_DIV,
        ASN_MOD,


        CND_AND,
        CND_OR,
        CND_NOT,

        EQL,
        NEQL,
        LT,
        GT,
        LEQL,
        GEQL,

        AND,
        OR,
        XOR,
        NOT,
        ASN_AND,
        ASN_OR,
        ASN_XOR,
        ASN_NOT,

        LSHIFT,
        RSHIFT,
        ASN_LSHIFT,
        ASN_RSHIFT,

        DOTDOT,

        ARROW,
        DOT,
        SCOL,
        COL,
        COLCOL,
        COMMA,
        SPC,
        TAB,
        NEWL,

        LPAREN,
        RPAREN,
        LBRACE,
        RBRACE,
        LBRACK,
        RBRACK,

        FEOF,
        INVALID,
        _LAST
    };
    extern const char *TokenMap[_LAST];

    struct TokData
    {
        int line;
        int pos;
        const std::string data;
        TokData(const int &_line, const int &_pos, const std::string &_data)
		:line(_line), pos(_pos),  data(_data)
	    {}
    };

    class Tok{
    private:
        Token_type tok;
    public: 
        Tok(Token_type tok);
        ~Tok();

        Token_type getTokType() const;
        
        inline bool isLiteral() {
            return tok == INT||tok == STR||tok == CHAR|| tok == FLOAT||tok == TRUE||tok == FALSE||tok == NIL;
        }
        
        inline bool ArithmeticOP() const {
            return tok == STAR || tok == DIV || tok == MOD ||
                                    tok == MINUS || tok == PLUS;
        } 

        inline bool ConditionalOP() const {
            return tok == LT || tok == LEQL || tok == GT || 
                    tok == GEQL || tok == EQL || tok == NEQL ||
                                        tok == CND_AND ||  tok == CND_OR;
        }  

        inline bool LogicalOP() const{
            return tok == LSHIFT || tok == RSHIFT || 
                        tok == AND || tok == XOR || 
                            tok == AND || tok == OR;
        }  

        inline bool IsBinaryOP() const {
            return ArithmeticOP()||ConditionalOP()||LogicalOP();
        } 

        inline bool IsUnaryOP() const {
            return tok == PLUS|| tok == MINUS||
            tok == STAR|| tok == AND ||tok == NOT || tok == CND_NOT;
        }

        inline bool IsAssnCombinedOP() const {
            return tok == ASN_DIV||
            tok == ASN_MINUS|| tok == ASN_MOD||tok == ASN_PLUS|| tok == ASN_STAR|| 
            tok == ASN_AND|| tok == ASN_XOR||tok == ASN_OR|| tok == ASN_NOT;
        }
        inline bool IsAssignOP() const {
            return tok == ASN;
        }
        
        inline bool IsPreDefType() const {
            return tok == I8 || tok == I16 ||
            tok == I32 || tok == I64 || tok == UI8 || tok == UI16 ||
            tok == UI32 || tok == UI64 || tok == F32 || tok == F64 ||
            tok == BOOL;
        }

        inline bool IsTerminal() const {
            return tok == RPAREN || tok == COMMA ||
            tok == RBRACE || tok == RBRACK || tok == COL || tok == SCOL;
        }

        inline bool isLiteral() const {
            return tok == INT||tok == STR||tok == CHAR|| tok == FLOAT||tok == TRUE||tok == FALSE||tok == NIL;
        }

        bool operator==(Tok _Tok) {
            if(tok == _Tok.getTokType()){
                return true;
            }
            return false;
        }
        bool operator!=(Tok _Tok) {
            if(tok != _Tok.getTokType()){
                return true;
            }
            return false;
        }

        bool operator==(Token_type _Tok) {
            if(tok == _Tok){
                return true;
            }
            return false;
        }
        bool operator!=(Token_type _Tok) {
            if(tok != _Tok){
                return true;
            }
            return false;
        }
    };
    
    class Lexeme {
    private:
        TokData data;
        Tok tok;
    public:
        explicit Lexeme(TokData _data, Tok _t)
        :data(_data),tok(_t){}
        explicit Lexeme(std::string s, Token_type _t);
        ~Lexeme();

        static Lexeme* create(int l, int p, std::string s, Token_type _t);
        Tok getTok() const { return tok; }
        Token_type getTokTy() const { return tok.getTokType(); }
        int getPos() const;
        std::string getStr() const;
        
    };

    class Lexer {
    private:
        Token_type get_keyword(std::string &src);
        std::string  get_string(const std::string &src, int &i);
        bool get_const_string(const std::string &src, int &i, char &quote, std::string &str);
        bool get_num(const std::string &src, int &i, int base, std::string &num, Token_type &type);
        bool get_operator(const std::string &src, Token_type &op_type, int &i);
    public:
        Lexer();
        ~Lexer();
        bool tokenizer(const std::string &src, std::vector<Lexeme> &toks, int begin, int end);
    };
}


