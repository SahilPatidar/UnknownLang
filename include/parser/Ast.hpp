#pragma once

#include<iostream>
#include<vector>
#include<memory>
#include"../lex/lex.hpp"
#include"../analyzer/AstVisitor.hpp"
#include"type.hpp"
#include"../analyzer/Value.hpp"
// #include"lex/lex.hpp"
// #include"analyzer/AstVisitor.hpp"
// #include"type.hpp"

using namespace lex;
namespace ast{

    enum NodeCategory{
        NodeBlockStm,
        NodeImport,
        NodeNumLit,
        NodeBoolLit,
        NodeFloatLit,
        NodeStrLit,
        NodeNullLit,
        NodeIdent,
        NodeEnum,
        NodeBinaryExpr,
        NodePrefix,
        NodeMemExpr,
        NodeAssnExpr,
        NodeListExpr,
        NodeIndexExpr,
        NodeDotDotExpr,
        NodeExtern,
        NodeAsExpr,
        NodeGroupExpr,
        NodeStructDef,
        NodePath,

        NodePreDefTy,
        NodeArray,
        NodePtrTy,
        NodeRefTy,
        NodeFnTy,
        NodeConstTy,

        NodeIfStm,
        NodeWhileStm,
        NodeTypeStm,
        NodeForStm,
        NodeLetStm,
        NodeRetStm,
        NodeBranchStm,
        NodeStructStm,
        NodeStm,
        NodeImpl,
        NodeMatchStmt,
        NodeUseStmt,
        
        NodeCallExpr,
        NodeFNStm,

        NodeDeref,
        NodeRef,
    };


    class Ast {
    protected:
        Type *type;
        Type *castTo;
        VALUE *value;
        Ast* Decl;
    public:
        virtual ~Ast() = default;

        inline void setDecl(Ast *_Decl) { Decl = _Decl; }
        inline Ast* getDecl() const { return Decl; }
        inline void setType(Type *_type) { type = _type; }
        inline void setValue(VALUE *_value) { value = _value; }
        inline void setTypeValue(Type *_type, VALUE *_value) {
            type = _type;
            value = _value; 
        }
        inline Type* getTypeInfo() const { return type; }
        inline VALUE* getValueInfo() const { return value; }
        inline bool setCastTo(Type *to) { 
            castTo = to;
        }
        virtual Lexeme token() const = 0;
        virtual std::string toString() const = 0;
        virtual bool accept(AstVisitor& visitor) = 0;
        virtual NodeCategory nodeCategory() const = 0;
    };
   



    class BlockStmt: public Ast {
    private:
        std::vector<Ast *> statms;
    public:
        BlockStmt(std::vector<Ast *> &_state)
            :statms(_state) {}

        static BlockStmt *Create(std::vector<Ast *> &statms);
        std::vector<Ast *> getStmts() const {return statms;}
        std::string toString() const;
        bool accept(AstVisitor& visitor);
        NodeCategory nodeCategory() const { return NodeBlockStm; }
    };

  
    class NumericLiteral: public Ast {
        private:
        Lexeme tok;
      //  std::string Int;
        public:
        NumericLiteral(Lexeme &_tok)
        : tok(_tok) {}

        static NumericLiteral *Create(Lexeme _tok);
        Lexeme token() const { return tok; };
        std::string toString() const;
        bool accept(AstVisitor& visitor);
        
        NodeCategory nodeCategory() const { return NodeNumLit; }
    };

    class BoolLiteral: public Ast {
        private:
      //  std::string val;
        Lexeme tok;
        public:
        BoolLiteral(Lexeme &_tok)
        : tok(_tok) {}

        static BoolLiteral *Create(Lexeme _tok);
        //std::string value() const;
        Lexeme token() const { return tok; }
        std::string toString() const;
        bool accept(AstVisitor& visitor);
        
        NodeCategory nodeCategory() const { return NodeBoolLit; }
    };

    class StringLiteral: public Ast {
        private:
        Lexeme tok;
        bool chr;

        public:
        StringLiteral(Lexeme _tok, bool _chr)
        : tok(_tok), chr(_chr) {}

        static StringLiteral *Create(Lexeme &_tok, bool _chr);
        Lexeme token() const{ return tok; }
        //std::string value() const{ return tok.data; }
        bool ischar() const{ return chr; }
        std::string toString() const;
        bool accept(AstVisitor& visitor);
        
        NodeCategory nodeCategory() const { return NodeStrLit; }
    };


    class NullLiteral: public Ast {
        private:
        Lexeme tok;
        public:
        NullLiteral(Lexeme &_tok)
        : tok(_tok) {}

        static NullLiteral *Create(Lexeme _tok);
        Lexeme token() const{ return tok; }
        std::string toString() const;
        bool accept(AstVisitor& visitor);
        
        NodeCategory nodeCategory() const { return NodeNullLit; }
    };

    class FloatLiteral: public Ast {
        private:
        Lexeme tok;

        public:
        FloatLiteral(Lexeme &_tok)
        :tok(_tok) {}

        static FloatLiteral *Create(Lexeme _tok);
        Lexeme token() const{ return tok; }
        //std::string value() const { return tok.data; }
        std::string toString() const;
        bool accept(AstVisitor& visitor);
        
        NodeCategory nodeCategory() const { return NodeFloatLit; }
    };


    class Identifier: public Ast {
        private:
        Lexeme tok;

        public:
        Identifier(Lexeme &_tok)
            :tok(_tok) {}

        static Identifier *Create(Lexeme &_tok);
        Lexeme token() const { return tok; }
        std::string getIdent() const { return tok.getStr(); }
        // bool HasSelf() const { return IsSelf; }
        std::string toString() const;
        bool accept(AstVisitor& visitor);
        
        NodeCategory nodeCategory() const { return NodeIdent; }
    };

    class UseStmt: public Ast {
        private:
        Lexeme tok;
        Ast* path;
        public:
        UseStmt(Lexeme &_tok, Ast *&_path)
            :tok(_tok), path(_path) {}

        static UseStmt *Create(Lexeme &_tok, Ast *&_path);
        Lexeme token() const { return tok; }
        Ast* getPath() const { return path; }
        std::string toString() const;
        bool accept(AstVisitor& visitor);
        
        NodeCategory nodeCategory() const { return NodeUseStmt; }
    };


    class EnumExpr: public Ast {
        private:
        Lexeme Name;
        std::vector<Lexeme>udata;
        std::vector<Ast *>val;
        public:
        EnumExpr(Lexeme &_Name, std::vector<Lexeme>&_u_data, std::vector<Ast *>&_val)
        :Name(_Name), udata(_u_data), val(_val)
        {}

        static EnumExpr *Create(Lexeme &_Name, std::vector<Lexeme>&_u_data, std::vector<Ast *>&_val);
        // Lexeme token() const{ return tok; }
        // Ast * getType() const{ return ty; }
        Lexeme getName() const{ return Name; }
        std::vector<Lexeme> getuData() const{ return udata; }
        std::vector<Ast *> getValue() const{ return val; }
        std::string toString() const;
        bool accept(AstVisitor& visitor);
        
        NodeCategory nodeCategory() const { return NodeEnum; }
    };

    class Method: public Ast {
    private:
        Lexeme tok;
        Ast* Name;
        Ast * AssociateTo;
        std::vector<Ast *>Impl;
    public:
        Method(Lexeme &_tok, Ast *&_Name, Ast * &_AssociateTo, std::vector<Ast *>&_impl)
        :tok(_tok), Name(_Name), AssociateTo(_AssociateTo), Impl(_impl)
        {}

        static Method *Create(Lexeme &_tok, Ast *&_Name, Ast * &_AssociateTo, std::vector<Ast *>&_impl);
        Lexeme token() const{ return tok; }
        Ast* getName() const{ return Name; }
        Ast * getAssociativity() const{ return AssociateTo; }
        std::vector<Ast *> getImpl() const{ return Impl; }
        std::string toString() const;
        bool accept(AstVisitor& visitor);
        
        NodeCategory nodeCategory() const { return NodeImpl; }
    };


    class PreDefineType: public Ast{
        private:
        Lexeme tok;
        //TypePtr typeinfo;
        public:
        PreDefineType(Lexeme &_tok)
        : tok(_tok) 
        {}
        static PreDefineType *Create(Lexeme &_tok);
        Lexeme token() const{ return tok; }
        Tok getType() const{ return tok.getTok(); }
        Token_type getTType() const{ return tok.getTokTy(); }
        std::string toString() const;
        bool accept(AstVisitor& visitor);
        
        NodeCategory nodeCategory() const {return NodePreDefTy;}

    };

    class PrefixExpr: public Ast{
        private:
        Lexeme tok;
        Tok op;
        Ast * base;
        bool isType;
        int DefCount;
        NodeCategory nodeKind;
        public:
        PrefixExpr(Lexeme &tok, Tok &_op, Ast * &_type, bool &_isType, int &_DefCount, NodeCategory _nodeKind)
        : tok(tok), op(_op), base(_type), isType(_isType), DefCount(_DefCount), nodeKind(_nodeKind) {}

        static PrefixExpr *Create(Lexeme &tok, Tok &_op, Ast * &_type, bool _isType, int _DefCount, NodeCategory _nodeKind);
        Lexeme token() const { return tok; }
        Tok getOp() const { return op; }
        int getDefCount() const { return DefCount; }
        Ast * getBase() const { return base; }
        bool IsType() const { return isType; }
        std::string toString() const;
        bool accept(AstVisitor& visitor);
        
        NodeCategory nodeCategory() const { return nodeKind; }
    };

    

    class PostfixExpr: public Ast{
        private:
        Lexeme tok;
        Ast * var;
        std::vector<Lexeme>Temp;
        std::vector<Ast *>inexpr;
        NodeCategory nodeKind;
        // bool isType;
        public:
        // PostfixExpr(Lexeme &tok,  Ast * &_var, std::vector<Lexeme>&_Temp, std::vector<Ast *> &_v, NodeCategory _nodeKind)
        // : tok(tok), var(_var), Temp(_Temp), expr(_v), nodeKind(_nodeKind) {}
        
        // PostfixExpr(Lexeme &tok,  Ast * &_var, std::vector<Ast *> &_v, NodeCategory _nodeKind)
        // : tok(tok), var(_var), expr(_v), nodeKind(_nodeKind) {}

        static PostfixExpr *Create(Lexeme &tok,  Ast * &_var, std::vector<Lexeme>&_Temp, std::vector<Ast *> &_v, NodeCategory _nodeKind);
        static PostfixExpr *Create(Lexeme &tok,  Ast * &_var, std::vector<Ast *> &_v, NodeCategory _nodeKind);
        void SetType(Type *_type) { type = _type; }
        Lexeme token() const { return tok; }
        Ast * getVar() const { return var; }
        std::vector<Lexeme> getTemp() const { return Temp; }
        bool HasTemp() const {return !Temp.empty();}
        std::vector<Ast *> getExpr() const { return inexpr; }
        std::string toString() const;
        bool accept(AstVisitor& visitor);
        
        NodeCategory nodeCategory() const { return nodeKind; }
    };


    class Expression: public Ast{
        private:
        Ast * LHS;
        Tok Op;
        Ast * RHS;
        bool isInParen;
        NodeCategory NodeKind;
        public:
        Expression(Ast *&_LHS, Tok _op, Ast *&_RHS,
                     NodeCategory &_nodeKind)
        :LHS(_LHS),Op(_op), RHS(_RHS), NodeKind(_nodeKind)
        {}

        static Expression *Create(Ast *_LHS, Tok _op, Ast *_RHS,
                     NodeCategory _nodeKind);
        // Lexeme token() const {return tok;}
        Ast * getLhs() const {return LHS;}
        Tok getOp() const {return Op;}
        Ast * getRhs() const {return RHS;}
        // bool isInGrouped() const{return isInParen;}
        std::string toString() const;
        bool accept(AstVisitor& visitor);
        
        NodeCategory nodeCategory()const {return NodeKind;}

    };

    class GroupedExpr:public Ast {
        Ast * expr;
        public:
        GroupedExpr(Ast * &_expr)
        :expr(_expr) {}

        static GroupedExpr *Create(Ast * &_expr);
        Ast * getExpression() const { return expr; }
        std::string toString() const;
        bool accept(AstVisitor& visitor);
        
        NodeCategory nodeCategory() const {return NodeGroupExpr;}
    };

    class FnType: public Ast {
        private:
        Lexeme tok;
        std::vector<Ast *> ty;
        Ast * ret;
       
        public:
        FnType(Lexeme &tok, std::vector<Ast *> &_ty, Ast * &_ret)
        : tok(tok), ty(_ty), ret(_ret) {}

        static FnType *Create(Lexeme &tok, std::vector<Ast *> &_ty, Ast * &_ret);
        Lexeme token() const { return tok; }
        std::vector<Ast *> getParamType() const { return ty; }
        Ast * getRetType() const { return ret; }
        std::string toString() const;
        bool accept(AstVisitor& visitor);
        
        NodeCategory nodeCategory() const { return NodeFnTy; }
    };

    class TypeStmt: public Ast {
        private:
        Lexeme tok;
        Lexeme Ident;
        Ast * type;

        public:
        TypeStmt(Lexeme &_tok, Lexeme &_Ident ,Ast * &_type)
        : tok(_tok), Ident(_Ident), type(_type) {}

        static TypeStmt *Create(Lexeme &_tok, Lexeme &_Ident ,Ast * &_type);
        Lexeme token() const { return tok; }
        Lexeme getIdent() const{ return Ident; }
        Ast * getType() const{ return type; }
        std::string toString() const;
        bool accept(AstVisitor& visitor);
        
        NodeCategory nodeCategory() const { return NodeTypeStm; }
    };

    
    class Array: public Ast {
        private:
        Lexeme tok;
        std::vector<Ast *> size;
        Ast * type;
        public:
        Array(Lexeme &_tok,std::vector<Ast *> &_size, Ast * &_type)
        : tok(_tok), size(_size), type(_type) {}

        static Array *Create(Lexeme &_tok,std::vector<Ast *> &_size, Ast * &_type);
        Lexeme token() const{ return tok; }
        std::vector<Ast *> getArraySize() const{return size;}
        Ast * getArrayType() const{return type;}
        std::string toString() const;
        bool accept(AstVisitor& visitor);
        
        NodeCategory nodeCategory() const { return NodeArray; }
    };

  

    class Extern: public Ast {
        private:
        Lexeme tok;
        std::vector<Ast *> header;
        public:
        Extern(Lexeme &_tok, std::vector<Ast *> &_header)
        :tok(_tok), header(_header){}

        static Extern *Create(Lexeme &_tok, std::vector<Ast *> &_header);
        Lexeme token() const {return tok;}
        std::vector<Ast *> getHeader() const {return header;}
        // Ast * getBlock()const {return block;}
        std::string toString() const;
        bool accept(AstVisitor& visitor);
        //void accept(AstVisitor &vistitor) const;
        NodeCategory nodeCategory() const {return NodeExtern;}
    };


    class WhileLoop: public Ast{
    private:
        Lexeme tok;
        Ast * expr;
        Ast * body;

    public:
        WhileLoop(Lexeme &_tok, Ast * &_expr, Ast * &_body )
            :tok(_tok), expr(_expr), body(_body)  {}

        static WhileLoop *Create(Lexeme &_tok, Ast * &_expr, Ast * &_body );
        Ast * getCond() const { return expr; }
        Ast * getBlock() const { return body; }
        Lexeme token() const {return tok; }
        std::string toString() const;
        bool accept(AstVisitor& visitor);
        
        NodeCategory nodeCategory() const { return NodeWhileStm; }

    };


    class ForLoop: public Ast{
    private:
        Lexeme tok;
        Ast * var;
        Ast * cond;
        Ast * incr;
        Ast * body;

    public:
        ForLoop(Lexeme &_tok, Ast * &_var, Ast * &_cond, Ast * &_incr, Ast * &_body )
            :tok(_tok), var(_var), cond(_cond), incr(_incr), body(_body)  {}

        static ForLoop *Create(Lexeme &_tok, Ast * &_var, Ast * &_cond, Ast * &_incr, Ast * &_body );
        Ast * getVar() const { return var; }
        Ast * getCond() const { return cond; }
        Ast * getIncr() const { return incr; }
        Ast * getBlock() const { return body; }
        Lexeme token() const {return tok; }
        std::string toString() const;
        bool accept(AstVisitor& visitor) ;
        
        NodeCategory nodeCategory() const { return NodeForStm; }

    };

    class IfStmt: public Ast{
        private:
        Lexeme tok;
        Ast * cond;
        Ast * ifblock;
        Ast * elblock;
        
        public:
        IfStmt(Lexeme &_tok, Ast * &_cond , Ast * &_ifblock,
                             Ast * &_elblock)
        : tok(_tok), cond(_cond), ifblock(_ifblock), 
                        elblock(_elblock) {}
        
        static ForLoop *Create(Lexeme &_tok, Ast * &_cond , Ast * &_ifblock,
                             Ast * &_elblock);
        Lexeme token() const { return tok; }
        Ast * getCondV() const { return cond; }
        Ast * getIfBlock() const { return ifblock; }
        Ast * getElBlock() const { return  elblock; }
        std::string toString() const;
        bool accept(AstVisitor& visitor);
        
        NodeCategory nodeCategory() const { return NodeIfStm; }
    };


    class BranchStmt: public Ast {
        private:
        Lexeme tok;
        public:
        BranchStmt(Lexeme &_tok)
        : tok(_tok) {}

        static BranchStmt *Create(Lexeme &_tok);
        Lexeme token() const { return tok; }
        std::string toString() const;
        bool accept(AstVisitor& visitor);
        
        NodeCategory nodeCategory() const { return NodeBranchStm; }
    };


    class StructStmt: public Ast {
        private:
        Lexeme tok;
        Lexeme Name;
        std::vector<Lexeme>Temp;
        std::vector<Lexeme> elmtName;
        std::vector<Ast *> elmtTy;
        // bool isDecl;
        public:
        StructStmt(Lexeme &_tok, Lexeme &_Name, std::vector<Lexeme> &_Temp, std::vector<Lexeme> &_memName,
                     std::vector<Ast *> &_memTy)
        : tok(_tok), Name(_Name), Temp(_Temp), elmtName(_memName), elmtTy(_memTy) {}

        static BranchStmt *Create(Lexeme &_tok, Lexeme &_Name, std::vector<Lexeme> &_Temp, std::vector<Lexeme> &_memName,
                     std::vector<Ast *> &_memTy);
    
        Lexeme token() const { return tok; }
        Lexeme getName() const { return Name; }
        std::vector<Lexeme> getTemp() const { return Temp; }
        bool HasTemplate() const { return !Temp.empty(); }
        std::vector<Lexeme> getIdentList() const { return elmtName; }
        std::vector<Ast *> getTypeList() const { return elmtTy; }
        std::string toString() const;
        bool accept(AstVisitor& visitor);
        
        NodeCategory nodeCategory() const { return NodeStructStm; }
    };


    class ListExpr: public Ast {
        private:
        Lexeme tok;
        std::vector<Ast *>list;
        public:
        ListExpr(Lexeme &_tok, std::vector<Ast *> &_list)
        :tok(_tok), list(_list) {}

        static ListExpr *Create(Lexeme &_tok, std::vector<Ast *> &_list);
        Lexeme token() const { return tok; }
        std::vector<Ast *> getList() const { return list; }
        std::string toString() const;
        bool accept(AstVisitor& visitor);
        
        NodeCategory nodeCategory() const { return NodeListExpr; }
    };


    class VarStmt: public Ast {
        private:
        Lexeme tok;
        Ast * name;
        Ast * type;
        //TypePtr typeinfo;
        Ast * val;
        bool HasConst;
        public:
        VarStmt(Lexeme &_tok, Ast * &_var, Ast * &_type, Ast * &_val, bool &_HasConst)
        : tok(_tok), name(_var), type(_type), val(_val), HasConst(_HasConst) {}

        static ListExpr *Create(Lexeme &_tok, Ast *_var, Ast *_type, Ast *_val, bool _HasConst);
        Lexeme token() const{ return tok; }
        Ast * getVarName() const{return name;}
        Ast * getType() const{return type;}
        void SetType(Ast * &Type) { type = Type;}
        Ast * getVal() const{return val;}
        bool HasConstStmt() const { return HasConst; }
        // bool getHasIsStmt()const {return HasIsStmt;}
        // TypePtr getType() const {return typeinfo;}
        // void setType(const TypePtr _typeinfo) {typeinfo = _typeinfo;}
        std::string toString() const;
        bool accept(AstVisitor& visitor);
        
        NodeCategory nodeCategory() const { return NodeLetStm; }
    };


    class FunctionDef: public Ast {
    private:
        Lexeme tok;
        Lexeme Name;
        std::vector<Lexeme> pName;
        std::vector<Ast *> pTy;
        Ast * retype;
        Ast * Block;
        // bool isDecl;
    public:
        FunctionDef(Lexeme &_tok, Lexeme &_Name, std::vector<Lexeme> &_pName,
                    std::vector<Ast *> &_pTy, Ast *  _retype, Ast * &_Block)
         : tok(_tok), Name(_Name), pName(_pName), pTy(_pTy), 
            retype(_retype), Block(_Block){}

        static FunctionDef *Create(Lexeme &_tok, Lexeme &_Name, std::vector<Lexeme> &_pName,
                    std::vector<Ast *> &_pTy, Ast *  _retype, Ast * &_Block);
        Lexeme token() const { return tok; }
        Lexeme getFuncName() const { return Name; }
        std::vector<Lexeme> getParameterNames() const { return pName; }
        std::vector<Ast *> getParameterTy() const { return pTy; }
        Ast * getResultType() const { return retype; }
        Ast * getFuncBlock() const { return Block; }
        std::string toString() const;
        bool accept(AstVisitor& visitor);
        
        NodeCategory nodeCategory() const { return NodeFNStm; }
    };


    class ReturnStmt: public Ast {
        private:
        Lexeme tok;
        Ast * val;

        public:
        ReturnStmt(Lexeme &_tok, Ast * &_val)
        : tok(_tok), val(_val) {}

        static ReturnStmt *Create(Lexeme &_tok, Ast * &_val);
        Lexeme token() const { return tok; }
        Ast * getRetValue() const { return val; }
        std::string toString() const;
        bool accept(AstVisitor& visitor);
        
        NodeCategory nodeCategory() const { return NodeRetStm; }
    };


    class FunctionCall: public Ast {
        private:
        Lexeme tok;
        Ast * name;
        std::vector<Ast *> args;

        public:
        FunctionCall(Lexeme &_tok, Ast * &_name, std::vector<Ast *> &_args)
        : tok(_tok), name(_name), args(_args) {}


        static FunctionCall *Create(Lexeme &_tok, Ast *_name, std::vector<Ast *>&_args);
        Lexeme token() const { return tok; }
        Ast * getCalle() const { return name; }
        std::vector<Ast *> getArgs() const { return args; }
        std::string toString() const;
        bool accept(AstVisitor& visitor);
        
        NodeCategory nodeCategory() const { return NodeCallExpr; }
    };

}
