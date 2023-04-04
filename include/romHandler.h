#ifndef ROMHANDLER_H
#define ROMHANDLER_H


struct romBuffer {
    char * buffer;
    size_t size;
};

romBuffer readROM(char * cfilename);

#endif