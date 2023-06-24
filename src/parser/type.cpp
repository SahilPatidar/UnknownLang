#include"../../include/parser/type.hpp"
// #include"parser/type.hpp"

namespace ast {

//////////////////////////////////StructType/////////////////////////////////////////

Type* StructType::getTypeAt(int i) const {
    if(i < TypeList.size()){
        return TypeList[i];
    }
}

int StructType::find(std::string n) const {
    for(int i = 0, siz = NameList.size(); i < siz; i++) {
        if(NameList[i] == n) {
            return i;
        }
    }
    return -1;
}

Type* StructType::getTypeAt(std::string n) const {
    int i = find(n);
    if(i != -1){
        return TypeList[i];
    }
    return nullptr;
}

Type* StructType::getType(std::string n) const {
    if(EleNameTypeList.find(n) == EleNameTypeList.end()) {
        return nullptr;
    }
    return EleNameTypeList.at(n);
}

 //////////////////////////////////IntType/////////////////////////////////////////

 Type* IntType::OperationFinalOutput(lex::Tok op, const Type* type) const {
    if(op.IsAssignOP()) {
        
    }
 }
 bool IntType::UnaryOpMatch(lex::Tok op) {

 }
 bool IntType::IsCastRequire(Type *tofrom) {
    if(tofrom->type() != TypeInt){
        return true;
    }
    auto type = static_cast<IntType*>(tofrom);
    int bit = type->getbit();
    if( (this->bit != bit) || ( (!isSign&&type->isSignInt()) || (isSign&&!type->isSignInt()) ) ){
        return true;
    }
    return false;
 }

 bool IntType::IsCastable(Type *tofrom) {
    switch (tofrom->type())
    {
    case TypeDecimal:
    {
        auto type = static_cast<FloatType*>(tofrom);
        int bit = type->getbit();
        if( (this->bit < bit)){

        } 
        
        if( (!isSign || (isSign&&!type->isSignInt()) ) ){
            return true;
        }

    }
        break;
    case TypeInt:
        /* code */
        break;
    
    case TypePointer:
        /* code */
        break;
    
    default:
        ///@code
        break;
    }
 }

 bool IntType::IsValidOperation(lex::Tok op, Type* Ty) {

 }

 //////////////////////////////////FloatType/////////////////////////////////////////

 bool FloatType::IsCastRequire(Type *tofrom) {
    if(tofrom->type() != TypeDecimal){
        return true;
    }
    auto type = static_cast<FloatType*>(tofrom);
    int bit = type->getbit();
    if( (this->bit != bit) ){
        return true;
    }
    return false;
 }


}
