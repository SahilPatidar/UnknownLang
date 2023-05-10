#pragma once
#include<memory>
#include<map>
#include"../lex/lex.hpp"
// #include"lex/lex.hpp"

namespace ast{


const enum KindType {
    TypeInt,
    TypeDecimal,
    TypeStr,
    TypeBoolean,
    TypePointer,
    TypeStruct,
    TypeArray,
    TypeConst,
    TypeFunction,
    TypeImport,
    TypeEnum,
};

class Val{
    
};


class Type{
public:
 virtual ~Type() = default;
 virtual TypePtr OperationFinalOutput(lex::Token_type op, const TypePtr type) const;
 virtual bool UnaryOpMatch(lex::Token_type op);
 virtual bool Match(TypePtr ty);
 virtual bool TypeCastTo();
 virtual KindType type() const = 0;
};

using TypePtr = std::shared_ptr<Type>;

class Int: public Type{
private:
    // Token_type tok;
    // int size;
    bool isSign;
public:
    Int(bool isSign)
    :isSign(isSign){}

    bool isSignInt() const {return isSign;}
    // int size() const {return size;}    
    TypePtr OperationFinalOutput(lex::Token_type op, const TypePtr type);
    bool Match(TypePtr ty);
    bool TypeCastTo();
    bool UnaryOpMatch(lex::Token_type op);
    KindType type() const { return TypeInt; }
};

class String: public Type{
    bool ischar;
public:
    String();
};

class Float: public Type {
private:
    int size;
public:
    Float(int size)
    :size(size) {}
    int size() const {return size;}
    bool Match(TypePtr ty);
    TypePtr OperationFinalOutput(lex::Token_type op, const TypePtr type);
    bool UnaryOpMatch(lex::Token_type op);
    bool TypeCastTo();
    KindType type() const { return TypeDecimal; }

};

class Bool: public Type {
private:

public:
    TypePtr OperationFinalOutput(lex::Token_type op, const TypePtr type);
    bool Match(TypePtr ty);
    bool UnaryOpMatch(lex::Token_type op);
    KindType type() const { return TypeBoolean; }

};

// class ConstType: public Type {
// private:
//     TypePtr type;

// public:
//     ConstType(TypePtr&_type)
//     :type(_type) {}

//     TypePtr consType() const {return type;}
//     TypePtr OperationFinalOutput(lex::Token_type op, const TypePtr type);
//     bool UnaryOpMatch(lex::Token_type op);
//     KindType type() const { return TYPE_CONST; }


// };

class EnumType: public Type {
    private:
    std::vector<std::string>uData;
    std::vector<TypePtr>uVal;
    public:
    EnumType(std::vector<std::string>&_uData, std::vector<TypePtr>&_uVal)
    :uData(_uData), uVal(_uVal) {}
};

class FunctionType: public Type {
private:
    std::vector<TypePtr>param;
    TypePtr retype;
public:
    FunctionType(std::vector<TypePtr>&_param, TypePtr &_ret)
    :param(_param), retype(_ret) {}

    std::vector<TypePtr>getParamTypes() const { return param; }
    TypePtr getRetType() const { return retype; }
    bool Match(TypePtr ty);
    TypePtr OperationFinalOutput(lex::Token_type op, const TypePtr type);
    bool UnaryOpMatch(lex::Token_type op);
    KindType type() const { return TypeFunction; }

};

class StructType: public Type{
private:
    std::vector<std::string>Name;
    std::vector<TypePtr>Type;
    std::vector<tokt>Temp;
public:
    StructType(std::vector<std::string>&_Name, 
                std::vector<TypePtr>&_Type, std::vector<tokt>&_Temp) 
    :Name(_Name), Type(_Type), Temp(_Temp) {}

    //std::string eleName() const {return name;}
    std::vector<std::string>getNameList() const {return Name;}
    std::vector<std::string>getTypeList() const {return Name;}
    TypePtr getTypeAt(int i) const {
        if(i < Type.size()){
            return Type[i];
        }
    }

    int find(std::string n) const {
        for(int i = 0, siz = Name.size(); i < siz; i++) {
            if(Name[i] == n) {
                return i;
            }
        }
        return -1;
    }

    TypePtr getTypeAt(std::string n) const {
        int i = find(n);
        if(i == -1){
            return Type[i];
        }
        return nullptr;
    }

    std::vector<tokt> getTemp() const {return Temp;}
    bool HasTemp() const {return !Temp.empty();}
    TypePtr OperationFinalOutput(lex::Token_type op, const TypePtr type);
    bool UnaryOpMatch(lex::Token_type op);
    KindType type() const { return TypeStruct; }
};

class ArrayType: public Type{
private:
    TypePtr base;
    int _size;
    std::vector<int>size;
public:
    TypePtr getArrType() const { return base; }
    std::vector<int> getArrSize() const { return size; }
    TypePtr OperationFinalOutput(lex::Token_type op, const TypePtr type);
    bool Match(TypePtr ty);
    bool UnaryOpMatch(lex::Token_type op);
    KindType type() const { return TypeArray; }
};


class PointerType: public Type{
private:
    int size;
    TypePtr base;
public:
    PointerType(TypePtr &_base)
    :base(_base) {}

    TypePtr getBaseType() const {return base;}
    int getDefCount() const { return size; }
    KindType type() const { return TypePointer; }
    bool Match(TypePtr ty);
    bool UnaryOpMatch(lex::Token_type op);
    // KindType base_type() const { reTYPE_turn base->type(); }
    TypePtr OperationFinalOutput(lex::Token_type op, const TypePtr type);


};


class TypeGenerator{
    public:
    static TypePtr Integer();
    static TypePtr Boolean();
    static TypePtr Decimal();
    static TypePtr String(Token_type tok);
    static TypePtr TupleTyGen(std::map<int,TypePtr>&ty);
    static TypePtr ArrayTyGen(TypePtr &base, int size);
    static TypePtr PointerTyGen(int count,TypePtr base);
    static TypePtr StructTyGen(std::map<std::string, TypePtr>&_ele);
    static TypePtr FuncTyGen(std::vector<TypePtr>&p_type, TypePtr r_type);
    static TypePtr EnumTyGen(std::vector<std::string>&uData, std::vector<TypePtr>&type);
    static TypePtr Generate(KindType type);
};

}