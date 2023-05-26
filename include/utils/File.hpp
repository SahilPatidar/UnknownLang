#ifndef FILE_H
#define FILE_H
#include<iostream>
#include<filesystem>

class InputFile{
private:
    std::string src;
    
public:
    InputFile(std::string path);
    ~InputFile();

    std::string getSrcString();
    std::string getPath();

};

#endif