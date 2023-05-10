#pragma once

#include<iostream>
#include<vector>
#include<memory>
#include"../lex/lex.hpp"
#include"../analyzer/AstVisitor.hpp"
#include"type.hpp"
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
        NodeFNStm,
    };


    class Ast {
    public:
        virtual ~Ast() {} //= default;
        virtual std::string toString() const = 0;
        virtual NodeCategory nodeCategory() const = 0;
        virtual void accept(AstVisitor& visitor) const = 0;
    };
   
    using AstPtr = std::shared_ptr<Ast>;



    class BlockStatement: public Ast {
    private:
        std::vector<AstPtr> statms;
    public:
        BlockStatement(std::vector<AstPtr> &_state)
            :statms(_state) {}

        std::vector<AstPtr> getStmts() const {return statms;}
        std::string toString() const;
        void accept(AstVisitor& visitor) const;
        NodeCategory nodeCategory() const { return NodeBlockStm; }
    };

  
    class NumericLiteral: public Ast {
        private:
        tokt tok;
      //  std::string Int;
        public:
        NumericLiteral(tokt &_tok)
        : tok(_tok) {}

        tokt token() const { return tok; };
        std::string toString() const;
        void accept(AstVisitor& visitor) const;
        NodeCategory nodeCategory() const { return NodeNumLit; }
    };

    class BoolLiteral: public Ast {
        private:
      //  std::string val;
        tokt tok;
        public:
        BoolLiteral(tokt &_tok)
        : tok(_tok) {}

        //std::string value() const;
        tokt token() const { return tok; }
        std::string toString() const;
        void accept(AstVisitor& visitor) const;
        NodeCategory nodeCategory() const { return NodeBoolLit; }
    };

    class StringLiteral: public Ast {
        private:
        tokt tok;
        bool chr;

        public:
        StringLiteral(tokt &_tok, bool _chr)
        : tok(_tok), chr(_chr) {}

        tokt token() const{ return tok; }
        //std::string value() const{ return tok.data; }
        bool ischar() const{ return chr; }
        std::string toString() const;
        void accept(AstVisitor& visitor) const;
        NodeCategory nodeCategory() const { return NodeStrLit; }
    };


    class NullLiteral: public Ast {
        private:
        tokt tok;
        public:
        NullLiteral(tokt &_tok)
        : tok(_tok) {}

        tokt token() const{ return tok; }
        std::string toString() const;
        void accept(AstVisitor& visitor) const;
        NodeCategory nodeCategory() const { return NodeNullLit; }
    };

    class FloatLiteral: public Ast {
        private:
        tokt tok;

        public:
        FloatLiteral(tokt &_tok)
        :tok(_tok) {}

        tokt token() const{ return tok; }
        //std::string value() const { return tok.data; }
        std::string toString() const;
        void accept(AstVisitor& visitor) const;
        NodeCategory nodeCategory() const { return NodeFloatLit; }
    };


    class Identifier: public Ast {
        private:
        tokt tok;
        //TypePtr typeinfo;
        public:
        Identifier(tokt &_tok)
            :tok(_tok) {}

        tokt token() const { return tok; }
        // TypePtr typeInfo() const { return typeinfo; }
        // void setTypeInfo(TypePtr &type) { typeinfo = type; }
        std::string ident() const { return tok.data; }
        bool HasSelf() const { return IsSelf; }
        std::string toString() const;
        void accept(AstVisitor& visitor) const;
        NodeCategory nodeCategory() const { return NodeIdent; }
    };


    class EnumExpr: public Ast {
        private:
        std::vector<tokt>udata;
        std::vector<AstPtr>val;
        public:
        EnumExpr(std::vector<tokt>&_u_data, std::vector<AstPtr>&_val)
        :udata(_u_data), val(_val)
        {}

        // tokt token() const{ return tok; }
        // AstPtr getType() const{ return ty; }
        std::vector<tokt> getuData() const{ return udata; }
        std::vector<AstPtr> getValue() const{ return val; }
        std::string toString() const;
        void accept(AstVisitor& visitor) const;
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
        //TypePtr setTy(TypePtr ty){typeinfo=ty;}
        //TypePtr getTy()const{return typeinfo;}
        tokt token() const{ return tok; }
        Token_type getType() const{ return tok.tok_type; }
        std::string toString() const;
        void accept(AstVisitor& visitor) const;
        NodeCategory nodeCategory() const {return NodePreDefTy;}

    };

    class PrefixExpr: public Ast{
        private:
        tokt tok;
        Token_type op;
        AstPtr base;
        bool isType;
        int DefCount;
        NodeCategory nodeKind;
        public:
        PrefixExpr(tokt &tok, Token_type &_op, AstPtr &_type, bool &_isType, int &_DefCount, NodeCategory _nodeKind)
        : tok(tok), op(_op), base(_type), isType(_isType), DefCount(_DefCount), nodeKind(_nodeKind) {}

        tokt token() const { return tok; }
        Token_type getOp() const { return op; }
        int getDefCount() const { return DefCount; }
        AstPtr getBase() const { return base; }
        bool IsType() const { return isType; }
        std::string toString() const;
        void accept(AstVisitor& visitor) const;
        NodeCategory nodeCategory() const { return nodeKind; }
    };

    class PostfixExpr: public Ast{
        private:
        tokt tok;
        AstPtr var;
        std::vector<tokt>Temp;
        std::vector<AstPtr>expr;
        NodeCategory nodeKind;
        bool isType;
        public:
        PostfixExpr(tokt &tok,  AstPtr &_var, std::vector<tokt>&_Temp, std::vector<AstPtr> &_v, NodeCategory _nodeKind)
        : tok(tok), var(_var), Temp(_Temp), expr(_v), nodeKind(_nodeKind) {}

        tokt token() const { return tok; }
        AstPtr getVar() const { return var; }
        std::vector<tokt> getTemp() const { return Temp; }
        bool HasTemp() const {return !Temp.empty();}
        std::vector<AstPtr> getExpr() const { return expr; }
        std::string toString() const;
        void accept(AstVisitor& visitor) const;
        NodeCategory nodeCategory() const { return nodeKind; }
    };

    class Expression: public Ast{
        private:
        AstPtr LHS;
        Token_type Op;
        AstPtr RHS;
        bool isInParen;
        NodeCategory NodeKind;
        public:
        Expression(AstPtr &_LHS, Token_type _op, AstPtr &_RHS,
                     NodeCategory &_nodeKind)
        :LHS(_LHS),Op(_op), RHS(_RHS), NodeKind(_nodeKind)
        {}

        // tokt token() const {return tok;}
        AstPtr getLhs() const {return LHS;}
        Token_type getOp() const {return Op;}
        AstPtr getRhs() const {return RHS;}
        // bool isInGrouped() const{return isInParen;}
        std::string toString() const;
        void accept(AstVisitor& visitor) const;
        NodeCategory nodeCategory()const {return NodeKind;}

    };

    class GroupedExpr:public Ast {
        AstPtr expr;
        public:
        GroupedExpr(AstPtr &_expr)
        :expr(_expr) {}

        AstPtr getExpression() const { return expr; }
        std::string toString() const;
        void accept(AstVisitor& visitor) const;
        NodeCategory nodeCategory() const {return NodeGroupExpr;}
    };

    class FunType: public Ast {
        private:
        tokt tok;
        std::vector<AstPtr> ty;
        AstPtr ret;
       
        public:
        FunType(tokt &tok, std::vector<AstPtr> &_ty, AstPtr &_ret)
        : tok(tok), ty(_ty), ret(_ret) {}

        tokt token() const { return tok; }
        std::vector<AstPtr> type() const { return ty; }
        AstPtr retval() const { return ret; }
        std::string toString() const;
        void accept(AstVisitor& visitor) const;
        NodeCategory nodeCategory() const { return NodeFnTy; }
    };

    class TypeStmt: public Ast {
        private:
        tokt tok;
        tokt Ident;
        AstPtr type;

        public:
        TypeStmt(tokt &_tok, tokt &_Ident ,AstPtr &_type)
        : tok(_tok), Ident(_Ident), type(_type) {}

        tokt token() const { return tok; }
        tokt getIdent() const{ return Ident; }
        AstPtr getType() const{ return type; }
        std::string toString() const;
        void accept(AstVisitor& visitor) const;
        NodeCategory nodeCategory() const { return NodeTypeStm; }
    };

    
    class ArrayTy: public Ast {
        private:
        tokt tok;
        std::vector<AstPtr> size;
        AstPtr type;
        public:
        ArrayTy(tokt &_tok,std::vector<AstPtr> &_size, AstPtr &_type)
        : tok(_tok), size(_size), type(_type) {}

        tokt token() const{ return tok; }
        std::vector<AstPtr> getArraySize() const{return size;}
        AstPtr getArrayType() const{return type;}
        std::string toString() const;
        void accept(AstVisitor& visitor) const;
        NodeCategory nodeCategory() const { return NodeArray; }
    };

  

    class Extern: public Ast {
        private:
        tokt tok;
        std::vector<AstPtr> header;
        public:
        Extern(tokt &_tok, std::vector<AstPtr> &_header)
        :tok(_tok), header(_header){}

        tokt token() const {return tok;}
        std::vector<AstPtr> getHeader() const {return header;}
        // AstPtr getBlock()const {return block;}
        std::string toString() const;
        //void accept(AstVisitor &vistitor) const;
        NodeCategory nodeCategory() const {return NodeExtern;}
    };


    class Import: public Ast{
    private:
        tokt tok;
        AstPtr name;
        AstPtr path;

    public:
        Import(tokt &_tok, AstPtr &_name, AstPtr &_path)
        : tok(_tok), path(_path) {}

        tokt token() const {return tok;}
        AstPtr nameof() const {return name;}
        AstPtr import_path() const {return path;}
        std::string toString() const;
        void accept(AstVisitor& visitor) const;
         NodeCategory nodeCategory() const { return NodeImport; }
    };


    class WhileLoop: public Ast{
    private:
        tokt tok;
        AstPtr expr;
        AstPtr body;

    public:
        WhileLoop(tokt &_tok, AstPtr &_expr, AstPtr &_body )
            :tok(_tok), expr(_expr), body(_body)  {}

        AstPtr expression() const { return expr; }
        AstPtr loopbody() const { return body; }
        tokt token() const {return tok; }
        std::string toString() const;
        void accept(AstVisitor& visitor) const;
        NodeCategory nodeCategory() const { return NodeWhileStm; }

    };


    class ForLoop: public Ast{
    private:
        tokt tok;
        AstPtr var;
        AstPtr cond;
        AstPtr incr;
        AstPtr body;

    public:
        ForLoop(tokt &_tok, AstPtr &_var, AstPtr &_cond, AstPtr &_incr, AstPtr &_body )
            :tok(_tok), var(_var), cond(_cond), incr(_incr), body(_body)  {}

        AstPtr getVar() const { return var; }
        AstPtr getCond() const { return cond; }
        AstPtr getIncr() const { return incr; }
        AstPtr getBody() const { return body; }
        tokt token() const {return tok; }
        std::string toString() const;
        void accept(AstVisitor& visitor) const;
        NodeCategory nodeCategory() const { return NodeForStm; }

    };

    class IfStatement: public Ast{
        private:
        tokt tok;
        AstPtr cond;
        AstPtr ifbody;
        AstPtr elbody;
        
        public:
        IfStatement(tokt &_tok, AstPtr &_cond , AstPtr &_ifbody,
                             AstPtr &_elbody)
        : tok(_tok), cond(_cond), ifbody(_ifbody), 
                        elbody(_elbody) {}
        
        tokt token() const { return tok; }
        AstPtr getCondV() const { return cond; }
        AstPtr getIfBody() const { return ifbody; }
        AstPtr getElBody() const { return  elbody; }
        std::string toString() const;
        void accept(AstVisitor& visitor) const;
        NodeCategory nodeCategory() const { return NodeIfStm; }
    };


    class BranchStmt: public Ast {
        private:
        tokt tok;
        public:
        BranchStmt(tokt &_tok)
        : tok(_tok) {}

        tokt token() const { return tok; }
        std::string toString() const;
        void accept(AstVisitor& visitor) const;
        NodeCategory nodeCategory() const { return NodeBranchStm; }
    };


    class StructStmt: public Ast {
        private:
        tokt tok;
        std::vector<tokt>Temp;
        std::vector<tokt> elmtName;
        std::vector<AstPtr> elmtTy;
        bool isDecl;
        public:
        StructStmt(tokt &_tok, std::vector<tokt> &_Temp, std::vector<tokt> &_memName,
                     std::vector<AstPtr> &_memTy, bool _isDecl)
        : tok(_tok), Temp(_Temp), elmtName(_memName), elmtTy(_memTy), isDecl(_isDecl) {}

        tokt token() const { return tok; }
        // AstPtr name() const { return iden; }
        std::vector<tokt> getTemp() const { return Temp; }
        bool HasTemplate() const { return !Temp.empty(); }
        std::vector<tokt> getIdentList() const { return elmtName; }
        std::vector<AstPtr> getTypeList() const { return elmtTy; }
        bool isDeclaration() const { return isDecl; }
        std::string toString() const;
        void accept(AstVisitor& visitor) const;
        NodeCategory nodeCategory() const { return NodeStructStm; }
    };


    class ListExpr: public Ast {
        private:
        tokt tok;
        std::vector<AstPtr>list;
        public:
        ListExpr(tokt &_tok, std::vector<AstPtr> &_list)
        :tok(_tok), list(_list) {}

        tokt token() const { return tok; }
        std::vector<AstPtr> getList() const { return list; }
        std::string toString() const;
        void accept(AstVisitor& visitor) const;
        NodeCategory nodeCategory() const { return NodeListExpr; }
    };


    class VarStmt: public Ast {
        private:
        tokt tok;
        AstPtr name;
        AstPtr type;
        //TypePtr typeinfo;
        AstPtr val;
        bool HasConst;
        public:
        VarStmt(tokt &_tok, AstPtr &_var, AstPtr &_type, AstPtr &_val, bool &_HasConst)
        : tok(_tok), name(_var), type(_type), val(_val), HasConst(_HasConst) {}

        tokt token() const{ return tok; }
        AstPtr getVarName() const{return name;}
        AstPtr getType() const{return type;}
        void SetType(AstPtr &Type) { type = Type;}
        AstPtr getVal() const{return val;}
        bool HasConstStmt() const { return HasConst; }
        // bool getHasIsStmt()const {return HasIsStmt;}
        // TypePtr getType() const {return typeinfo;}
        // void setType(const TypePtr _typeinfo) {typeinfo = _typeinfo;}
        std::string toString() const;
        void accept(AstVisitor& visitor) const;
        NodeCategory nodeCategory() const { return NodeLetStm; }
    };


    class FunctionDef: public Ast {
        private:
        tokt Name;
        std::vector<tokt> pName;
        std::vector<AstPtr> pTy;
        AstPtr AssociateTo;
        AstPtr retype;
        AstPtr body;
        bool isDecl;
        static bool isTemp;
        static bool isBelong;
        public:
        FunctionDef(tokt &_Name, std::vector<tokt> &_pName,
                    std::vector<AstPtr> &_pTy, AstPtr  _retype, 
                    AstPtr &_AssociateTo, AstPtr &_body, bool _isDecl)
         : Name(_Name), pName(_pName), pTy(_pTy), AssociateTo(_AssociateTo), 
            retype(_retype), body(_body), isDecl(_isDecl){}

        // tokt token() const { return Name; }
        tokt getFuncName() const { return Name; }
        std::vector<tokt> getParameterNames() const { return pName; }
        std::vector<AstPtr> getParameterTy() const { return pTy; }
        AstPtr getResultType() const { return retype; }
        AstPtr getFuncBody() const { return body; }
        bool isDeclaration() const { return isDecl; }
        bool HasStructFunc() const { return AssociateTo != nullptr; }
        AstPtr getStructFunc() const { return AssociateTo; }
        std::string toString() const;
        void accept(AstVisitor& visitor) const;
        NodeCategory nodeCategory() const { return NodeFNStm; }
    };


    class ReturnStmt: public Ast {
        private:
        tokt tok;
        AstPtr val;

        public:
        ReturnStmt(tokt &_tok, AstPtr &_val)
        : tok(_tok), val(_val) {}

        tokt token() const { return tok; }
        AstPtr getRetValue() const { return val; }
        std::string toString() const;
        void accept(AstVisitor& visitor) const;
        NodeCategory nodeCategory() const { return NodeRetStm; }
    };


    class FunctionCall: public Ast {
        private:
        tokt tok;
        AstPtr name;
        std::vector<AstPtr> args;

        public:
        FunctionCall(tokt &_tok, AstPtr &_name, std::vector<AstPtr> &_args)
        : tok(_tok), name(_name), args(_args) {}


        tokt token() const { return tok; }
        AstPtr getCalle() const { return name; }
        std::vector<AstPtr> getArgs() const { return args; }
        std::string toString() const;
        void accept(AstVisitor& visitor) const;
        NodeCategory nodeCategory() const { return NodeCallExpr; }
    };

}
