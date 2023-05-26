#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H
#include<iostream>
namespace compiler{
namespace preprocessor{
    enum PPToken{
        PPdefine,
        PPundef,
        PPifndef,
        PPifdef,
        PPif,
        PPelse,
        PPelif,
        PPendif,
    };

    class Preprocessor {
        
    };

    class PPRegistry {
        std::vector<>
    };

    ///Lexer -> if token is pp then lex and record it 
    ///
}
}
#endif