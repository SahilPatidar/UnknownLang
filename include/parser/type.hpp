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
    TypeEnum,
    TypeRef,
    TypeContainedTy
};



class Type{
protected:
    bool IsConst;
    bool HasRefOrAndOp;
public:
 virtual ~Type() = default;
 virtual Type* OperationFinalOutput(lex::Tok op, const Type* type) const;
 virtual bool UnaryOpMatch(lex::Tok op);
 virtual bool IsCastRequire(Type *tofrom);
 virtual bool IsCastable(Type *tofrom);
 virtual bool IsValidOperation(lex::Tok op, Type* Ty);
 virtual KindType type() const = 0;
 inline bool isConst() const { return IsConst; }
 inline void setConst(bool _isConst) { IsConst = _isConst; }
};


class IntType: public Type{
private:
    uint16_t bit;
    bool isSign;
public:
    IntType(uint16_t &_bit, bool &isSign)
    :bit(_bit), isSign(isSign){}

    bool IsCastRequire(Type *tofrom);
    bool IsCastable(Type *tofrom);
    bool IsValidOperation(lex::Tok op, Type* Ty);

    bool isSignInt() const {return isSign;}
    int getbit() const {return bit;}    
    Type* OperationFinalOutput(lex::Tok op, const Type* type) const;
    bool UnaryOpMatch(lex::Tok op);
    KindType type() const { return TypeInt; }
};



class FloatType: public Type {
private:
    uint16_t bit;
public:
    FloatType(uint16_t &_bit)
    :bit(_bit) {}
   
    bool IsCastRequire(Type *tofrom);
    bool IsCastable(Type *tofrom);
    bool IsValidOperation(lex::Tok op, Type* Ty);
   
    int getbit() const {return bit;}
    Type* OperationFinalOutput(lex::Tok op, const Type* type);
    bool UnaryOpMatch(lex::Tok op);
    KindType type() const { return TypeDecimal; }

};

class BoolType: public Type {
private:

public:
    Type* OperationFinalOutput(lex::Tok op, const Type* type);
    bool UnaryOpMatch(lex::Tok op);
    KindType type() const { return TypeBoolean; }

};

// class ConstType: public Type {
// private:
//     Type* type;

// public:
//     ConstType(Type*&_type)
//     :type(_type) {}

//     Type* consType() const {return type;}
//     Type* OperationFinalOutput(lex::Tok op, const Type* type);
//     bool UnaryOpMatch(lex::Tok op);
//     KindType type() const { return TYPE_CONST; }


// };

class EnumType: public Type {
    private:
    std::vector<std::string>uData;
    std::vector<Type*>uVal;
    public:
    EnumType(std::vector<std::string>&_uData, std::vector<Type*>&_uVal)
    :uData(_uData), uVal(_uVal) {}
};

class FunctionType: public Type {
private:
    std::vector<Type*>param;
    Type* retype;
public:
    FunctionType(std::vector<Type*>&_param, Type* &_ret)
    :param(_param), retype(_ret) {}

    std::vector<Type*>getParamTypes() const { return param; }
    Type* getRetType() const { return retype; }
    Type* OperationFinalOutput(lex::Tok op, const Type* type);
    bool UnaryOpMatch(lex::Tok op);
    KindType type() const { return TypeFunction; }

};

class StructType: public Type{
private:
    std::map<std::string, Type*>EleNameTypeList;
    std::vector<std::string>NameList;
    std::vector<Type*>TypeList;
    std::vector<TypeTy*>Temp;
    std::map<std::string,Type*>Impl;
public:
    StructType(std::map<std::string, Type*>&_EleNameTypeList, std::vector<TypeTy*>&_Temp) 
    :EleNameTypeList(_EleNameTypeList), Temp(_Temp) {}

    std::map<std::string, Type*>getNameTypeList() const {return EleNameTypeList;}
    std::vector<std::string>getNameList() const { return NameList; }
    std::vector<Type*>getTypeList() const { return TypeList; }
    Type* getTypeAt(int i) const;

    int find(std::string n) const;
    bool contains(std::string n) const;

    Type* getTypeAt(std::string n) const;
    Type* getType(std::string n) const;

    void setImpl(std::map<std::string,Type*> &_Impl) { Impl = _Impl; }
    bool HasImpl() const {
        if(Impl.empty()) {
            return false;
        }
        return true; 
    }
    std::map<std::string,Type*> getImpl() const { 
        if(!HasImpl()) {
            return {};
        }
        return Impl; 
    }
    Type* getImpl(std::string &n) { 
        return Impl.find(n) == Impl.end()? nullptr: Impl[n]; 
    }
    void InsertField(std::string &n, Type* ty); 
    std::vector<TypeTy*> getTemp() const {return Temp;}
    bool HasTemp() const {return !Temp.empty();}
    Type* OperationFinalOutput(lex::Tok op, const Type* type);
    bool UnaryOpMatch(lex::Tok op);
    KindType type() const { return TypeStruct; }
};


class ArrayType: public Type{
private:
    Type* base;
    int Dim;
public:
    Type* getArrType() const { return base; }
    // std::vector<int> getArrSize() const { return size; }
    int getArrSize() const { return Dim; }
    Type* OperationFinalOutput(lex::Tok op, const Type* type);
    bool UnaryOpMatch(lex::Tok op);
    KindType type() const { return TypeArray; }
};

class TypeTy: public Type{
private:
    uint32_t TyId;
public:
    TypeTy();
    ~TypeTy();

    uint32_t getContainedId() const { return TyId; }

    Type* OperationFinalOutput(lex::Tok op, const Type* type);
    bool UnaryOpMatch(lex::Tok op);
    KindType type() const { return TypeContainedTy; }
};


class PointerType: public Type{
private:
    int size;
    Type* base;
public:
    PointerType(Type* &_base)
    :base(_base) {}

    Type* getBaseType() const {return base;}
    int getDefCount() const { return size; }
    KindType type() const { return TypePointer; }
    bool UnaryOpMatch(lex::Tok op);
    Type* OperationFinalOutput(lex::Tok op, const Type* type);
};

class RefType: public Type{
private:
    Type* base;
public:
    RefType(Type* &_base)
    :base(_base) {}

    Type* getBaseType() const {return base;}
    KindType type() const { return TypeRef; }
    bool UnaryOpMatch(lex::Tok op);
    Type* OperationFinalOutput(lex::Tok op, const Type* type);
};

class TypeGenerator{
    public:
    static IntType* GenerateIntType(uint16_t bit,bool isSign);
    static BoolType* GenerateBoolType();
    static FloatType* GenerateFltType(uint16_t bit);
    static ArrayType* GenerateArrType(Type* &base, int size);
    static PointerType* GeneratePtrType(int count,Type* base);
    static StructType* GenerateStructType(const std::vector<std::string>&str, const std::vector<Type*>&ty,const std::vector<TypeTy*>&temp);
    static FunctionType* GenerateFuncType(std::vector<Type*>&p_type, Type* r_type);
    static EnumType* GenerateEnumType(std::vector<std::string>&uData, std::vector<Type*>&type);
    static RefType* GenerateRefType(Type*&ty);
    static TypeTy* GenerateTypeTy(uint32_t id);
};

}