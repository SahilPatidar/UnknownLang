#include"../../include/analyzer/TypeChecker.hpp"
#include"../../include/Error.hpp"
// #include"analyzer/TypeChecker.hpp"

namespace analyzer{

bool TypeChecker::visit(PreDefineType *AstNode) {
    switch (AstNode->getType())
    {
    case I8:
        ResType = TypeGenerator::GenerateIntType(8,true);
        break;
    case I16:
        ResType = TypeGenerator::GenerateIntType(16,true);
        break;
    case I32:
        ResType = TypeGenerator::GenerateIntType(32,true);
        break;
    case I64:
        ResType = TypeGenerator::GenerateIntType(64,true);
        break;
    case I128:
        ResType = TypeGenerator::GenerateIntType(128,true);
        break;
    case F32:
        ResType = TypeGenerator::GenerateFltType(32);
        break;
    case F64:
        ResType = TypeGenerator::GenerateFltType(64);
        break;
    case UI8:
        ResType = TypeGenerator::GenerateIntType(8,false);
        break;
    case UI16:
        ResType = TypeGenerator::GenerateIntType(16,false);
        break;
    case UI32:
        ResType = TypeGenerator::GenerateIntType(32,false);
        break;
    case UI64:
        ResType = TypeGenerator::GenerateIntType(64,false);
        break;
    case BOOL:
        ResType = TypeGenerator::GenerateBoolType();
        break;
    default:
        break;
    }
    AstNode->setType(ResType);
    return true;
}

bool TypeChecker::visit(FnType *AstNode) {
    std::vector<Type*>pTy;
    if(!AstNode->getParamType().empty()) {
        for(int i = 0, size = AstNode->getParamType().size(); i < size; i++) {
            AstNode->getParamType()[i]->accept(*this);
            if(!ResType) {
                ///@todo
            }
            pTy.push_back(ResType);
        }
    }
    Type* ret;
    if(AstNode->getRetType()){
        AstNode->getRetType()->accept(*this);
        if(!ResType){
            ///@todo
        }
        ret = ResType;
    }
    ResType = TypeGenerator::GenerateFuncType(pTy, ret);
    AstNode->setType(ResType);
    return true;
}


bool TypeChecker::visit(StructStmt *AstNode) {
    std::vector<tokt>Temp;
    std::vector<std::string>ElementNameList;
    std::vector<Type*>ElementTypeList;
    if(Regmgr->contains(AstNode->getName().data)) {
        //todo
    }
    if(AstNode->HasTemplate()){
        Temp = AstNode->getTemp();
    }

    if(AstNode->getIdentList().size() != AstNode->getTypeList().size()){
        //todo
    }
    for(size_t i = 0, siz = AstNode->getIdentList().size(); i < siz; i++) {
        AstNode->getTypeList()[i]->accept(*this);
        if(!ResType){
            //todo
        }
        std::string EleName = AstNode->getIdentList()[i].data;
        if(std::find(ElementNameList.begin(), ElementNameList.end(), EleName) == ElementNameList.end()){
            //todo
        }
        ElementNameList.push_back(EleName);
        ElementTypeList.push_back(ResType);
    }
    ResType = TypeGenerator::GenerateStructType(ElementNameList, ElementTypeList, Temp);
    Regmgr->addInfo(AstNode->getName().data, ResType, nullptr, AstNode);
    return true;
}

bool TypeChecker::visit(EnumExpr *AstNode) {
    std::vector<std::string>uData;
    std::vector<Type*>Val;
    if(Regmgr->contains(AstNode->getName().data)) {
        //todo
    }
    for(int i = 0, size = AstNode->getuData().size(); i < size; i++) {
        uData.push_back(AstNode->getuData()[i].data);
        if(AstNode->getValue()[i]) {
            AstNode->getValue()[i]->accept(*this);
            if(!ResType) {
                //todo
            }
            if(ResType->type() != TypeInt) {
                //todo
            }
            Val.push_back(ResType);
        }else{
            Val.push_back(TypeGenerator::GenerateIntType(32,true));
        }
    }
    ResType = TypeGenerator::GenerateEnumType(uData,Val);
    Regmgr->addInfo(AstNode->getName().data, ResType, nullptr, AstNode);
    return true;
}

bool TypeChecker::CheckImplicitTypeCast(Ast *Node, Type *&from, Type *&to) {
    if(from->IsCastRequire(to)){
       if(!from->IsCastable(to)) {
            ///@todo
       }
       Node->setType(from);
       Node->setCastTo(to);
    }
    return true;
}

bool TypeChecker::visit(ReturnStmt *AstNode) {
    auto RetType = Regmgr->getFuncBack()->getRetType();
    if(!RetType&&!AstNode->getRetValue()){
        return true;
    }else if((!RetType&&AstNode->getRetValue())||(RetType&&!AstNode->getRetValue())){
        //todo
    }
    if(AstNode->getRetValue()->accept(*this)){
        return false;
    }
    ResType = RetType;
    if(!CheckImplicitTypeCast(AstNode->getRetValue(), ResType, RetType)){
       return false;
    }
    return true;
}



bool TypeChecker::visit(ListExpr *AstNode) {
    if(AstNode->getList().empty()){
        //todo
    }
    AstNode->getList()[0]->accept(*this);
    int ArrDim = 1;
    Type* PrevType = ResType;
    for(auto &val: AstNode->getList()){
        val->accept(*this);
        if(ResType->type() != PrevType->type()){
            ///@todo
        }
    }
    if(ResType->type() == TypeArray) {
        auto ArrTy = static_cast<ArrayType*>(ResType);
        ArrDim += ArrTy->getArrSize();
        ResType = ArrTy->getArrType();
    }
    ResType = TypeGenerator::GenerateArrType(ResType, ArrDim);
    return true;
}


bool TypeChecker::visit(FunctionDef *AstNode) {
    std::string n = AstNode->getFuncName().data;
    FunctionType* fnTy;
    if(Regmgr->contains(n)){
            ///@todo
    }
    Regmgr->Push_Stack();
    Type *RetType;
    if(AstNode->getResultType()) {
        if(!AstNode->getResultType()->accept(*this)){
            ///@todo
        }
        RetType = ResType;
    }
    // std::map<std::string, Type*>ElementType;
    std::vector<Type*>ElementType;
    if(!AstNode->getParameterNames().empty()){
        for(size_t i = 0, size = AstNode->getParameterNames().size(); i < size; i++) {
            AstNode->getParameterTy()[i]->accept(*this);
            if(!ResType){
                //todo
            }
            std::string ParamName = AstNode->getParameterNames()[i].data;
            if(std::find(ElementType.begin(),ElementType.end(),ParamName) == ElementType.end()){
                err::dumperr("redefinition of parameter", AstNode->getParameterNames()[i], AstNode->toString());
                return false;
            }
            ElementType.push_back(ResType);
            Regmgr->addInfo(ParamName, ResType, nullptr, AstNode);
        }
    }
    auto func = TypeGenerator::GenerateFuncType(ElementType, RetType);
    Regmgr->addInfo(AstNode->getFuncName().data, func, nullptr, AstNode);
    Regmgr->Push_Func(func);
    if(!AstNode->getFuncBlock()->accept(*this)){
        ///@todo
    }
    AstNode->setType(func);
    Regmgr->Pop_Func();
    Regmgr->Pop_Stack();
    ResType = func;
    return true;
}


bool TypeChecker::visit(FunctionCall *AstNode) {
    if(!AstNode->getCalle()->accept(*this)) {
        ///@todo
    }
    if (!ResType) {
        /* code */
    }
    auto FuncType = static_cast<ast::FunctionType*>(ResType);
    if(AstNode->getArgs().size() != FuncType->getParamTypes().size()) {
        //todo
    }
    if(!AstNode->getArgs().empty()){
        for(size_t i = 0, siz = AstNode->getArgs().size(); i < siz; i++) {
            if(!AstNode->getArgs()[i]->accept(*this)){
                return false;
            }
            if(!ResType){
                ///@todo
            }
            if(!CheckImplicitTypeCast(AstNode->getArgs()[i], ResType, FuncType->getParamTypes()[i])) {
                err::dumperr("no matching function for call to", AstNode->getCalle()->token(), AstNode->toString());
                return false;
            }
        }
    }
    AstNode->setType(FuncType);
    ResType = FuncType->getRetType();
    return true;
}

bool TypeChecker::visit(VarStmt *AstNode) {
    AstNode->getType()->accept(*this);
    Type* type = ResType;
    AstNode->getVal()->accept(*this);
    Type* valtype = ResType;
     
    if(!type&&!valtype){
        ///@todo
    }
    if(!CheckImplicitTypeCast(AstNode->getType(), valtype, type)) {
        ///@todo
    }

    if(!ResType) {
        Regmgr->addInfo(AstNode->getVarName()->toString(), valtype, nullptr, AstNode);
    }else {
        Regmgr->addInfo(AstNode->getVarName()->toString(), type, nullptr, AstNode);;
    }

    return true;
}



bool TypeChecker::visit( PostfixExpr *AstNode) {
    switch (AstNode->nodeCategory())
    {   
    case NodeStructDef:
    {
        AstNode->getVar()->accept(*this);
        Type* ty;
        if(ResType->type() != TypeStruct){
        ///todo
        }
        ty = ResType;
        auto StructTy = static_cast<StructType*>(ResType);
        if(StructTy->HasTemp()){
            if(!AstNode->HasTemp()){
                //todo
            }
            if(StructTy->getTemp().size() != AstNode->getTemp().size()){
                //todo
            }
        }else if(AstNode->HasTemp()){
            //todo
        }
        if(AstNode->getExpr().size() != StructTy->getNameTypeList().size()){
            //todo
        }else{
            for(size_t i = 0, siz = AstNode->getExpr().size(); i < siz; i++){
                AstNode->getExpr()[i]->accept(*this);
                if(!ResType){
                    //todo
                }
                if(!CheckImplicitTypeCast(AstNode->getExpr()[i],ResType, StructTy->getTypeList()[i])) {
                    ///@todo
                }
            }
        }

        ResType = ty;
    }
        break;
    case NodeIndexExpr:
    {
        AstNode->getVar()->accept(*this);
        if(!ResType || ResType->type() != TypeArray) {
            //todo
        }
        Type* arrtype = ResType;
        auto ArrTy = static_cast<ArrayType*>(ResType);
        if((AstNode->getExpr().size() > ArrTy->getArrSize()) ||
            AstNode->getExpr().size() < ArrTy->getArrSize()) {
            //todo
        }
        for(int i = 0, size = AstNode->getExpr().size(); i < size; i++){
            AstNode->getExpr()[i]->accept(*this);
            if(ResType->type() != TypeInt){
                ///@todo
            }
        }
        AstNode->SetType(arrtype);
        ResType = ArrTy->getArrType();
    }
        
        break;
    default:
        break;
    }
}

bool TypeChecker::visit(Array *AstNode) {
    AstNode->getArrayType()->accept(*this);

    if(!ResType){
        //todo
    }
    Type* ArrTy = ResType;
    // std::vector<Type*>SizeType;
    int dim = AstNode->getArraySize().size();
    for(int i = 0, size = AstNode->getArraySize().size(); i < size; i++) {
        AstNode->getArraySize()[i]->accept(*this);
        if(!ResType){
            //todo
        }

        if(!(ResType->isConst())||(ResType->type() != TypeInt)){
            //todo
        }
        // SizeType.push_back(ResType);
    }
    ResType = TypeGenerator::GenerateArrType(ArrTy,dim);
    AstNode->setType(ResType);
    return true;
}

bool TypeChecker::visit(PrefixExpr *AstNode) {
    // auto Ptr = static_cast<>
    switch (AstNode->nodeCategory())
    {
    case NodePtr:
    {
        if(AstNode->IsType()) {
        AstNode->getBase()->accept(*this);
        if(!ResType) {
            //todo
        }

        ResType = TypeGenerator::GeneratePtrType(AstNode->getDefCount(), ResType); 
        }else {
            AstNode->getBase()->accept(*this);
            if(!ResType) {
                //todo
            }
            if(ResType->type() == TypePointer){
                auto PtrTy = static_cast<PointerType*>(ResType);
                if(PtrTy->getDefCount() < AstNode->getDefCount()){
                    //todo
                }else if(PtrTy->getDefCount() > AstNode->getDefCount()) {
                    ResType = TypeGenerator::GeneratePtrType(PtrTy->getDefCount() - AstNode->getDefCount(), PtrTy->getBaseType());
                }else{
                    ResType = PtrTy->getBaseType();
                }
            }
        }
    }
        break;
    case NodeRef:
    {
        if(!AstNode->getBase()->accept(*this)){
            ///@todo
        }
        ResType->UnaryOpMatch(AstNode->getOp());
        ResType = TypeGenerator::GenerateRefType(ResType);
    }
    case NodePrefix:
    default:
        break;
    }
    
    return true;
}

bool TypeChecker::visit(GroupedExpr *AstNode) {
    AstNode->getExpression()->accept(*this);
    if(!ResType) {
        //todo
    }
    return true;
}



bool TypeChecker::visit(Method *AstNode) {
    std::string n = AstNode->getName().data;
    auto AssociateTy = Regmgr->getType(n);
    if(AssociateTy->type() != TypeStruct) {
        ///@todo
    }
    std::map<std::string, Type*>impl;
    Regmgr->Push_Stack();
    for(int i = 0, size = AstNode->getImpl().size(); i < size; i++) {
        auto fn = static_cast<FunctionDef*>(AstNode->getImpl()[i])->getFuncName().data;
        AstNode->getImpl()[i]->accept(*this);
        if(!ResType) {
            ///@todo
        }
        if(ResType->type() != TypeFunction){
            ///@todo
        }
        impl.insert({fn,ResType});
    }

    Regmgr->Pop_Stack();
    static_cast<StructType*>(AssociateTy)->setImpl(impl);
    return true;
}

bool TypeChecker::CheckLHS(Ast* &Node, bool inField, StructType *type) {
    if(inField) {
        if(Node->nodeCategory() != NodeIdent && Node->nodeCategory() != NodeArray) {
            //todo
        }
        if(Node->nodeCategory() == NodeIdent) {
            std::string n = Node->toString();
            if(n == ""){
                //todo
            }
            if(type->find(n) == -1) {
                //todo
            }
            ResType = type->getTypeAt(n); 
        }else if(Node->nodeCategory() == NodeIndexExpr) {
            auto Operand = static_cast<PostfixExpr*>(Node);
            std::string Ident = Operand->getVar()->toString();
            ResType = type->getTypeAt(Ident);
            if(!ResType||ResType->type() != TypeArray){
                //todo
            }
        }
    }else
        Node->accept(*this);
   if(!ResType){
    //todo
   }
   if(ResType->type() != TypeStruct && ResType->type() != TypeArray && ResType->type() != TypePointer) {
     //todo
   }else if(ResType->type() == TypeArray) {
        auto Arrty = static_cast<ArrayType*>(ResType);
        if(Arrty->getArrType()->type() != TypeStruct && Arrty->getArrType()->type() != TypePointer) {
            //todo
        }else if(Arrty->getArrType()->type() != TypePointer) {
            auto PtrTy = static_cast<PointerType*>(Arrty->getArrType());
            if(PtrTy->getBaseType()->type() != TypeStruct) {
                //todo
            }
        }
        ResType = Arrty->getArrType();
   }else if(ResType->type() != TypePointer) {
        auto PtrTy = static_cast<PointerType*>(ResType);
        if(PtrTy->getBaseType()->type() != TypeStruct) {
            //todo
        }
   }
   return true;
}

bool TypeChecker::CheckRHSInField(Ast* &Node, StructType *&StructTy) {
    if(Node->nodeCategory() == NodeCallExpr) {
        if(!StructTy->HasImpl()){
            ///@todo
        }
        auto StructImpl = StructTy->getImpl();
        std::string n = static_cast<FunctionCall*>(Node)->getCalle()->toString();
        if(StructImpl.find(n) == StructImpl.end()) {
            ///@todo
        }
        Regmgr->Push_Stack();
        Regmgr->addInfo(n,StructImpl[n],nullptr,Node);
        Node->accept(*this);
        Regmgr->Pop_Stack();
    }else if(Node->nodeCategory() == NodeIndexExpr) {
        auto Operand = static_cast<PostfixExpr*>(Node);
        std::string Ident = Operand->getVar()->toString();
        ResType = StructTy->getTypeAt(Ident);
        if(!ResType||ResType->type() != TypeArray){
            //todo
        }
    }else if(Node->nodeCategory() == NodeIdent) {
        auto Operand = static_cast<Identifier*>(Node);
        std::string n = Operand->toString();
        ResType = StructTy->getTypeAt(n);
        if(!ResType){
            //todo
        }
    }else if(Node->nodeCategory() == NodeMemExpr){
        auto Expr = static_cast<Expression*>(Node);
        if(!Expr){
            //todo
        }
        auto lhs = Expr->getLhs();
        if(!CheckLHS(lhs, true, StructTy)) {
            //todo
        }
        if(!ResType || (ResType->type() != TypeStruct && ResType->type() != TypePointer)) {
            //todo
        }

        if(ResType->type() == TypePointer) {
            if(Expr->getOp() != ARROW) {
                //todo
            }
            auto PtrTy = static_cast<PointerType*>(ResType);
            if(PtrTy->getBaseType()->type() != TypeStruct) {
                //todo
            }
            if(PtrTy->getDefCount() > 1){
                //todo
            }
        }

        auto rhs = Expr->getRhs();
        auto type = static_cast<StructType*>(ResType);
        if(!CheckRHSInField(rhs, type)) {
            //todo
        }
    }else{
        return false;
    }
   return true;
}

bool TypeChecker::visit(Expression *AstNode) {
    switch (AstNode->nodeCategory())
    {
    case NodeMemExpr:
    {
        auto lhs = AstNode->getLhs();
        if(!CheckLHS(lhs, false, nullptr)) {
            //todo
            return false;
        }

        if(!ResType || (ResType->type() != TypeStruct && ResType->type() != TypePointer)) {
            //todo
        }

        if(ResType->type() == TypePointer) {
            if(AstNode->getOp() != ARROW) {
                //todo
            }
            auto PtrTy = static_cast<PointerType*>(ResType);
            if(PtrTy->getBaseType()->type() != TypeStruct) {
                //todo
            }
            if(PtrTy->getDefCount() > 1){
                //todo
            }
        }

        auto StructTy = static_cast<StructType*>(ResType);

        auto RHS = AstNode->getRhs();
        if(!CheckRHSInField(RHS, StructTy)) {
                //todo
        }
    }
        break;
    case NodeBinaryExpr:
    {
        if(!AstNode->getLhs()->accept(*this)){
            ///@todo
        }
        if(!ResType) {
            //todo
        }
        Type* LHS = ResType;
        if(!AstNode->getRhs()->accept(*this)){
            ///@todo
        }
        if(!ResType) {
            //todo
        }
        Type* RHS = ResType;
        if(LHS->IsValidOperation(AstNode->getOp(),ResType)) {

        }
        ResType = LHS->OperationFinalOutput(AstNode->getOp(), RHS);
        if(LHS->type() == ResType->type()){
            if(!CheckImplicitTypeCast(AstNode->getLhs(), LHS, ResType)) {
                ///@todo
            }
        }else if(RHS->type() == ResType->type()){
            if(!CheckImplicitTypeCast(AstNode->getRhs(), RHS, ResType)) {
                ///@todo
            }
        }else {
            if(!CheckImplicitTypeCast(AstNode->getRhs(), RHS, ResType)&&!CheckImplicitTypeCast(AstNode->getLhs(), LHS, ResType)) {
                ///@todo
            }
        }
        AstNode->setType(ResType);
    }
        break;
    case NodeAsExpr:
    {
        AstNode->getLhs()->accept(*this);
        if(!ResType) {
            //todo
        }
        Type* LHSTy = ResType;
        AstNode->getRhs()->accept(*this);
        if(!ResType) {
            //todo
        }

        if(!LHSTy->IsCastable(ResType)) {
            //todo
        }
        AstNode->setType(ResType);
    }
        break;

    default:
        break;
    }
}

bool TypeChecker::visit(BlockStmt *AstNode) {
    if(AstNode->getStmts().empty()) {
        ///@warning unused loop block
    }

    for(int i = 0, s = AstNode->getStmts().size(); i < s; i++) {
        if(!AstNode->getStmts()[i]->accept(*this)){
            ///@todo
        }
        if(!ResType){
            ///@todo
        }
    }
    return true;
}

bool TypeChecker::visit(ForLoop *AstNode) {
    Regmgr->Push_Stack();
    AstNode->getVar()->accept(*this);
    if(!ResType){
        ///@todo
    }
    AstNode->getCond()->accept(*this);
    if(!ResType || ResType->type() != TypeBoolean){
        ///@todo
    }
    AstNode->getBlock()->accept(*this);
    Regmgr->Pop_Stack();
    return true;
}

bool TypeChecker::visit(WhileLoop *AstNode) {
    AstNode->getCond()->accept(*this);
    if(!ResType || ResType->type() != TypeBoolean){
        ///@todo
    }
    Regmgr->Push_Stack();
    AstNode->getBlock()->accept(*this);
    Regmgr->Pop_Stack();
    return true;
}

bool TypeChecker::visit(IfStmt *AstNode) {
    AstNode->getCondV()->accept(*this);
    if(!ResType || ResType->type() != TypeBoolean){
        ///@todo
    }
    Regmgr->Push_Stack();
    AstNode->getIfBlock()->accept(*this);
    AstNode->getElBlock()->accept(*this);
    Regmgr->Pop_Stack();
    return true;
}

}

 