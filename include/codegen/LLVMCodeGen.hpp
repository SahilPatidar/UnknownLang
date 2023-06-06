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
#include"../analyzer/AstVisitor.hpp"
#include"../parser/Ast.hpp"


namespace codegen{
using namespace llvm;

class IRCodegenVisitor {
    std::unique_ptr<LLVMContext> TheContext;
    std::unique_ptr<IRBuilder<>> Builder(TheContext);
    std::unique_ptr<Module> TheModule;
    std::map<std::string, Value *> NamedValues;

    FunctionType *getFunctionType(const ast::FunctionDef &Func);
    Value *visit(const ast::BlockStmt& astnode );
    Value *visit(const ast::FunctionDef& astnode );
    Value *visit(const ast::WhileLoop& astnode );
    Value *visit(const ast::ForLoop& astnode );
    Value *visit(const ast::StructStmt& astnode );
    Value *visit(const ast::BranchStmt& astnode );
    Value *visit(const ast::VarStmt& astnode );
    Value *visit(const ast::IfStmt& astnode );
    Value *visit(const ast::ReturnStmt& astnode );
    
    Value *visit(const ast::Expression& astnode );
    Value *visit(const ast::ListExpr& astnode );
    Value *visit(const ast::FunctionCall& astnode );
    Value *visit(const ast::GroupedExpr& astnode );
    // Value *visit(const ast::IndexExpr& astnode );
    Value *visit(const ast::PrefixExpr& astnode );
    Value *visit(const ast::PostfixExpr& astnode );
    
    // Value *visit(const ast::PointerExpr& astnode );
    Value *visit(const ast::Array& astnode ) ;
    Value *visit(const ast::PreDefineType& astnode );
    Value *visit(const ast::Identifier& astnode );
    

    Value *visit(const ast::FloatLiteral& astnode );
    Value *visit(const ast::BoolLiteral& astnode );
    Value *visit(const ast::NumericLiteral& astnode );
    Value *visit(const ast::NullLiteral& astnode );
    Value *visit(const ast::StringLiteral& astnode );

};



}