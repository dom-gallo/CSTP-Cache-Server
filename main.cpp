/*
 * CSTP_server:
 * Built on top of a boilerplate echo server written in C.
 * usage:  <port>
 *
 * echoserver.c - A simple connection-based echo server
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include "lib/RequestHandler.cpp"
#define BUFSIZE 1024

#if 0
#endif
int getListenFD();
void buildServerAddress(struct sockaddr_in &server_in, const int portno);
void listenOnWith(int listenFileDescriptor,int portNumber, int maxNumConnections);

/*
 * error - wrapper for perror
 */
void error(char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char **argv) {
    int listenfd; /* listening socket */
    int connfd; /* connection socket */
    int portno; /* port to listen on */
    int clientlen; /* byte size of client's address */
    struct sockaddr_in serveraddr; /* server's addr */
    struct sockaddr_in clientaddr; /* client addr */
    struct hostent *hostp; /* client host info */
    char buf[BUFSIZE]; /* message buffer */
    char *hostaddrp; /* dotted decimal host addr string */
    int optval; /* flag value for setsockopt */
    int n; /* message byte size */

    /* check command line args */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }
    portno = atoi(argv[1]);

    /* socket: create a socket */
    listenfd = getListenFD();


    /* setsockopt: Handy debugging trick that lets
     * us rerun the server immediately after we kill it;
     * otherwise we have to wait about 20 secs.
     * Eliminates "ERROR on binding: Address already in use" error.
     */
    optval = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
               (const void *)&optval , sizeof(int));

    /* build the server's internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    buildServerAddress(serveraddr, portno);


    /* bind: associate the listening socket with a port */
    if (bind(listenfd, (struct sockaddr *) &serveraddr,
             sizeof(serveraddr)) < 0)
        error("ERROR on binding");


    /* listen: make it a listening socket ready to accept connection requests */
    listenOnWith(listenfd, portno,5); /* allow 5 requests to queue up */

    /* main loop: wait for a connection request, echo input line,
       then close connection. */
    clientlen = sizeof(clientaddr);

    while (1) {

        /* accept: wait for a connection request */
        connfd = accept(listenfd, (struct sockaddr *) &clientaddr, reinterpret_cast<socklen_t *>(&clientlen));
        if (connfd < 0)
            error("ERROR on accept");

        /* gethostbyaddr: determine who sent the message */
        hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,
                              sizeof(clientaddr.sin_addr.s_addr), AF_INET);
        if (hostp == NULL){
            error("ERROR on gethostbyaddr");
        }

        hostaddrp = inet_ntoa(clientaddr.sin_addr);

        if (hostaddrp == NULL)
            error("ERROR on inet_ntoa\n");
        printf("server established connection with %s (%s)\n",
               hostp->h_name, hostaddrp);

        /* read: read input string from the client */
        bzero(buf, BUFSIZE);
        RequestHandler *requestHandler = new RequestHandler(connfd, buf);
        Message *message = requestHandler->getMessage();

        int opCode = requestHandler->getOpCodeFromSteam();
        message->setOpcode(opCode);
        std::cout << "message opcode: " <<  std::hex<< (OpCodes) message->getOpCode() << std::endl;

        int dataSize = requestHandler->getPayLoadSize();
        std::cout << "incoming payload size: " << std::hex << dataSize << std::endl;

        message->setCapacity(dataSize);
        message->setBuffer(buf);
        requestHandler->loadDataIntoMessage(message);

        n = write(connfd, buf, strlen(buf));
        if (n < 0){
            printf("ERROR writing to socket\n");
        }

        close(connfd);
    }
}


// Get the file descriptor index
int getListenFD(){
    int a = socket(AF_INET, SOCK_STREAM, 0);
    if ( a < 0 ){
        error("ERROR opening socket");
    }
    return a;
}
void buildServerAddress(struct sockaddr_in &server_in, const int portno){
    server_in.sin_family = AF_INET; /* we are using the Internet */
    server_in.sin_addr.s_addr = htonl(INADDR_ANY); /* accept reqs to any IP addr */
    server_in.sin_port = htons((unsigned short)portno); /* port to listen on */
}
void listenOnWith(int listenFileDescriptor,int portNumber, int maxNumConnections){
    int a = listen(listenFileDescriptor, maxNumConnections);
    if( a < 0){
        error("ERROR on listen");
    }
    std::cout << "Server listening on: " << portNumber << std::endl;
}