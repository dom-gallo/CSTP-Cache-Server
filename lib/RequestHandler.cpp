//
// Created by Dominic Gallo on 4/30/22.
//

//#include "RequestHandler.h"

#include "Message.cpp"
#include <unistd.h>
#include <iostream>
#include "OpCodes.h"

class RequestHandler {
public:
    int getConFD(){
        return connFileDescriptor;
    }
    RequestHandler(int &fd, char *buf){
        connFileDescriptor = fd;
        buffer = buf;
    };
    Message* getMessage(){
        return &message;
    }
    int getOpCodeFromStream(){
        int n = read(connFileDescriptor, buffer, 1);
        if(n < 1){
            std::cout << "Could not read 1 byte from the stream to get the op code" << std::endl;
        }

        if(buffer[0] == OP_GET){
            printf("DATA RECEIVED: %02x \n", buffer[0]);
            printf("Get (@) OP CODE RECEIVED\n");
            return OP_GET;
        }
        return '#';
    }
    int getPayLoadSize(){
        int n = read(connFileDescriptor, buffer, 4);
        if(n < 4) {
            std::cout << "Could not read dataSize bytes" << std::endl;
        }
        int dataSize = 0;
        for(int i = 0; i < n; i++){
            dataSize += buffer[i];
        }
//        std::cout << "payload Size: " << dataSize << std::endl;
        setPayLoadSize(dataSize);
        return dataSize;
    }
    char* getKey(){
        char* key = new char[8];
        int n = read(connFileDescriptor, buffer, 8);
        if(n<8)
        {
            std::cout << "Could not read key bytes" << std::endl;
        }
        for(int i = 0; i < 8; i++){
            key[i] = buffer[i];
        }
        return key;
    }
    void loadDataIntoMessage(Message *message){
        int messageCapacity = message->getCapacity();
        if(messageCapacity > 1024) {
            messageCapacity = 1024;
        }
        int n;
        // Loop for loading in chunks of data.
        char *messageBuffer = message->getBuffer();

        while( (n = read(connFileDescriptor, messageBuffer, messageCapacity)) > 0){
            if(n < 0) {
                std::cout << "Error reading data in message buffer " << std::endl;
            }
            std::cout << "Data = ";
            for(unsigned int i = 0; i < messageCapacity; ++i)
            {
                printf("%02x ", buffer[i]);
            }
            std::cout<< std::endl;

//            std::cout << "Server received " << messageCapacity << " bytes: " << buffer << std::endl;
        }
    }
private:
    int connFileDescriptor;
    int payloadSize;
    void setPayLoadSize(int datasize){
        payloadSize = datasize;
    }
    char *buffer;
    Message message;
};


