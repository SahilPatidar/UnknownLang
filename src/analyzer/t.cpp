#include"analyzer/TypeChecker.hpp"

bool TypeChecker::visit(const StructStmt &AstNode) {
    std::vector<tokt>Temp;
    std::vector<std::string>ElementNameList;
    std::vector<TypePtr>ElementTypeList;

    if(AstNode.HasTemplate()){
        Temp = AstNode.getTemp();
    }
    if(!AstNode.isDeclaration()){

    }
    if(AstNode.getIdentList().size() != AstNode.getTypeList().size()){
        //todo
    }
    for(size_t i = 0, siz = AstNode.getIdentList().size(); i < siz; i++) {
        AstNode.getTypeList()[i]->accept(*this);
        if(!ResType){
            //todo
        }
        std::string EleName = AstNode.getIdentList()[i].data;
        if(ElementNameList.find(EleName) == ElementNameList.end()){
            //todo
        }
        ElementNameList.push_back(EleName);
        ElementTypeList.push_back(ResType);
    }
    ResType = TypeGenerator::StructTyGen(ElementNameList, ElementTypeList,Temp);
    return true;
}

bool TypeChecker::visit(const EnumExpr &AstNode) {
    std::vector<std::string>uData;
    std::vector<TypePtr>Val;
    for(int i = 0, size = AstNode.getuData().size(); i < size; i++) {
        uData.push_back(Astnode.getuData()[i].data);
        if(AstNode.getValue()[i]) {
            AstNode.getValue()[i]->accept(*this);
            if(!ResType) {
                //todo
            }
            Val.push_back(ResType);
        }
    }
    ResType = TypeGenerator::EnumTyGen(uData,Val);
    return true;
}


bool TypeChecker::visit(const ReturnStmt& AstNode) {
    if(!retype&&!AstNode.getRetValue()){
        return true;
    }else if(!retype&&AstNode.getRetValue()||retype&&!AstNode.getRetValue()){
        //todo
    }
    m_type = retype;
    AstNode.getRetValue()->accept(*this);
    if(!isValid(m_type, retype)){
        //todo
    }
    return true;
}

static int getListDim() {

}

bool TypeChecker::visit(const ListExpr& AstNode) {
    if(AstNode.getList().empty()){
        //todo
    }
    AstNode.getList()[0]->accept(*this);
    TypePtr PrevType = ResType;
    for(auto &val: AstNode.getList()){
        val->accept(*this);
        if(m_type->type() != PrevType->type()){
            //todo
        }
    }
    m_type = TypeGenerator::ArrayTyGen(m_type, AstNode.listof().size());
    return true;
}


bool TypeChecker::visit(const FunctionDef& AstNode) {
    AstNode.getResultType()->accept(*this);
    TypePtr RetType = ResType;
    // std::map<std::string, TypePtr>ElementType;
    std::vector<TypePtr>ElementType;
    if(!AstNode.getParameterNames().empty()){
        for(size_t i = 0, siz = AstNode.getParameterNames().size(); i < size; i++) {
            AstNode.getParameterTy()[i]->accept(*this);
            if(!ResType){
                //todo
            }
            std::string ParamName = AstNode.getParameterNames()[i].data;
            if(ElementType.contain(ParamName)){
                //todo
            }
            // ElementType.insert({ParamName,ResType});
            ElementType.push_back(ResType);
            Table->insert({ParamName, ResType});
        }
    }
    ResType = TypeGenerator::FuncTyGen(ElementType, RetType);
    return true;
}

bool TypeChecker::visit(const FunctionCall& AstNode) {
    AstNode.getCalle()->accept(*this);
    if (!ResType) {
        /* code */
    }
    auto FuncType = std::dynamic_pointer_cast<ast::FunctionType>(ResType);
    if(AstNode.getArgs().size() != FuncType->getParamTypes()) {
        //todo
    }
    if(!AstNode.getArgs().empty()){
        for(size_t i = 0, siz = AstNode.getArgs().size(); i < siz; i++) {
            AstNode.getArgs()[i]->accept(*this);
            if(!ResType){
                //todo
            }
            if(ResType->type() != FuncType->getParamTypes()[i]->type()) {
                //todo
            }
        }
    }
    TypePtr RetType = FuncType->getRetType();
    return true;
}

bool TypeChecker::visit(const VarStmt &AstNode) {
    AstNode.getType()->accept(*this);
    std::string VarName = identifier(AstNode.getVarName());
    TypePtr Type;
    if(!ResType) {
        AstNode.getVal()->accept(*this);
        if(!ResType) {
            //todo
        }
        Table->insert(VarName, ResType);
    }else {
        Type = ResType;
        AstNode.getVal()->accept(*this);
        if(!ResType) {
            //todo
        }else {
            if(Type->type() != ResType->type()){
                //todo
            }
            Table->insert(VarName, Type);
        }
        
    }

    return true;
}

bool TypeChecker::visit(const PostfixExpr &AstNode) {
    switch (AstNode.nodeCategory())
    {   
    case NodeStructDef:
    {
        AstNode.getVar()->accept(*this);
        TypePtr ty;
        if(ResType->type() != TypeStruct){
        ///todo
        }else{
            ty = ResType;
            auto StructTy = std::dynamic_pointer_cast<StructType>(ResType);
            if(StructTy.HasTemp()){
                if(!AstNode.HasTemp()){
                    //todo
                }
                if(StructTy.getTemp().size() != AstNode.getTemp().size()){
                    //todo
                }
            }else if(AstNode.HasTemp()){
                //todo
            }
            if(AstNode.getExpr().size() != StructTy->getNameList().size()){
                //todo
            }else{
                for(size_t i = 0, siz = AstNode.getExpr().size(); i < siz; i++){
                    AstNode.getExpr()[i]->accept(*this);
                    if(!ResType){
                        //todo
                    }
                    if(StructTy.getTypeList()[i]->type() != ResType->type()) {
                        //todo
                    }
                }
            }
        }
        ResType = ty;
    }
        break;
    case NodeIndexExpr:
    {
        AstNode.getVar()->accept(*this);
        if(!ResType || Restype->type() != TypeArray) {
            //todo
        }
        auto ArrTy = std::dynamic_pointer_cast<ArrayType>(ResType);
        if(AstNode.getExpr() > ArrTy->getArrSize() || AstNode.getExpr() < ArrTy->getArrSize()) {
            //todo
        }
        ResType = ResType.getArrType();
    }
        
        break;
    default:
        break;
    }
}

bool TypeChecker::visit(const ArrayTy &AstNode) {
    AstNode.getArrayType()->accept(*this);
    if(!ResType){
        //todo
    }
    
    
    return true;
}

bool TypeChecker::visit(const PrefixExpr &AstNode) {
    if(AstNode.isType()) {
        AstNode.getBase()->accept(*this);
        if(!ResType) {
            //todo
        }
        if(ResType->type() == TypePtr){
            ResType = TypeGenerator::PointerTyGen(AstNode.getNumofPtr(), ResType); 
        }
    }else {
        AstNode.getBase()->accept(*this);
        if(!ResType) {
            //todo
        }
        if(ResType->type() == TypePtr){
            auto PtrTy = std::dynamic_pointer_cast<PointerType>(ResType);
            if(PtrTy->getNumofPtr() < AstNode.getNumofPtr()){
                //todo
            }else if(PtrTy->getDefCount() > AstNode.getNumofPtr()) {
                ResType = TypeGenerator::PointerTyGen(PtrTy->getDefCount() - AstNode.getNumofPtr(), PtrTy->getBaseType());
            }else{
                ResType = PtrTy->getBaseType();
            }
        }
    }
    return true;
}

bool TypeChecker::visit(const GroupedExpr &AstNode) {
    AstNode.getExpression()->accept(*this);
    if(!ResType) {
        //todo
    }
    return true;
}

static TypePtr CheckLHS(const Expression &AstNode) {
   AstNode.getLhs()->accept(*this);
   if(!ResType){
    //todo
   }
   if(ResType->type() != TypeStruct) {
     //todo
   } else if(ResType->type() == TypeArray) {
        auto Arrayty = std::dynamic_pointer_cast<ArrayType>(ResType);
        if(Arrayty.getBase()->type() != TypePointer) {
            auto Ptrty = std::dynamic_pointer_cast<PointerType>(Arrayty.getBase());
            if(Ptrty->getBaseType()->type() != TypeStruct) {
                //todo
            }
        }else if(Arrayty.getBase()->type() != TypeStruct) {
            //todo
        }
   }else if(ResType->type() == Pointer) {
        auto Ptrty = std::dynamic_pointer_cast<PointerType>(Arrayty.getBase());
        if(Ptrty->getBaseType()->type() != TypeStruct) {
            //todo
        }
   }else if(ResType->type() != TypeEnum) {
        //todo
   }else if(ResType->type() != TypeImport) {
        //todo
   }
   return true;
}

static bool CheckLHSInField(AstPtr &Node, StructType *&StructTy) {
    if(Node->nodeCategory() == NodeIndexExpr) {
        auto Operand = std::dynamic_pointer_cast<PostfixExpr>(Node);
        std::string Ident = Operand->getVar().toString();
        ResType = StructType->getTypeAt(Ident);
        if(!ResType||ResType->type() != TypeArray){
            //todo
        }
    }else if(Node->nodeCategory() == NodeIdent) {
        auto Operand = std::dynamic_pointer_cast<Identifier>(Node);
        std::string n = Operand.toString();
        ResType = StructType->getTypeAt(n);
        if(!ResType){
            //todo
        }
    }else if(Node->nodeCategory() == NodeMemExpr){
        auto Expr = std::dynamic_pointer_cast<Expression>(Node);
        if(!CheckLHSInField(Expr->getLhs(), StructTy)) {
            //todo
        }
        if(ResType->type() != TypeStruct) {
            //todo
        }
        if(!CheckLHSInField(Expr->getRhs(), std::dynamic_pointer_cast<StructType>(ResType))) {
            //todo
        }
    }else{
        return false;
    }
   return true;
}

bool TypeChecker::visit(const Expression &AstNode) {
    switch (AstNode.nodeCategory())
    {
    case NodeMemExpr:
    {
        AstNode.getLhs()->accept(*this);
        if(!ResType || ResType->type() != TypeStruct || 
                        ResType->type() != TypeImport || 
                        ResType->type() != TypePointer || 
                            ResType->type() != TypeEnum) {
            //todo
        }
        auto LHS = CheckLHS(AstNode);
        if(!LHS) {
            //todo
            return false;
        }

        auto StructType = std::dynamic_pointer_cast<StructType>(LHS);

        if(AstNode.getOp() == DOT){
            auto RHS = AstNode.getRhs();
            if(!CheckLHSInField(RHS, StructType)) {
                  //todo
            }
        }else if(AstNode.getOp() == ARROW){

        }
    }
        break;
    case NodeBinaryExpr:
    {
        AstNode.getLhs()->accept(*this);
        if(!ResType) {
            //todo
        }
        TypePtr LHS = ResType;
        AstNode.getRhs()->accept(*this);
        if(!ResType) {
            //todo
        }
        ResType = ResType->OperationFinalOutput(AstNode.getOp(), LHS);
        if(!ResType) {
            //todo
        }
    }
        break;
    case NodeAsExpr:
    {
        AstNode.getLhs()->accept(*this);
        if(!ResType) {
            //todo
        }
        TypePtr LHS = ResType;
        AstNode.getRhs()->accept(*this);
        if(!ResType) {
            //todo
        }

        if(!LHS->TypeCastTo(ResType)) {
            //todo
        }
    }
        break;

    default:
        break;
    }
}

