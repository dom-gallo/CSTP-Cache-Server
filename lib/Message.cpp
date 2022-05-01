//
// Created by Dominic Gallo on 4/30/22.
//

#include <iostream>
#include "Message.h"
#include "OpCodes.h"
class Message {
public:
    Message(int opValue){
        opcode = static_cast<OpCodes>(opValue);
    }

    Message() {}

    OpCodes getOpCode(){
        return opcode;
    }
    void setOpcode( int x){
        opcode = static_cast<OpCodes>(x);
    }
    void setCapacity(int newCapacity){
        if(capacity >= MAX_SIZE){
            std::cout << "Greater than maximum storage " << std::endl;
        }
        capacity = newCapacity;
    }
    const int MAX_SIZE = 2147483647-1;
    void setBuffer(char *bufferIn){
        buffer = bufferIn;
    }
    char* getBuffer(){
        return buffer;
    }
    int getCapacity(){
        return capacity;
    }
    int getSize(){
        return size;
    }
    void setSize(int x){
        size = x;
    }
    void addToSize(int x){
        size += x;
    }
    void setKey(char *keyIn){
        key = keyIn;
    }
    char* getKey(){
        return key;
    }
private:
    OpCodes opcode;
    char *key;
    int size = 0;
    int capacity = 0;
    char *buffer;
};