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

        NodePreDefTy,
        NodeArray,
        NodePtr,
        NodeRef,
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
        NodeMatchStmt,
        
        NodeCallExpr,
        NodeFNStm
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
        virtual tokt token() const = 0;
        virtual std::string toString() const = 0;
        virtual bool accept(AstVisitor& visitor) const;
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
        bool accept(AstVisitor& visitor) const;
        NodeCategory nodeCategory() const { return NodeBlockStm; }
    };

  
    class NumericLiteral: public Ast {
        private:
        tokt tok;
      //  std::string Int;
        public:
        NumericLiteral(tokt &_tok)
        : tok(_tok) {}

        static NumericLiteral *Create(tokt &_tok);
        tokt token() const { return tok; };
        std::string toString() const;
        bool accept(AstVisitor& visitor) const;
        
        NodeCategory nodeCategory() const { return NodeNumLit; }
    };

    class BoolLiteral: public Ast {
        private:
      //  std::string val;
        tokt tok;
        public:
        BoolLiteral(tokt &_tok)
        : tok(_tok) {}

        static BoolLiteral *Create(tokt &_tok);
        //std::string value() const;
        tokt token() const { return tok; }
        std::string toString() const;
        bool accept(AstVisitor& visitor) const;
        
        NodeCategory nodeCategory() const { return NodeBoolLit; }
    };

    class StringLiteral: public Ast {
        private:
        tokt tok;
        bool chr;

        public:
        StringLiteral(tokt &_tok, bool _chr)
        : tok(_tok), chr(_chr) {}

        static StringLiteral *Create(tokt &_tok, bool _chr);
        tokt token() const{ return tok; }
        //std::string value() const{ return tok.data; }
        bool ischar() const{ return chr; }
        std::string toString() const;
        bool accept(AstVisitor& visitor) const;
        
        NodeCategory nodeCategory() const { return NodeStrLit; }
    };


    class NullLiteral: public Ast {
        private:
        tokt tok;
        public:
        NullLiteral(tokt &_tok)
        : tok(_tok) {}

        static NullLiteral *Create(tokt &_tok);
        tokt token() const{ return tok; }
        std::string toString() const;
        bool accept(AstVisitor& visitor) const;
        
        NodeCategory nodeCategory() const { return NodeNullLit; }
    };

    class FloatLiteral: public Ast {
        private:
        tokt tok;

        public:
        FloatLiteral(tokt &_tok)
        :tok(_tok) {}

        static FloatLiteral *Create(tokt &_tok);
        tokt token() const{ return tok; }
        //std::string value() const { return tok.data; }
        std::string toString() const;
        bool accept(AstVisitor& visitor) const;
        
        NodeCategory nodeCategory() const { return NodeFloatLit; }
    };


    class Identifier: public Ast {
        private:
        tokt tok;

        public:
        Identifier(tokt &_tok)
            :tok(_tok) {}

        static FloatLiteral *Create(tokt &_tok);
        tokt token() const { return tok; }
        std::string getIdent() const { return tok.data; }
        // bool HasSelf() const { return IsSelf; }
        std::string toString() const;
        bool accept(AstVisitor& visitor) const;
        
        NodeCategory nodeCategory() const { return NodeIdent; }
    };


    class EnumExpr: public Ast {
        private:
        tokt Name;
        std::vector<tokt>udata;
        std::vector<Ast *>val;
        public:
        EnumExpr(tokt &_Name, std::vector<tokt>&_u_data, std::vector<Ast *>&_val)
        :Name(_Name), udata(_u_data), val(_val)
        {}

        static EnumExpr *Create(tokt &_Name, std::vector<tokt>&_u_data, std::vector<Ast *>&_val);
        // tokt token() const{ return tok; }
        // Ast * getType() const{ return ty; }
        tokt getName() const{ return Name; }
        std::vector<tokt> getuData() const{ return udata; }
        std::vector<Ast *> getValue() const{ return val; }
        std::string toString() const;
        bool accept(AstVisitor& visitor) const;
        
        NodeCategory nodeCategory() const { return NodeEnum; }
    };

    class Method: public Ast {
    private:
        tokt tok;
        tokt Name;
        Ast * AssociateTo;
        std::vector<Ast *>Impl;
    public:
        Method(tokt &_tok, tokt &_Name, Ast * &_AssociateTo, std::vector<Ast *>&_impl)
        :tok(_tok), Name(_Name), AssociateTo(_AssociateTo), Impl(_impl)
        {}

        static Method *Create(tokt &_tok, tokt &_Name, Ast * &_AssociateTo, std::vector<Ast *>&_impl);
        tokt token() const{ return tok; }
        tokt getName() const{ return Name; }
        Ast * getAssociativity() const{ return AssociateTo; }
        std::vector<Ast *> getImpl() const{ return Impl; }
        std::string toString() const;
        bool accept(AstVisitor& visitor) const;
        
        NodeCategory nodeCategory() const { return NodeEnum; }
    };


    class PreDefineType: public Ast{
        private:
        tokt tok;
        //TypePtr typeinfo;
        public:
        PreDefineType(tokt &_tok)
        : tok(_tok) 
        {}
        static PreDefineType *Create(tokt &_tok);
        tokt token() const{ return tok; }
        Token_type getType() const{ return tok.tok_type; }
        std::string toString() const;
        bool accept(AstVisitor& visitor) const;
        
        NodeCategory nodeCategory() const {return NodePreDefTy;}

    };

    class PrefixExpr: public Ast{
        private:
        tokt tok;
        Token_type op;
        Ast * base;
        bool isType;
        int DefCount;
        NodeCategory nodeKind;
        public:
        PrefixExpr(tokt &tok, Token_type &_op, Ast * &_type, bool &_isType, int &_DefCount, NodeCategory _nodeKind)
        : tok(tok), op(_op), base(_type), isType(_isType), DefCount(_DefCount), nodeKind(_nodeKind) {}

        static PrefixExpr *Create(tokt &tok, Token_type &_op, Ast * &_type, bool _isType, int _DefCount, NodeCategory _nodeKind);
        tokt token() const { return tok; }
        Token_type getOp() const { return op; }
        int getDefCount() const { return DefCount; }
        Ast * getBase() const { return base; }
        bool IsType() const { return isType; }
        std::string toString() const;
        bool accept(AstVisitor& visitor) const;
        
        NodeCategory nodeCategory() const { return nodeKind; }
    };

    

    class PostfixExpr: public Ast{
        private:
        tokt tok;
        Ast * var;
        std::vector<tokt>Temp;
        std::vector<Ast *>expr;
        NodeCategory nodeKind;
        // bool isType;
        public:
        PostfixExpr(tokt &tok,  Ast * &_var, std::vector<tokt>&_Temp, std::vector<Ast *> &_v, NodeCategory _nodeKind)
        : tok(tok), var(_var), Temp(_Temp), expr(_v), nodeKind(_nodeKind) {}
        
        PostfixExpr(tokt &tok,  Ast * &_var, std::vector<Ast *> &_v, NodeCategory _nodeKind)
        : tok(tok), var(_var), expr(_v), nodeKind(_nodeKind) {}

        static PostfixExpr *Create(tokt &tok,  Ast * &_var, std::vector<tokt>&_Temp, std::vector<Ast *> &_v, NodeCategory _nodeKind);
        static PostfixExpr *Create(tokt &tok,  Ast * &_var, std::vector<Ast *> &_v, NodeCategory _nodeKind);
        void SetType(Type *_type) { type = _type; }
        tokt token() const { return tok; }
        Ast * getVar() const { return var; }
        std::vector<tokt> getTemp() const { return Temp; }
        bool HasTemp() const {return !Temp.empty();}
        std::vector<Ast *> getExpr() const { return expr; }
        std::string toString() const;
        bool accept(AstVisitor& visitor) const;
        
        NodeCategory nodeCategory() const { return nodeKind; }
    };

    class Expression: public Ast{
        private:
        Ast * LHS;
        Token_type Op;
        Ast * RHS;
        bool isInParen;
        NodeCategory NodeKind;
        public:
        Expression(Ast *&_LHS, Token_type _op, Ast *&_RHS,
                     NodeCategory &_nodeKind)
        :LHS(_LHS),Op(_op), RHS(_RHS), NodeKind(_nodeKind)
        {}

        static Expression *Create(Ast *_LHS, Token_type _op, Ast *_RHS,
                     NodeCategory _nodeKind);
        // tokt token() const {return tok;}
        Ast * getLhs() const {return LHS;}
        Token_type getOp() const {return Op;}
        Ast * getRhs() const {return RHS;}
        // bool isInGrouped() const{return isInParen;}
        std::string toString() const;
        bool accept(AstVisitor& visitor) const;
        
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
        bool accept(AstVisitor& visitor) const;
        
        NodeCategory nodeCategory() const {return NodeGroupExpr;}
    };

    class FnType: public Ast {
        private:
        tokt tok;
        std::vector<Ast *> ty;
        Ast * ret;
       
        public:
        FnType(tokt &tok, std::vector<Ast *> &_ty, Ast * &_ret)
        : tok(tok), ty(_ty), ret(_ret) {}

        static FnType *Create(tokt &tok, std::vector<Ast *> &_ty, Ast * &_ret);
        tokt token() const { return tok; }
        std::vector<Ast *> getParamType() const { return ty; }
        Ast * getRetType() const { return ret; }
        std::string toString() const;
        bool accept(AstVisitor& visitor) const;
        
        NodeCategory nodeCategory() const { return NodeFnTy; }
    };

    class TypeStmt: public Ast {
        private:
        tokt tok;
        tokt Ident;
        Ast * type;

        public:
        TypeStmt(tokt &_tok, tokt &_Ident ,Ast * &_type)
        : tok(_tok), Ident(_Ident), type(_type) {}

        static TypeStmt *Create(tokt &_tok, tokt &_Ident ,Ast * &_type);
        tokt token() const { return tok; }
        tokt getIdent() const{ return Ident; }
        Ast * getType() const{ return type; }
        std::string toString() const;
        bool accept(AstVisitor& visitor) const;
        
        NodeCategory nodeCategory() const { return NodeTypeStm; }
    };

    
    class Array: public Ast {
        private:
        tokt tok;
        std::vector<Ast *> size;
        Ast * type;
        public:
        Array(tokt &_tok,std::vector<Ast *> &_size, Ast * &_type)
        : tok(_tok), size(_size), type(_type) {}

        static Array *Create(tokt &_tok,std::vector<Ast *> &_size, Ast * &_type);
        tokt token() const{ return tok; }
        std::vector<Ast *> getArraySize() const{return size;}
        Ast * getArrayType() const{return type;}
        std::string toString() const;
        bool accept(AstVisitor& visitor) const;
        
        NodeCategory nodeCategory() const { return NodeArray; }
    };

  

    class Extern: public Ast {
        private:
        tokt tok;
        std::vector<Ast *> header;
        public:
        Extern(tokt &_tok, std::vector<Ast *> &_header)
        :tok(_tok), header(_header){}

        static Extern *Create(tokt &_tok, std::vector<Ast *> &_header);
        tokt token() const {return tok;}
        std::vector<Ast *> getHeader() const {return header;}
        // Ast * getBlock()const {return block;}
        std::string toString() const;
        bool accept(AstVisitor& visitor) const;
        //void accept(AstVisitor &vistitor) const;
        NodeCategory nodeCategory() const {return NodeExtern;}
    };


    class WhileLoop: public Ast{
    private:
        tokt tok;
        Ast * expr;
        Ast * body;

    public:
        WhileLoop(tokt &_tok, Ast * &_expr, Ast * &_body )
            :tok(_tok), expr(_expr), body(_body)  {}

        static WhileLoop *Create(tokt &_tok, Ast * &_expr, Ast * &_body );
        Ast * getCond() const { return expr; }
        Ast * getBlock() const { return body; }
        tokt token() const {return tok; }
        std::string toString() const;
        bool accept(AstVisitor& visitor) const;
        
        NodeCategory nodeCategory() const { return NodeWhileStm; }

    };


    class ForLoop: public Ast{
    private:
        tokt tok;
        Ast * var;
        Ast * cond;
        Ast * incr;
        Ast * body;

    public:
        ForLoop(tokt &_tok, Ast * &_var, Ast * &_cond, Ast * &_incr, Ast * &_body )
            :tok(_tok), var(_var), cond(_cond), incr(_incr), body(_body)  {}

        static ForLoop *Create(tokt &_tok, Ast * &_var, Ast * &_cond, Ast * &_incr, Ast * &_body );
        Ast * getVar() const { return var; }
        Ast * getCond() const { return cond; }
        Ast * getIncr() const { return incr; }
        Ast * getBlock() const { return body; }
        tokt token() const {return tok; }
        std::string toString() const;
        bool accept(AstVisitor& visitor) const;
        
        NodeCategory nodeCategory() const { return NodeForStm; }

    };

    class IfStmt: public Ast{
        private:
        tokt tok;
        Ast * cond;
        Ast * ifblock;
        Ast * elblock;
        
        public:
        IfStmt(tokt &_tok, Ast * &_cond , Ast * &_ifblock,
                             Ast * &_elblock)
        : tok(_tok), cond(_cond), ifblock(_ifblock), 
                        elblock(_elblock) {}
        
        static ForLoop *Create(tokt &_tok, Ast * &_cond , Ast * &_ifblock,
                             Ast * &_elblock);
        tokt token() const { return tok; }
        Ast * getCondV() const { return cond; }
        Ast * getIfBlock() const { return ifblock; }
        Ast * getElBlock() const { return  elblock; }
        std::string toString() const;
        bool accept(AstVisitor& visitor) const;
        
        NodeCategory nodeCategory() const { return NodeIfStm; }
    };


    class BranchStmt: public Ast {
        private:
        tokt tok;
        public:
        BranchStmt(tokt &_tok)
        : tok(_tok) {}

        static BranchStmt *Create(tokt &_tok);
        tokt token() const { return tok; }
        std::string toString() const;
        bool accept(AstVisitor& visitor) const;
        
        NodeCategory nodeCategory() const { return NodeBranchStm; }
    };


    class StructStmt: public Ast {
        private:
        tokt tok;
        tokt Name;
        std::vector<tokt>Temp;
        std::vector<tokt> elmtName;
        std::vector<Ast *> elmtTy;
        // bool isDecl;
        public:
        StructStmt(tokt &_tok, tokt &_Name, std::vector<tokt> &_Temp, std::vector<tokt> &_memName,
                     std::vector<Ast *> &_memTy)
        : tok(_tok), Name(_Name), Temp(_Temp), elmtName(_memName), elmtTy(_memTy) {}

        static BranchStmt *Create(tokt &_tok, tokt &_Name, std::vector<tokt> &_Temp, std::vector<tokt> &_memName,
                     std::vector<Ast *> &_memTy);
    
        tokt token() const { return tok; }
        tokt getName() const { return Name; }
        std::vector<tokt> getTemp() const { return Temp; }
        bool HasTemplate() const { return !Temp.empty(); }
        std::vector<tokt> getIdentList() const { return elmtName; }
        std::vector<Ast *> getTypeList() const { return elmtTy; }
        std::string toString() const;
        bool accept(AstVisitor& visitor) const;
        
        NodeCategory nodeCategory() const { return NodeStructStm; }
    };


    class ListExpr: public Ast {
        private:
        tokt tok;
        std::vector<Ast *>list;
        public:
        ListExpr(tokt &_tok, std::vector<Ast *> &_list)
        :tok(_tok), list(_list) {}

        static ListExpr *Create(tokt &_tok, std::vector<Ast *> &_list);
        tokt token() const { return tok; }
        std::vector<Ast *> getList() const { return list; }
        std::string toString() const;
        bool accept(AstVisitor& visitor) const;
        
        NodeCategory nodeCategory() const { return NodeListExpr; }
    };


    class VarStmt: public Ast {
        private:
        tokt tok;
        Ast * name;
        Ast * type;
        //TypePtr typeinfo;
        Ast * val;
        bool HasConst;
        public:
        VarStmt(tokt &_tok, Ast * &_var, Ast * &_type, Ast * &_val, bool &_HasConst)
        : tok(_tok), name(_var), type(_type), val(_val), HasConst(_HasConst) {}

        static ListExpr *Create(tokt &_tok, Ast *_var, Ast *_type, Ast *_val, bool _HasConst);
        tokt token() const{ return tok; }
        Ast * getVarName() const{return name;}
        Ast * getType() const{return type;}
        void SetType(Ast * &Type) { type = Type;}
        Ast * getVal() const{return val;}
        bool HasConstStmt() const { return HasConst; }
        // bool getHasIsStmt()const {return HasIsStmt;}
        // TypePtr getType() const {return typeinfo;}
        // void setType(const TypePtr _typeinfo) {typeinfo = _typeinfo;}
        std::string toString() const;
        bool accept(AstVisitor& visitor) const;
        
        NodeCategory nodeCategory() const { return NodeLetStm; }
    };


    class FunctionDef: public Ast {
    private:
        tokt tok;
        tokt Name;
        std::vector<tokt> pName;
        std::vector<Ast *> pTy;
        Ast * retype;
        Ast * Block;
        // bool isDecl;
    public:
        FunctionDef(tokt &_tok, tokt &_Name, std::vector<tokt> &_pName,
                    std::vector<Ast *> &_pTy, Ast *  _retype, Ast * &_Block)
         : tok(_tok), Name(_Name), pName(_pName), pTy(_pTy), 
            retype(_retype), Block(_Block){}

        static FunctionDef *Create(tokt &_tok, tokt &_Name, std::vector<tokt> &_pName,
                    std::vector<Ast *> &_pTy, Ast *  _retype, Ast * &_Block);
        tokt token() const { return tok; }
        tokt getFuncName() const { return Name; }
        std::vector<tokt> getParameterNames() const { return pName; }
        std::vector<Ast *> getParameterTy() const { return pTy; }
        Ast * getResultType() const { return retype; }
        Ast * getFuncBlock() const { return Block; }
        std::string toString() const;
        bool accept(AstVisitor& visitor) const;
        
        NodeCategory nodeCategory() const { return NodeFNStm; }
    };


    class ReturnStmt: public Ast {
        private:
        tokt tok;
        Ast * val;

        public:
        ReturnStmt(tokt &_tok, Ast * &_val)
        : tok(_tok), val(_val) {}

        static ReturnStmt *Create(tokt &_tok, Ast * &_val);
        tokt token() const { return tok; }
        Ast * getRetValue() const { return val; }
        std::string toString() const;
        bool accept(AstVisitor& visitor) const;
        
        NodeCategory nodeCategory() const { return NodeRetStm; }
    };


    class FunctionCall: public Ast {
        private:
        tokt tok;
        Ast * name;
        std::vector<Ast *> args;

        public:
        FunctionCall(tokt &_tok, Ast * &_name, std::vector<Ast *> &_args)
        : tok(_tok), name(_name), args(_args) {}


        static FunctionCall *Create(tokt &_tok, Ast *_name, std::vector<Ast *>&_args);
        tokt token() const { return tok; }
        Ast * getCalle() const { return name; }
        std::vector<Ast *> getArgs() const { return args; }
        std::string toString() const;
        bool accept(AstVisitor& visitor) const;
        
        NodeCategory nodeCategory() const { return NodeCallExpr; }
    };

}
