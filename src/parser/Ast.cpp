
#include"../../include/parser/Ast.hpp"
// #include"parser/Ast.hpp"

namespace ast {
    
    std::string NumericLiteral::toString() const{
        return tok.data;
    }

    std::string FloatLiteral::toString() const{
        return tok.data;
    }

    std::string BoolLiteral::toString() const {
        return tok.data;
    }

    std::string StringLiteral::toString() const {
        return tok.data;
    }

    std::string NullLiteral::toString() const {
        return tok.data;
    }

    std::string Identifier::toString() const {
        return tok.data;
    }

    std::string BlockStmt::toString() const {
        std::string str = "";
        for(int i = 0 ; i < statms.size(); i++){
            str += statms[i]->toString();
            str += "\n";
        }
        return str;
    }

    std::string Extern::toString() const {
        std::string str = "extern ";
        for(int i = 0, size = header.size(); i < size; i++) {
            str += header[i]->toString();
        }
        return str;
    }
    

    std::string EnumExpr::toString() const {
        std::string str = "enum ";
        str += Name.data;
        str += " {\n";
        for (size_t i = 0, size = udata.size(); i < size; i++) {
            if (i)
                str += "\n";

            str += udata[i].data;
            if(val[i]) {
                str += " = ";
                str += val[i]->toString();
            }
            str += ",";
        }
        str += "}\n";

        return str;
    }

    std::string PreDefineType::toString() const {
        return tok.data;
    }

    std::string TypeStmt::toString() const {
        std::string str = "type";
        str += " ";
        str += Ident.data;
        str += " ";
        str += type->toString();
        str += ";";
        return str;
    }

    std::string VarStmt::toString() const {
        std::string str = tok.data;
        str += " ";
        str += name->toString();
        str += " : ";
        if(type){
            str += type->toString();
        }
        if(val){
            str += " = ";
            str += val->toString();
        }
        str += ";";
        return str;
    }


    std::string Array::toString() const {
        std::string str = "";
        str = "[";
        for(int i = 0 ; i < size.size(); i++){
            if(!size[i]){

            }
            str += size[i]->toString();
            str += "; ";
        }
        str += "]";
        str += type->toString();
        return str;
    }

    std::string GroupedExpr::toString() const {
        std::string str = "(";
        str += expr->toString();
        str += ")";
        return str;
    }

    
    std::string FnType::toString() const {
        std::string str = "fn";
        str += "(";
        if(!ty.empty()){
            for(int i = 0 ; i < ty.size(); i++){
                str += ty[i]->toString();
                str += ", ";
            }
        }
        str += ")";
        if(ret){
            str += ret->toString();
        }
        return str;
    }

    
    std::string PrefixExpr::toString() const {
        std::string str = "";
        str += lex::token[op];
        str += base->toString();
        return str;
    }
    
    
    std::string ForLoop::toString() const {
        std::string str = "for ";
        str += var->toString();
        str += ";";
        str += cond->toString();
        str += ";";
        str += incr->toString();
        str += " : {\n";
        str += body->toString();
        str += "}\n";
        return str;
    }

    std::string WhileLoop::toString() const {
        std::string str = "while ";
        str += expr->toString();
        str += " {\n";
        str += body->toString();
        str += "}\n";
        return str;
    }

    
    std::string IfStmt::toString() const {
        std::string str = "if ";
        str += cond->toString();
        str += " {\n";
        str +=  ifblock->toString();
        str += " } ";
        if(elblock){
            str += "else ";
            str += " {\n";
            str += elblock->toString();
            str += " }";
        }
        return str + "\n";
    }
    
    std::string BranchStmt::toString() const {
        return tok.data + ";\n";
    }
    
    std::string Expression::toString() const {
        std::string str = "";
        str += LHS->toString();
        str += lex::token[Op];
        str += RHS->toString();
        return str;
    }
    
    std::string StructStmt::toString() const {
        std::string str = "struct ";
        str += Name.data;
        if(!Temp.empty()){
            str += "<";
            for(int i = 0 ; i < Temp.size(); i++){
                str += Temp[i].data;
                str += ",";
            }
            str += ">";
        }
        str += "{\n";
        for(int i = 0 ; i < elmtName.size(); i++){
            str += elmtName[i].data;
            str += ": ";
            str += elmtTy[i]->toString();
            str += ",\n";
        }
        str += " };";
        return str;
    }
    
    std::string ListExpr::toString() const {
        std::string str = " [ ";
        for(int i = 0 ; i < list.size(); i++){
            str += list[i]->toString();
            str += ",";
        }
        str += " ] ";
        return str;
    }
   
    std::string FunctionDef::toString() const {
        std::string str = "fn ";
        str += Name.data;
        str += " ( ";
        if(!pName.empty()){
         
            for(size_t i = 0; i < pName.size(); i++){
                str += pName[i].data;
                str += pTy[i]->toString();
                str += ",";
            }
        }

        str += " ) ";
        if(retype){
            str += " -> ";
            str += retype->toString();
        }
        // if(AssociateTo) {
        //     str += " for ";
        //     str += AssociateTo->toString();
        // }
        if(Block)
        {    
            str += " {\n";
            str += Block->toString();
            str += " }\n";
        }else{
            str += ";\n";
        }
        return str;
    }


    std::string ReturnStmt::toString() const {
        std::string res = "return ";

        res += val->toString();
        res += ";\n";
        return res;
    }

    
    std::string FunctionCall::toString() const {
        std::string res = "";

        res += name->toString();
        res += "(";

        for (size_t i = 0; i < args.size(); i++) {
            if (i)
                res += ", ";

            res += args[i]->toString();
        }

        res += ")";
        return res;
    }
}
