#include <string>
#include <iostream>

#include "serializer.cpp"

using namespace std;


class Protocol{
    private:
        int socket_fd;
        string buffer;
        bool slave;
    public:
        Protocol(int fd, bool is_slave = true){
            this->socket_fd = fd;
            this->slave = is_slave;
        }
        /**
         * action to take in master protocol
         ***/
        string master_action(string msg){

        }
        /**
         * action to take in slave protocol
         * **/
        string slave_action(string msg){

        }
};