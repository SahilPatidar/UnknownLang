#include"../../include/parser/type.hpp"
// #include"parser/type.hpp"

namespace ast {
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
    if(i == -1){
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
}
