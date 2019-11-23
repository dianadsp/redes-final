#include <string>
#include <iostream>
#include <vector>

#include "serializer.cpp"
#include "utility_connection.h"

using namespace std;


class Protocol{
    private:
        int socket_fd;
        string buffer;
        bool slave;
        JSON format;
        vector<int> * ptr_neighbors;
    public:
        Protocol(int fd, bool is_slave, vector<int>* neighhooks){
            this->socket_fd = fd;
            this->slave = is_slave;
            this->ptr_neighbors = neighhooks;
        }
        /***
         * interface: to take action based in type of part in architecture
         * ***/
        string take_action(string msg){
            this->format.deserialize(msg);
            if(this->slave){
                return this->slave_action(msg);
            }
            return this->master_action(msg);
        }
        /**
         * interface: action to take in master protocol
         ***/
        string master_action(string msg){
            string rps;
            string code = this->format.get_field("code");
            if(code == "print_all"){
                rps = this->print_all_master();
            }else{
                rps = "Error en la respuesta";  
            }
            return rps;
        }
        /**
         * interface: action to take in slave protocol
         * **/
        string slave_action(string msg){
            string rps;
            string code = this->format.get_field("code");
            if(code == "print_all"){
                cout << this->format.get_field("msg") << endl;
                rps = "Exito en la operacion";
            }else{
                rps = "Error en la respuesta";  
            }
            return rps;
        }
        //////////////// utility functions
        string print_all_master(){
                string msg = this->format.serialize();
                int size_neighbors = this->ptr_neighbors->size();
                for(int idx = 0 ; idx != size_neighbors; idx++ ){
                    sct__write((*(this->ptr_neighbors))[idx] , msg.c_str(), msg.size() );
                }
                return "Exito en la operacion";
        }
};