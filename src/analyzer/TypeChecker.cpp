#include"analyzer/TypeChecker.hpp"

namespace analyzer{


TablePtr TypeChecker::newTable(TablePtr table) {
    return std::make_shared<SymTable<TypePtr>>(table);
}

bool TypeChecker::checkTuple(TypePtr type1, TypePtr type2) {
    if(type1->type() != TypeTuple){
        //todo
    }
    auto ty1 = std::dynamic_pointer_cast<TupleType>(type1);
    auto ty2 = std::dynamic_pointer_cast<TupleType>(type2);
    if(ty1->tupleTy().size() != ty2->tupleTy().size()){
        //todo
    }
    for(int i = 0; i < ty2->tupleTy().size(); i++){
        if(ty1->tupleTy().at(i)->type() != ty2->tupleTy().at(i)->type()) {
            //todo
        }
    }
    return true;
}

std::string TypeChecker::identifier(const AstPtr& iden) {
    if(iden->nodeCategory() != NodeIdent){
        //todo
    }

    return std::dynamic_pointer_cast<ast::Identifier>(iden)->iden();
} 

bool TypeChecker::isValid(const TypePtr &type1, const TypePtr &type2) {
    if((type1&&!type2) ||
        (!type1&&type2)) {
        //todo
    } 
    if((type1 != NULL && type2 != NULL) && (type1->type() != type2->type())){
        //todo
    }

    return true;
}   

bool TypeChecker::visit(const NumericLiteral& astnode) {
    m_type = TypeGenerator::Integer();
    return true;
}

bool TypeChecker::visit(const FloatLiteral& astnode) {
    m_type = TypeGenerator::Decimal();
    return true;
}

bool TypeChecker::visit(const StringLiteral& astnode) {
    m_type = TypeGenerator::String(astnode.token().tok_type);
    return true;
}

bool TypeChecker::visit(const BoolLiteral& astnode) {
    m_type = TypeGenerator::Boolean();
    return true;
}

bool TypeChecker::visit(const PreDefineType& astnode) {
    switch(astnode.token().tok_type){
        case I8:
        case I16:
        case I32:
        case I64:
        {
            m_type = TypeGenerator::Integer();
            break;
        }
        case UI8:
        case UI16:
        case UI32:
        case UI64:
        {
            m_type = TypeGenerator::Integer();
            break;
        }
        case F32:
        case F64:
        {
            m_type = TypeGenerator::Decimal();
            break;
        }
        case STRING:
        {
            m_type = TypeGenerator::String(astnode.token().tok_type);
            break;
        }
        case BOOL:
        {
            m_type = TypeGenerator::Boolean();
            break;
        }
        default:
            //todo
    }
    return true;
}

bool TypeChecker::visit(const Identifier& astnode) {
    if(!table->contains(astnode.iden())){
        err::out("cannot find value `b` in this scope", astnode.token());
    }else{
        m_type = table->get(astnode.iden());
    }

    //if(m_type->type() != unserdefined){
        //todo
    //}
    return true;
}


bool TypeChecker::visit(const BineryExpr& astnode) {
    astnode.leftOpr()->accept(*this);
    TypePtr left = m_type;
    astnode.rightOpr()->accept(*this);
    if(!m_type){
        err::out("cannot find value `b` in this scope", );
    }

    TypePtr finalty = left->OperationFinalOutput(astnode.Operator(), m_type);
    if(!finalty){
        //todo
    }

    m_type = finalty;
    return true;
}

bool TypeChecker::visit(const AssignmentExpr& astnode) {
    astnode.lvalue()->accept(*this);
    TypePtr lval = m_type;
    astnode.rvalue()->accept(*this);
    if(m_type == NULL){

    }

    TypePtr ty = lval->OperationFinalOutput(astnode.Operator(), m_type);
    if(!ty){
        //todo
    }

    if(lval->type() == TypeTuple){
        if(checkTuple(lval,m_type)){
            //todo
        }
    }
    m_type = ty;
    return true;
}


bool TypeChecker::visit(const FunctionCall& astnode) {
    astnode.Calle()->accept(*this);
    if(m_type->type() != TypeFunction){
            //todo
    }
    auto functype = std::dynamic_pointer_cast<ast::FuncType>(m_type);
    if(functype->paramType().size() != astnode.Args().size()){
        //todo
    }else{
        for(int i = 0 ; i < astnode.Args().size(); i++){
            astnode.Args()[i]->accept(*this);
            isValid(functype->paramType()[i],m_type);            
        }
    }

    m_type = functype->retType();
    return true;
}

//todo--------============
bool TypeChecker::visit(const FunctionDef& astnode) {
    
    table->setFlag(true);
    std::string name = identifier(astnode.getFuncName());
    if(table->contains(name)){
        //todo
    }
    std::vector<TypePtr>p_type;
    for(auto &param: astnode.getPName()){
        // std::dynamic_pointer_cast<ast::Parameter>(param)->p_type()->accept(*this);
        if(table->contain(astnode.getPName()->toString(), true)){
            //todo
        }
        astnode.getPTy()->accept(*this);
        if(m_type == NULL){
            //todo
        } else {
            p_type.push_back(m_type);
            table->insert(identifier(param),m_type);
        }
    }       
    astnode.getResultType()->accept(*this);
    if(m_type == NULL){
        //todo
    }
    retype = m_type;
    m_type = TypeGenerator::FuncTypeGenerate(p_type,retype);
    table->pop();
    table->setFlag(false);
    table->insert(name, m_type);
    return true;
}


bool TypeChecker::visit(const ReturnStmt& astnode) {
    if(!retype&&!astnode.value()){
        return true;
    }else if(!retype&&astnode.value()||retype&&!astnode.value()){
        //todo
    }
    m_type = retype;
    astnode.value()->accept(*this);
    if(!isValid(m_type, retype)){
        //todo
    }
    return true;
}


bool TypeChecker::visit(const ListExpr& astnode) {
    if(astnode.listof().empty()){
        //todo
    }
    astnode.listof()[0]->accept(*this);
    TypePtr prev_type = m_type;
    for(auto &val: astnode.listof()){
        val->accept(*this);
        if(m_type->type() != prev_type->type()){
            //todo
        }
    }
    m_type = TypeGenerator::ArrayTyGen(m_type, astnode.listof().size());
    return true;
}


bool TypeChecker::visit(const PointerExpr& astnode) {
    if(astnode.isType()){
        astnode.type()->accept(*this);
        if(!m_type){
            //todo
        }
        m_type = TypeGenerator::PointerTyGen(m_type);
    }else {   
        auto node = std::dynamic_pointer_cast<PointerExpr>(astnode.type());
        while(node->type()->nodeCategory() == NodePtr) {
            auto node = std::dynamic_pointer_cast<PointerExpr>(astnode.type());
        }
        node->getType()->accept(*this);
        check(m_type, astnode);
    }
    return true;
}

bool TypeChecker::visit(const RefExpr &astnode) {
    astnode.getBase()->accept(*this);
    if(!m_type) {
        //todo
    }
    return true;
}


bool TypeChecker::visit(const PrefixExper &astnode) {
    astnode.variable()->accept(*this);
    TypePtr type = m_type;
    if(m_type == NULL){
        //todo
    }
    m_type->UnaryOpMatch(astnode.oprator());
    return true;
}

bool TypeChecker::visit(const VarStmt& astnode) {
    auto& letnode = const_cast<VarStmt&>(astnode);
    std::string name = identifier(astnode.varName());
    if(table->contains(name)){
        //todo check local and global 
            //todo
    }
    if(astnode.token().tok_type == LET){
        
        astnode.varType()->accept(*this);
        if(m_type == NULL) {
            astnode.varVal()->accept(*this);
            if(m_type == NULL) {
                //todo
            }else {
                letnode.setType(m_type);
            }
        }else { 
            TypePtr ty = m_type;
            astnode.varVal()->accept(*this);
            if(ty->type() == TypeTuple) {
                if(checkTuple(ty,m_type)) {
                    //todo
                }
            }else if(ty->type() == TypeArray){
                //todo
            }
            if(!ty->Match(m_type)) {
                //todo
            }   
        }

    }else if(astnode.token().tok_type == CONST) {
        //todo
        astnode.varType()->accept(*this);
        if(m_type == NULL) {
            //todo
        }else{
            TypePtr type = m_type;
            astnode.varVal()->accept(*this);
            if(m_type == NULL){
                //todo
            }
            isValid(type, m_type);
        }

    }
    table->insert(name, m_type);
    return true;
}

bool TypeChecker::visit(const AsState &astnode) {
    astnode.getLOparand()->accept(*this);
    if(!m_type) {
        //todo
    }
    TypePtr ty = m_type;
    astnode.getROparand()->accept(*this);
    if(!m_type) {
        //todo
    }
    if(!ty->TypeCastTo(m_type)){
        //todo
    }
    return true;
}

bool TypeChecker::visit(const Variadic &astnode) {
    
}

bool TypeChecker::visit(const Extern &astnode) {

}

bool TypeChecker::visit(const Path &astnode) {

}


bool TypeChecker::visit(const ForInLoop& astnode) {
    TablePtr old = table;
    table = newTable(table);
    astnode.leftExpr()->accept(*this);
    astnode.rightExpr()->accept(*this);
    if(m_type){
        //todo
    }
    //body
    table = old;
    return true;
}


bool TypeChecker::visit(const IfStatement &astnode) {
    astnode.condition()->accept(*this);
    if(m_type == NULL){
        //todo
    }
    
    TypePtr type = m_type;
    //body;
    return true;
}


bool TypeChecker::visit(const StructStmt &astnode) {
    TypePtr ty;
    if(astnode.isDeclaration()){
        std::string name = identifier(astnode.name());
        if(table->contains(name)){
            //todo
        }
        std::map<std::string,TypePtr>ele;
        if(!astnode.getElements().empty()){
            for(size_t i = 0, siz = astnode.getElements().size(); i < siz; i++){
                auto val = std::dynamic_pointer_cast<Parameter>(astnode.getElements()[i]);
                std::string name = identifier(val->identifier());
                if(ele.contain(name)){
                    //todo
                    return false;
                }
                val->p_type()->accept(*this);
                if(m_type == NULL){
                    //todo
                }else{

                    ele.insert({name, m_type});
                }
            }
        }
        m_type = TypeGenerator::StructTyGen(ele);
        table->insert(name,m_type);
        return true;
    }else{
        astnode.name()->accept(*this);
        TypePtr ty;
        if(m_type->type() != TypeStruct){
        ///todo
        }else{
            ty = m_type;
            auto type = std::dynamic_pointer_cast<StructType>(m_type);
            if(astnode.getElements().size() != type->member().size()){
                //todo
            }else{
                for(size_t i = 0, siz = astnode.getElements().size(); i < siz; i++){
                    auto val = std::dynamic_pointer_cast<Parameter>(astnode.getElements()[i]);
                    std::string n = identifier(val->identifier());
                    val->p_type()->accept(*this);
                    if(!type->contain(n)){
                        //todo
                    }else if(m_type == NULL){
                        if(!type->matchType(n, m_type)){
                                //todo
                        }
                    }
                    
                }
            }
        }
    }
    m_type = ty;
    return true;
}


bool TypeChecker::tupleAccessExpr(MemberExpr node) {
    if(!isInField)
        node.owner()->accept(*this);

    if(!m_type){
        //todo
    }
    auto tuplety = std::dynamic_pointer_cast<TupleType>(m_type);
    if(node.reference()->nodeCategory() == NodeNumLit) {
        if(!tuplety->contain(node.reference()->toString())){
            //todo
        }
    }else if(node.reference()->nodeCategory() == NodeIndexExpr){
        auto arr = std::dynamic_pointer_cast<IndexExpr>(node.reference());
        if(arr->identifier()->nodeCategory() != NodeNumLit) {
            //todo
        }
        if(!tuplety->contain(arr->identifier()->toString())){
            //todo
        }
        auto ty = tuplety->get(arr->identifier()->toString());
        if(ty->type() != TypeArray||
            ty->type() != TypeStr ||
             ty->type() != TypePointer){
            //todo
        }
        
    }else if(node.reference()->nodeCategory() == NodeMemExpr){
        auto expr = std::dynamic_pointer_cast<MemberExpr>(node.reference());
        if(expr->owner()->nodeCategory() != NodeNumLit) {
            //todo
        }
        if(!tuplety->contain(expr->owner()->toString())){
            //todo
        }
        auto ty = m_type;
        m_type = tuplety->get(expr->owner()->toString());
        if(m_type->type() != TypePointer || m_type->type() != TypeStruct || 
            (node.memOperator() == DOT&&m_type->type() != TypeTuple)){
            //todo
        }
        isInField = true;
        node.reference()->accept(*this);
        isInField = false;
    }
    return true;
}




bool TypeChecker::visit(const MemberExpr &astnode){
    if(!isInField){
        astnode.owner()->accept(*this);
    }
    std::shared_ptr<StructType> structy;
    if(astnode.memOperator() == DOT){
        if(!m_type){
            //todo
        }
        if(m_type->type() == TypeStruct) {
            structy = std::dynamic_pointer_cast<StructType>(m_type);
        }else if(m_type->type() == TypeTuple) {
            if(!tupleAccessExpr(astnode)){
                //todo
            }
        }

    } else if(astnode.memOperator() == ARROW){
        if(!m_type){
            //todo
        }
        if(m_type->type() != TypePointer) {
            //todo
        }
        auto ty = std::dynamic_pointer_cast<Pointer>(m_type);
        if(ty->baseType()->type() != TypeStruct){
            //todo
        }
        structy = std::dynamic_pointer_cast<StructType>(ty->baseType());
    }
    if(astnode.reference()->nodeCategory() == NodeIdent){
        std::string n = identifier(astnode.reference());
        if(!structy->contain(n)){
            //todo
        }
        m_type = structy->getMem(n);
    }else if(astnode.reference()->nodeCategory() == NodeMemExpr){
        auto newowner = std::dynamic_pointer_cast<MemberExpr>(astnode.reference());
        std::string iden; 
        if(newowner->owner()->nodeCategory() == NodeIdent){
            iden = identifier(newowner->owner());
        }else if(newowner->owner()->nodeCategory() == NodeIndexExpr){
            auto arr = std::dynamic_pointer_cast<IndexExpr>(astnode.owner());
            iden = identifier(arr->identifier());
        }
        if(!structy->contain(iden)){
            //todo
        }
        m_type = structy->getMem(iden);
        isInField = true;
        astnode.reference()->accept(*this);
        isInField = false;
    }else if(astnode.reference()->nodeCategory() == NodeIndexExpr){
        auto arr = std::dynamic_pointer_cast<IndexExpr>(astnode.reference());
        std::string n = identifier(arr->identifier());
        if(!structy->contain(n)){
            //todo
        }
        m_type = structy->getMem(n);
        if(m_type->type() != TypeArray){
            //todo
        }
    }else{
        //todo
    }

    return true;
}



bool TypeChecker::visit(const ArrayType& astnode) {
    astnode.arraytype()->accept(*this);
    std::vector<std::string>s;
    TypePtr ty; 
    if(m_type == NULL) {
        //todo
    }else{
        ty = m_type;
        if(astnode.arraysize().empty()){
            //todo
        }else{
            for(int i = 0 ; i < astnode.arraysize().size(); i++){
                astnode.arraysize()[i]->accept(*this);
                if(!m_type){
                    //todo
                }
                s.push_back(astnode.arraysize()[i]->toString());
            }
        }
    }
    m_type = TypeGenerator::ArrayTyGen(ty,s);
    return true;
}




bool TypeChecker::visit(const IndexExpr& astnode ) {
    astnode.identifier()->accept(*this);
    if(!m_type && m_type->type() != TypeArray && m_type->type() != TypePointer) {
        err::out("cannot index into a value of type {%s}",m_type->type()); 
        return false;
    }
    if(astnode.arrDim().size() < m_type->arrSize()){
        err::out("cannot index into a value of type {%s}", m_type ,astnode.toString()); 
        return false;
    }
    if(astnode.arrDim().size() > m_type->arrSize()){
        err::out("mismatched types {%s}",m_type->type()); 
        return false;
    }

    m_type = std::dynamic_pointer_cast<Array>(m_type)->arrType();
    return true;
}


bool TypeChecker::visit(const Tuple& astnode) {
    std::map<int,TypePtr>tupleIndexMap;
    if(!astnode.tupleEle().empty()){
        for(int i = 0 ; i < astnode.tupleEle().size(); i++){
            astnode.tupleEle()[i]->accept(*this);
            if(!m_type){
                //todo
            }
            tupleIndexMap.insert({i,m_type});
        }
    }
    m_type = TypeGenerator::TupleTyGen(tupleIndexMap);
    return true;
}


}
 