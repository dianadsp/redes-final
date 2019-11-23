#include <string>
#include <iostream>

#include "serializer.cpp"
#include "utility_connection.h"

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
        /***
         * interface: to take action based in type of part in architecture
         * ***/
        string take_action(string msg){
            if(this->slave){
                return this->slave_action(msg);
            }
            return this->master_action(msg);
        }
        /**
         * interface: action to take in master protocol
         ***/
        string master_action(string msg){

        }
        /**
         * interface: action to take in slave protocol
         * **/
        string slave_action(string msg){

        }
};