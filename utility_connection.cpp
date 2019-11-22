#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <vector>
#include <signal.h>

using namespace std;

//////////////////////////////////////////
///////////////// connection in socket
//////////////////////////////////////////

/***
 * purpose: read msg of clients
 * params:
 *      fd: file descriptor
 *      buffer: buffer to store msg
 *      len: length in bytes by msg
***/
int sct__read(int fd, char buff[], int len){
    int n = read(fd, buff, len );
    if(n < 0){
        throw string("ERROR in reading from socket");
    }
    return n;
}

/***
 * purpose: send msg to clients
 * params:
 *      fd: file descriptor
 *      msg: message to send
 *      len: length in bytes by msg
 ***/
int sct__write(int fd, const char* msg, int len){
    int n = write(fd, msg, len);
    if(n < 0){
        throw string("Error in writing to socket");
    }
    return n;
}

/***
 * purpose: accept new client in server
 * params:
 *      sockFD: file descriptor from connection queue
 *      addr: pointer with address of the peer socket client
 *      addrlen: size of pointer
 ***/
int sct__accept(int sockFD, struct sockaddr* addr, socklen_t* addrlen)
{
    int connectFD = accept(sockFD, addr, addrlen);
    if(connectFD < 0)
      throw string("error accept failed");
    return connectFD;
}

/***
 * purpose: listen for connections on a socket
 * params:
 *      sockFD: file descriptor of socket client
 *      max_length: maximum lenght to which the quere of pending connections may grow
 ***/
void sct__listen(int sockFD, int max_length){
    int res = listen(sockFD, max_length);
    if(res == -1){
        throw string("Error listen failed");
    }
}

/***
 * purpose: bind name to a server socket
 * params:
 *      sockFD: file descriptor of server socket
 *      addr: address to assign to server socket
 *      addrlen: size (in bytes) of addr
**/
void sct__bind(int sockFD, struct sockaddr* addr, int addrlen)
{
    int res = bind(sockFD, addr, addrlen);
    if(res == -1)
      throw string("ERROR bind failed");
}

/***
 * purpose: connect a new client socket
 * params:
 *      sockFD: file descriptor of server socket
 *      addr: address to assign to server socket
 *      addrlen: size (in bytes) of addr
 ***/
void sct__connect(int sockFD, struct sockaddr* addr, int addrlen)
{
    int res = connect(sockFD, addr, addrlen);
    if(res == -1)
      throw string("connect failed");
}

/***
 * purpose: create a server socket
 * params:
 *      domain: select protocol family will use in communication
 *      type: type of socket TCP or UDP
 *      protocol: particular protocol to used
 ***/
int sct__socket(int domain, int type, int protocol){
    int sockFD = socket(domain, type, protocol);
    if (-1 == sockFD)
      throw string("cannot create socket");
    return sockFD;
}

/**
 * purpose: convert IP to binary to use of client socket
 * params:
 *      af: address family
 *      src: IP in string
 *      dst: destination of address
 **/
int sct__inet_pton(int af, const char *src, void* dst){
    int res = inet_pton(af, src, dst);
    if(0 > res){
        throw string("ERROR: first params is not valid");
    }
    else if( res == 0){
        throw string("ERROR: second params is not valid");
    }
    return res;
}

/***
 * split string base to substring based in delimiter
 * **/
vector<string> split_string(string base, string delimiter){
    vector<string> rps;
    int found = base.find(delimiter);
    while(found != string::npos){
        string substring = base.substr(0, base.find(delimiter));
        rps.push_back(substring);
        base.erase(0, base.find(delimiter) + delimiter.size());
        found = base.find(delimiter);
    }
    rps.push_back(base);
    return rps;
}
