#pragma once
#include<iostream>
#include<llvm/IR/IRBuilder.h>
#include<llvm/IR/DIBuilder.h>
#include<llvm/IR/Module.h>
#include<llvm/IR/LLVMContext.h>
#include<llvm/IR/Type.h>
#include<llvm/IR/DerivedTypes.h>
#include<llvm/ADT/ArrayRef.h>
#include<llvm/ADT/StringRef.h>
#include<llvm/IR/BasicBlock.h>
#include"analyzer/AstVisitor.hpp"


namespace codegen{
using namespace ast;
using namespace llvm;

class IRCodegenVisitor: public AstVisitor {
    std::unique_ptr<LLVMContext> TheContext;
    std::unique_ptr<IRBuilder<>> Builder(TheContext);
    std::unique_ptr<Module> TheModule;
    std::map<std::string, Value *> NamedValues;

    FunctionType *getFunctionType(const FunctionDef &Func);
    Value *visit(const Program& astnode );
    Value *visit(const BlockStatement& astnode );
    Value *visit(const FunctionDef& astnode );
    Value *visit(const WhileLoop& astnode );
    Value *visit(const ForInLoop& astnode );
    Value *visit(const StructState& astnode );
    Value *visit(const BranchState& astnode );
    Value *visit(const Tuple& astnode );
    Value *visit(const VarState& astnode );
    Value *visit(const IfStatement& astnode );
    Value *visit(const ReturnState& astnode );
    
    Value *visit(const BineryExper& astnode );
    Value *visit(const AssignmentExpr& astnode );
    Value *visit(const ListExpr& astnode );
    Value *visit(const FunctionCall& astnode );
    Value *visit(const MemberExpr& astnode );
    Value *visit(const IndexExpr& astnode );
    Value *visit(const PrefixExper& astnode );
    Value *visit(const Parameter& astnode );
    
    Value *visit(const PointerExpr& astnode );
    Value *visit(const ArrayType& astnode ) ;
    Value *visit(const PreDefineType& astnode );
    Value *visit(const Identifier& astnode );
    

    Value *visit(const FloatLiteral& astnode );
    Value *visit(const BoolLiteral& astnode );
    Value *visit(const NumericLiteral& astnode );
    Value *visit(const NullLiteral& astnode );
    Value *visit(const StringLiteral& astnode );

};



}