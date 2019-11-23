#include <string>
#include <iostream>
#include <vector>

#include "serializer.h"
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
                return this->slave_action();
            }
            return this->master_action();
        }
        /**
         * interface: action to take in master protocol
         ***/
        string master_action(){
            string rps;
            string code = this->format.get_field("code");
            ///// operation - send msg to all client ( iterate 1)
            if(code == "send_all"){
                if(this->format.get_field("iterate") == "-1"){
                    this->format.set_field("iterate", "0");
                    rps = this->send_all_master();
                }
            }else{
                rps = "Error en la respuesta";  
            }
            return rps;
        }
        /**
         * interface: action to take in slave protocol
         * **/
        string slave_action(){
            string rps;
            string code = this->format.get_field("code");
            if(code == "send_all"){
                if(this->format.get_field("iterate") == "0"){
                    this->format.set_field("iterate", "1");
                    cout << "todos recibimos este msg: "<< this->format.get_field("msg") << endl;
                    string send_msg = this->format.serialize();
                    sct__write(this->socket_fd, send_msg.c_str(), send_msg.size());
                    rps = "Exito en la operacion";
                }
                else if(this->format.get_field("iterate") == "1"){
                    ///cout << "finalizo la operacion send_all (2)" << endl;
                    rps = "ok";
                }
            }else{
                rps = "Error en la respuesta";  
            }
            return rps;
        }
        //////////////// utility functions
        string send_all_master(){
                string msg = this->format.serialize();
                int size_neighbors = this->ptr_neighbors->size();
                for(int idx = 0 ; idx != size_neighbors; idx++ ){
                    sct__write((*(this->ptr_neighbors))[idx] , msg.c_str(), msg.size() );
                }
                return "Exito en la operacion parte (1) send_all";
        }
};