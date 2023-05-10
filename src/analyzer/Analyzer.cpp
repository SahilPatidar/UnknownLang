#include"analyzer/Analyzer.hpp"

namespace analyzer {
    
    bool AstAnalyzer::visit(const ast::Program& astnode) {
        for(int i = 0 ; i < astnode.statements().size(); i++){
            switch(astnode.statements()[i]->nodeCategory()){
                case NodeCallExpr:
                case NodeBranchStm:
                case NodeDotDotExpr:
                case NodeWhileStm:
                case NodeForStm:
                case NodeRetStm:
                case NodeIfStm:
                {
                    //todo
                    return false;
                }
                default:
                {
                    astnode.statements()[i]->accept(*this);
                }
            }
        }
        return true;
    }

    bool AstAnalyzer::checkType(ast::NodeCategory node) {
        switch(node){
            case NodePreDefTy:
            case NodePtr:
            case NodeTuple:
            case NodeIdent:
            case NodeRef:
                break;
            default:
                return false;
        }
        return true;
    }

    bool AstAnalyzer::visit(const ast::StructState& astnode){
        astnode.name()->accept(*this);
        if(astnode.isDeclaration()){
            if(!astnode.element().empty()){
                for(int i = 0 ; i < astnode.element().size(); i++){
                    auto n = std::dynamic_pointer_cast<ast::Parameter>(astnode.element()[i]);
                    n->identifier()->accept(*this);
                    if(!checkType(n->p_type()->nodeCategory())){
                        //todo
                    }  
                    n->p_type()->accept(*this); 
                }
            }

        }else{
            for(int i = 0 ; i < astnode.element().size(); i++){
                auto n = std::dynamic_pointer_cast<ast::Parameter>(astnode.element()[i]);
                n->identifier()->accept(*this);
                n->p_type()->accept(*this); 
            }
        }
        return true;
    }

    bool AstAnalyzer::visit(const ast::Tuple& astnode){
        if(!astnode.tupleEle().empty()){
            for(int i = 0 ; i < astnode.tupleEle().size(); i++){
                astnode.tupleEle()[i]->accept(*this);      
            }
        }
        return true;
    }

    bool AstAnalyzer::visit(const ast::FunctionCall& astnode) {
        astnode.funcname()->accept(*this);
        if(!astnode.arg().empty()){
            for(int i = 0; i < astnode.arg().size(); i++ ){
                astnode.arg()[i]->accept(*this);
            }
        }
        return true;
    }

    bool AstAnalyzer::visit(const ast::BineryExper& astnode) {
        astnode.leftOpr()->accept(*this);
        astnode.rightOpr()->accept(*this);
        return true;
    }

    bool AstAnalyzer::visit(const ast::RefExpr &astnode) {
        switch (astnode.getbase()->nodeCategory())
        {
        case NodeListExpr:
        case NodeIdent:
        case NodeTuple:
        case NodeIndexExpr:
        case NodeStructStm:
        case NodeMemExpr:
            break;
        default:
            //todo
        }
        return true;
    }


    bool AstAnalyzer::visit(const ast::FunctionDef& astnode){
        auto name = std::dynamic_pointer_cast<ast::Identifier>(astnode.func_name())->iden();
        if(name == "main"){
            mainFunc = true;
        }
        astnode.func_name()->accept(*this);
        if(!astnode.parameter().empty()) {
            for(int i = 0; i < astnode.parameter().size(); i++){
                auto pty = std::dynamic_pointer_cast<ast::Parameter>(astnode.parameter()[i]);
                if(pty->identifier()->nodeCategory() != NodeIdent) {
                    //todo
                }
                if(!checkType(pty->p_type()->nodeCategory())){
                    //todo
                }
                pty->p_type()->accept(*this);

            }
        }

        switch(astnode.ret_type()->nodeCategory()){
            case NodePreDefTy:
            case NodePtr:
            case NodeTuple:
            case NodeIdent:
            case NodeRef:
                break;
            default:
                //todo
        }
        astnode.func_body()->accept(*this);
        return true;
    }
    
    
    

}
