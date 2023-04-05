#include <vector>
#include <fstream>
#include <string>
#include <exception>
#include "romHandler.h"




romBuffer readROM(char * cfilename){
    std::ifstream file;
    std::string filename(cfilename);
    file.open(filename, std::fstream::in | std::fstream::binary);
    if(file.fail()){
        throw std::runtime_error("Cannot open file: " + filename);
    }
    std::vector<unsigned char> ROM;

    // get length of file:
    file.seekg (0, file.end);
    int length = file.tellg();
    file.seekg (0, file.beg);

    romBuffer rb;

    rb.buffer = new char [length];
    rb.size = length;

    file.read (rb.buffer,length);
    file.close();
    return rb;
}