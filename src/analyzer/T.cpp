#include"../../include/analyzer/TypeChecker.hpp"
#include"../../include/Error.hpp"
// #include"analyzer/TypeChecker.hpp"

namespace analyzer{



bool TypeChecker::visit(Expression  *AstNode) {
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
    return true;
}

}

 