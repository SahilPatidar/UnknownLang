#include"../../include/codegen/LLVMCodeGen.hpp"
#include"llvm/ADT/StringRef.h"
#include"llvm/IR/ConstantFold.h"
#include"llvm/IR/Constant.h"
#include<iostream>
#include<stdlib.h>

namespace codegen{
    Value *IRCodegenVisitor::visit(const ast::NumericLiteral &AstNode) {
        return ConstantInt::get(Type::getInt32Ty(*TheContext),
                                    stoi(AstNode.toString()));
    }

    Value *IRCodegenVisitor::visit(const ast::FloatLiteral &AstNode) {
        return ConstantFP::get(*TheContext,
                                      APFoat(stoi(AstNode.toString())));
    }

    Value *IRCodegenVisitor::visit(const ast::BoolLiteral &AstNode) {
        switch (AstNode.token().getTokTy())
        {
        case TRUE:
            return ConstantInt::getBool(*TheContext, true);
        case FALSE:
            return ConstantInt::getBool(*TheContext, false);
        default:
            break;
        }
        return nullptr;
    }

    Value *IRCodegenVisitor::visit(const ast::StringLiteral &AstNode) {
        return ConstantDataSequance::get(*TheContext, APFloat(std::to_integer(AstNode.tok.data)));
    }
    
    Value *IRCodegenVisitor::visit(const ast::PreDefineType &AstNode) {
        switch (AstNode.getType().getTokType())
        {
        case I8:
            return Builder->getInt8Ty();
        case I16:
            return Builder->getInt16Ty();
        case I32:
            return Builder->getInt32Ty();
        case I64:
            return Builder->getInt64Ty();
        case I128:
            return Builder->getInt128Ty();
        case UI8:
            return Builder->getInt8Ty();
        case UI16:
            return Builder->getInt8Ty();
        case UI32:
            return Builder->getInt8Ty();
        case UI64:
            return Builder->getInt8Ty();
        default:
            break;
        }
        return nullptr;
    }

    Value *IRCodegenVisitor::visit(const ast::Expression &AstNode) {
        Value *L = AstNode.getLhs()->accept(*this);
        Value *R = AstNode.getRhs()->accept(*this);
        if (!L || !R)
            return nullptr;
        
        switch (AstNode.getOp().getTokType())
        {
        case STAR:
            return Builder->CreateFMul(L,R,"multmp");
            break;
        case PLUS:
            return Builder->CreateFAdd(L,R,"addtmp");
            break;
        case MINUS:
            return Builder->CreateFSub(L,R,"subtmp");
            break;
        case MOD:
            return Builder->CreateFMod(L,R,"modtmp");
            break;
        case AND:
            /* code */
            break;
        

        default:
            break;
        }

    }

}
Value *IRCodegenVisitor::visit(const ast::BlockStmt &AstNode) {
    for(int i = 0, siz = AstNode.getStmts().size(); i < siz; i++) {

    }
}

Value *IRCodegenVisitor::visit(const ast::IfStatement &AstNode) {
    Value *CondV = AstNode.getCondV()->accept(*this);
    if(!CondV) 
        return nullptr;
    
    CondV = Builder->CreateCmp( ,CondV, ConstantInt::get(*TheContext, 0), "ifcond")

}

Function *IRCodegenVisitor::getFunctionType(const ast::FunctionDef &Func) {
    Type *ResultTy = Func.getResultType()->accept(*this);
    std::vector<Type *>Params;
    for(size_t i = 0, siz = Func.getParameter().size(); i < siz; i++) {
        Params.push_back(Func.getParameter()[i]->accept(*this));
    }
    FunctionType* FuncTy = FunctionType::get(ResultTy, Params, false);
    Function *Func = Function::Create(FuncTy, Function::ExternalLinkage,
                                    AstNode.getFuncName()->toString(), TheModule->get());
     // Set names for all arguments.
    unsigned Idx = 0;
    for (auto &Arg : Func->args())
        Arg.setName(AstNode.getParameterNames()[Idx++].toString());


    return Func;
}

Value *IRCodegenVisitor::visit(const ast::FunctionDef &AstNode) {
    FunctionType *FuncTy = getFunctionType(AstNode);
    Function *Func = Function::Create(FuncTy, Function::ExternalLinkage,
                                    AstNode.getFuncName()->toString(), TheModule->get());
     // Set names for all arguments.
    unsigned Idx = 0;
    for (auto &Arg : Func->args())
        Arg.setName(AstNode.getParameterNames()[Idx++].toString());



    BasicBlock *Block = BasicBlock::create(TheContext, "entry", Func, nullptr);

    Function *TheFunction = TheModule->getFunction(Proto->getName());

    if (!TheFunction)
        TheFunction = Proto->codegen();

    if (!TheFunction)
        return nullptr;

    // Create a new basic block to start insertion into.
    BasicBlock *BB = BasicBlock::Create(TheContext, "entry", TheFunction);
    Builder.SetInsertPoint(BB);

    // Record the function arguments in the NamedValues map.
    NamedValues.clear();
    for (auto &Arg : TheFunction->args())
        NamedValues[Arg.getName()] = &Arg;

    if (Value *RetVal = Body->codegen()) {
        // Finish off the function.
        Builder.CreateRet(RetVal);

        // Validate the generated code, checking for consistency.
        verifyFunction(*TheFunction);

    }
}

Value *IRCodegenVisitor::visit(const ast::FunctionCall &AstNode) {
    Function *CalleeF = TheModule->getFunction(AstNode.name());
    if(!CalleeF) {
        //todo
    }
    if(CalleeF->arg_size() != AstNode.Args().size()) {
        //todo
    }
    std::vector<Value *>argv;
    for(unsigned i = 0, size = AstNode.Args().size(); i != size; i++) {
        argv.push_back(AstNode.Args()[i]->accept(*this));
        if(!argv.back()){
            return nullptr;
        }
    }
    return Builder->CreateCall(CalleeF, argv, "calltmp");
}



Value *IRCodegenVisitor::visit(const ast::StructStmt &AstNode) {
    StructType *treeType = StructType::create(*TheContext, StringRef(AstNode.name()->toString()));
    std::ArrayRef<Type *>ele;
    for(size_t i = 0, siz = AstNode.getIdentList().size(); i < siz; i++) {
        ele.push_back(AstNode.getTypeList()[i]->accept(*this));
        if(!ele.back()) {
            return nullptr;
        }
    }
    treeType->setBody(ele);
    return treeType;
}

Value* IRCodegenVisitor::visit(const ast::VarStmt &AstNode) {
    switch(AstNode.getVal()->nodeCategory()) {
        case NodeFNStm:
        {
            Value *FuncTy = AstNode.getVal()->accept(*this);
            if(!Val){
                //todo
            }
            Function *Func = Function::Create(FuncTy, Function::ExternalLinkage,
                                    AstNode.getVarName()->toString(), TheModule->get());

        }
        break;
        case NodeStructStm:
        case NodeStructDef:
        case NodeEnum:
    }
    Type *Type = AstNode.getType()->accept(*this);
    if(!Type){

    }
    Value *Val = AstNode.getVal()->accept(*this);
    if(!Val){

    }
    Value *Var = IRBuilder.CreateAlloca(Type);
    IRBuilder.CreateStore(Val, Var);
    return Var;
}

Value *IRCodegenVisitor::visit(const ast::Array &AstNode) {
    Type *EleType = AstNode.arrType()->accept(*this);
    if(!eleType){
        
    }

    ArrayType *arrType = ArrayType::get(EleType, AstNode.);
    for(size_t i = 0, siz = AstNode.getArraySize(); i < siz; i++) {
    }
    if(!arrType) {

    }
    return arrType;
}

Value *IRCodegenVisitor::visit(const ast::Expression &AstNode) {
    switch (AstNode.nodeCategory())
    {
    case NodeMemExpr:
    
        break;
    
    default:
        break;
    }
}


