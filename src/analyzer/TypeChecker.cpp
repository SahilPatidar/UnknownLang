#include"../../include/analyzer/TypeChecker.hpp"
#include"../../include/Error.hpp"
// #include"analyzer/TypeChecker.hpp"

namespace analyzer{


bool TypeChecker::visit(Identifier *AstNode) {
    AstNodeInfo *info;
    if((info = Regmgr->getAstNodeinfo(AstNode->toString())) == NULL){
        ///@todo
    }
    AstNode->setType(info->type);
    AstNode->setDecl(info->Node);
    AstNode->setDecl(info->instTy);
    ResType = info->type;
    return true;
}

bool TypeChecker::visit(UserDefinedTy *AstNode){
    if(!AstNode->getExpr()->accept(*this)){
        return false;
    }
    if(ResType->type() != TypeStruct) {
        ///@todo
    }
    auto StTy = static_cast<StructType*>(ResType);
    if( ( !AstNode->HasGeneric() && StTy->HasTemp() ) || ( AstNode->HasGeneric() && !StTy->HasTemp() )){
        ///@todo
    }
    if(AstNode->getGeneric()->getParam().size() != StTy->getTemp().size()){
        ///@todo
    }
    return 0;
}


bool TypeChecker::visit(PreDefineType  *AstNode) {
    switch (AstNode->getType().getTokType())
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

bool TypeChecker::visit(FnType  *AstNode) {
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

bool TypeChecker::CheckStructDef(StructStmt  *AstNode) {
     AstNode->getName()->accept(*this);
        Type* ty;
        if(ResType->type() != TypeStruct){
        ///todo
        }
        ty = ResType;
        auto StructTy = static_cast<StructType*>(ResType);
        if(StructTy->HasTemp()){
            if(!AstNode->HasGenParam()){
                if(StructTy->getTemp().size() != AstNode->getGenParamList().size()){
                    //todo
                }
            }
        }else if(AstNode->HasGenParam()){
            //todo
        }
        if(AstNode->getIdentList().size() != StructTy->getNameTypeList().size()){
            //todo
        }else{
            for(size_t i = 0, siz = AstNode->getIdentList().size(); i < siz; i++){
                std::string n = AstNode->getIdentList()[i]->toString();
                Type* Sty = StructTy->getType(n);
                if(Sty == nullptr){
                    ///@todo
                }
                if(!AstNode->getTypeList()[i]->accept(*this)){
                    return false;
                }
                if(!ResType){
                    //todo
                }
                if(!CheckImplicitTypeCast(AstNode->getIdentList()[i],ResType, Sty)) {
                    ///@todo
                }
            }
        }

        ResType = ty;
    return true;
}

bool TypeChecker::CheckStructStmt(StructStmt  *AstNode) {
    Regmgr->Push_Stack();
    std::vector<TypeTy*>Temp;
    std::string name = AstNode->getName()->toString();
    if(Regmgr->contains(name)) {
        //todo
    }
    if(AstNode->HasGenParam()){
        std::vector<Lexeme>param = AstNode->getGenParamList();
        for(int i = 0, size = param.size(); i < size; i++){
            TypeTy* t = TypeGenerator::GenerateTypeTy(param[i].getTokTy());
            Temp.push_back(t);
            Regmgr->addInfo(param[i].getStr(), t, nullptr, nullptr);
        }
    }

    if(AstNode->getIdentList().size() != AstNode->getTypeList().size()){
        //todo
    }
    StructType* s = TypeGenerator::GenerateStructType({}, {}, Temp);
    Regmgr->addInfo(name, s , nullptr , AstNode);
    for(size_t i = 0, siz = AstNode->getIdentList().size(); i < siz; i++) {
        if(!AstNode->getTypeList()[i]->accept(*this)){
            return false;
        }
        if(!ResType){
            //todo
        }
        std::string EleName = AstNode->getIdentList()[i]->toString();
        if(s->contains(EleName)){
            ///@todo
            return false;
        }
        s->InsertField(EleName, ResType);
    }
    Regmgr->Pop_Stack();
    AstNode->setType(s);
    Regmgr->addInfo(name, ResType, nullptr, AstNode);
    return true;
}

bool TypeChecker::visit(StructStmt  *AstNode) {
    if(AstNode->nodeCategory() == NodeStructStm){
        if(!CheckStructStmt(AstNode)){
            return false;
        }
    }else {
        if(!CheckStructDef(AstNode)){
            return false;
        }
    }
    
    return true;
}

bool TypeChecker::visit(EnumExpr  *AstNode) {
    std::vector<std::string>uData;
    std::vector<Type*>Val;
    if(Regmgr->contains(AstNode->getName().getStr())) {
        //todo
    }
    for(int i = 0, size = AstNode->getuData().size(); i < size; i++) {
        uData.push_back(AstNode->getuData()[i].getStr());
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
    Regmgr->addInfo(AstNode->getName().getStr(), ResType, nullptr, AstNode);
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


bool TypeChecker::visit(ReturnStmt  *AstNode) {
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



bool TypeChecker::visit(ListExpr  *AstNode) {
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


bool TypeChecker::visit(FunctionDef  *AstNode) {
    std::string n = AstNode->getFuncName().getStr();
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
            std::string ParamName = AstNode->getParameterNames()[i]->toString();
            if(std::find(ElementType.begin(),ElementType.end(),ParamName) == ElementType.end()){
                err::dumperr("redefinition of parameter", AstNode->getParameterNames()[i], AstNode->toString());
                return false;
            }
            ElementType.push_back(ResType);
            Regmgr->addInfo(ParamName, ResType, nullptr, AstNode);
        }
    }
    auto func = TypeGenerator::GenerateFuncType(ElementType, RetType);
    Regmgr->addInfo(AstNode->getFuncName().getStr(), func, nullptr, AstNode);
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


bool TypeChecker::visit(FunctionCall  *AstNode) {
    Regmgr->Push_Stack();
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
    Regmgr->Pop_Stack();
    AstNode->setType(FuncType);
    ResType = FuncType->getRetType();
    return true;
}

bool TypeChecker::visit(VarStmt  *AstNode) {
    if(AstNode->token().getTokTy() == CONST){
        AstNode->getType()->accept(*this);
        Type* type = ResType;
        AstNode->getVal()->accept(*this);
        Type* valtype = ResType;

        if(!CheckImplicitTypeCast(AstNode->getType(), valtype, type)) {
            ///@todo
        }

        Regmgr->addInfo(AstNode->getVarName()->toString(), type, nullptr, AstNode);;
    }else if(AstNode->token().getTokTy() == LET){
        AstNode->getType()->accept(*this);
        Type* type = ResType;
        AstNode->getVal()->accept(*this);
        Type* valtype = ResType;
        
        // if(!type&&!valtype){
        //     ///@todo
        // }
        if(type&&valtype){
            if(!CheckImplicitTypeCast(AstNode->getType(), valtype, type)) {
                ///@todo
            }
        }

        if(!ResType) {
            Regmgr->addInfo(AstNode->getVarName()->toString(), valtype, nullptr, AstNode);
        }else {
            Regmgr->addInfo(AstNode->getVarName()->toString(), type, nullptr, AstNode);;
        }
    }

    return true;
}



bool TypeChecker::visit(IndexExpr *AstNode) {
    AstNode->getVar()->accept(*this);
    if(!ResType || ResType->type() != TypeArray) {
        //todo
    }
    Type* arrtype = ResType;
    auto ArrTy = static_cast<ArrayType*>(ResType);
    if((AstNode->getIndex().size() > ArrTy->getArrSize()) ||
        AstNode->getIndex().size() < ArrTy->getArrSize()) {
        //todo
    }
    for(int i = 0, size = AstNode->getIndex().size(); i < size; i++){
        AstNode->getIndex()[i]->accept(*this);
        if(ResType->type() != TypeInt){
            ///@todo
        }
    }
    AstNode->SetType(arrtype);
    ResType = ArrTy->getArrType();
}

bool TypeChecker::visit(Array  *AstNode) {
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

bool TypeChecker::visit(PrefixExpr  *AstNode) {
    // auto Ptr = static_cast<>
    switch (AstNode->nodeCategory())
    {
    case NodeDeref:
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

bool TypeChecker::visit(GroupedExpr  *AstNode) {
    AstNode->getExpression()->accept(*this);
    if(!ResType) {
        //todo
    }
    return true;
}



bool TypeChecker::visit(Method  *AstNode) {
    // std::string n = AstNode->getName().data; //old
    if(!AstNode->getName()->accept(*this)){
        return false;
    }
    // auto AssociateTy = Regmgr->getType(n); //old
    auto AssociateTy = ResType;
    if(AssociateTy->type() != TypeStruct) {
        ///@todo
    }
    std::map<std::string, Type*>impl;
    Regmgr->Push_Stack();
    for(int i = 0, size = AstNode->getImpl().size(); i < size; i++) {
        auto fn = static_cast<FunctionDef*>(AstNode->getImpl()[i])->getFuncName().getStr();
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


bool TypeChecker::visit(Expression  *AstNode) {
    switch (AstNode->nodeCategory())
    {
    case NodeMemExpr:
    {
        Regmgr->Push_Stack();
        if(!AstNode->getLhs()->accept(*this)){
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
            if((PtrTy->getBaseType()->type() != TypeStruct) && (PtrTy->getDefCount() > 1)){
                //todo
            }
            if(PtrTy->getBaseType()->type() != TypeStruct) {
                //todo
            }
            if(PtrTy->getDefCount() > 1){
                //todo
            }
            ResType = static_cast<PointerType*>(PtrTy->getBaseType());
            }else if(ResType->type() == TypeStruct) {
                if(AstNode->getOp() != DOT) {
                    //todo
                }
        }

        StructType* StTy = static_cast<StructType*>(ResType);
        if(AstNode->getRhs()->nodeCategory() == NodeMemExpr){
            for(int i = 0, size = StTy->getNameList().size(); i < size; i++){
                Regmgr->addInfo(StTy->getNameList()[i], StTy->getTypeList()[i], nullptr, AstNode->getLhs()->getDecl());
            }
            if(!AstNode->getRhs()->accept(*this)){
                return false;
            }
        }else if(AstNode->getRhs()->nodeCategory() == NodeIdent){
            ResType = StTy->getType(AstNode->getRhs()->toString());
            if(!ResType){
                if(!StTy->HasImpl()){
                    ///@todo
                }
                auto StructImpl = StTy->getImpl();
                std::string n = static_cast<FunctionCall*>(AstNode->getRhs())->getCalle()->toString();
                if(StructImpl.find(n) == StructImpl.end()) {
                    ///@todo
                }
                ResType = StructImpl[n];
            }
        }
        Regmgr->Pop_Stack();
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

        if(AstNode->getOp().IsAssignOP() ) {
            if(LHS->isConst()&&(RHS->type() == TypeRef && !RHS->isConst())){
                ///@todo
            }
            if( varTy == UnInitialized ) {
                
            }
        }else if(AstNode->getOp().IsAssnCombinedOP()){
            if( LHS->isConst() && !RHS->isConst() ){
                ///@todo
            }
            if(varTy == UnInitialized) {
                ///@todo
            }
        }
        if(!LHS->IsValidOperation(AstNode->getOp(),ResType)) {
            ///@todo
        }
        ResType = LHS->OperationFinalOutput(AstNode->getOp(), RHS);
        if(LHS->type() != ResType->type()){
            if(!CheckImplicitTypeCast(AstNode->getLhs(), LHS, ResType)) {
                ///@todo
            }
        }
        if(RHS->type() != ResType->type()){
            if(!CheckImplicitTypeCast(AstNode->getRhs(), RHS, ResType)) {
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
    case NodePath:
    {
        std::string n = AstNode->getLhs()->toString();
        module::Module *tmp = mod.getSubMod(n);
        
        if(AstNode->getRhs()->nodeCategory() == NodeIdent){
            
        }else if(!AstNode->getRhs()->accept(*this)){
            return false;
        }
        auto node = AstNode;
        while(true) {
            if(node->getLhs()->nodeCategory() == NodeIdent){
                tmp = mod.getSubMod(n);
            }
            if(node->getRhs()->nodeCategory() == NodeIdent){
                tmp = mod.getSubMod(n);
            }else if(node->getRhs()->nodeCategory() == NodePath){
                node = static_cast<Expression*>(node->getRhs());
            }else if(node->getRhs()->nodeCategory() == NodeBlockStm){
                break;
            }else{
                ///@todo
            }
            if(tmp == NULL){
                ///@todo
            }
        }

        tmp->
        if(node->getRhs()->nodeCategory() == NodeBlockStm){
            auto block = static_cast<BlockStmt*>(node->getRhs());
            for(int i = 0, size = block->getStmts().size(); i < size; i++){
                std::string s = block->getStmts()[i]->toString();
                auto item = tmp->getModItems(s);
                if(!item){
                    ///@todo
                }


            }
        }
    }
    break;
    default:
        break;
    }
    return true;
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

bool TypeChecker::visit(ForLoop  *AstNode) {
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

bool TypeChecker::visit(WhileLoop  *AstNode) {
    AstNode->getCond()->accept(*this);
    if(!ResType || ResType->type() != TypeBoolean){
        ///@todo
    }
    Regmgr->Push_Stack();
    AstNode->getBlock()->accept(*this);
    Regmgr->Pop_Stack();
    return true;
}

bool TypeChecker::visit(IfStmt  *AstNode) {
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


bool TypeChecker::visit(UseStmt *AstNode){
    AstNode->getPath();
}

}

 