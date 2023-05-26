#include"../parser/Ast.hpp"

namespace ast{
using ValPtr = std::shared_ptr<VALUE>;

enum KindValue{
    ValueInt,
    ValueFlt,
    ValueArray,
    ValueStruct,
    ValueEnum,
    ValueString,
    ValueBool
};

class VALUE {
    public:
    virtual ~VALUE() = default;
    virtual KindValue getKindValue() const = 0;
};

class IntVal: public VALUE {
    int64_t val;
    public:
    IntVal(int64_t &_val)
    :val(_val) {}

    int64_t getVal() const { return val; }
};

class FltVal: public VALUE {
    double val;
    public:
    FltVal(double &_val)
    :val(_val) {}
};

class StringVal: public VALUE {
    std::string val;
    public:
    StringVal(std::string &_val)
    :val(_val) {}
};

class ArrayVal: public VALUE {
    std::vector<ValPtr>vals;
    public:
    ArrayVal(std::vector<ValPtr>&_vals)
    :vals(_vals) {}

    std::vector<ValPtr>getVals() const { return vals; }
};

class EnumVal: public VALUE {
    std::vector<ValPtr>vals;
    public:
    EnumVal(std::vector<ValPtr> &_vals)
    :vals(_vals) {}

    std::vector<ValPtr>getVals() const { return vals; }
};

class StructVal: public VALUE {
    std::vector<ValPtr>vals;
    public:
    StructVal(std::vector<ValPtr> &_vals)
    :vals(_vals) {}

    std::vector<ValPtr>getVals() const { return vals; }
};
}