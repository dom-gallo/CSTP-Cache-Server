//
// Created by Dominic Gallo on 4/30/22.
//

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
private:
    OpCodes opcode;
};