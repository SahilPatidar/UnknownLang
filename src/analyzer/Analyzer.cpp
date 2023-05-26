// #pragma once
#include"../../include/analyzer/Analyzer.hpp"

namespace analyzer {


bool StaticAnalyzer::visit(BlockStmt &AstNode) {
    for(int i = 0, size = AstNode.getStmts().size(); i < size; i++) {
        StmtStack->PushStmt()
    }
    return true;
}



}
