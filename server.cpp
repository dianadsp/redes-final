/***
    Author(s): Carlos Chicata, Diana Patiño, Melissa Zevallos.
    Last Update: 29/oct/2019

    Title:
        Server module
    
    Purpose:
        - To be the master server in distributed DB of graph.
    
    Funcionality:
        - manage connections of clients.

    HOW TO WORK:
        - add the Port and fun it!
    
    HOW TO COMPILE:
        - g++ server.cpp -o server -pthread -std=c++11
***/ 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <set>
#include <iostream>

#include "protocol.cpp"
#include "utility_connection.h"
#include "analizador.h"

#define MAX_SIZE_BUFFER 10000

using namespace std;

/////////// definetion of head methods and class
class Connection;

/////////// global variables of file
vector<Connection *> slaves;
vector<int> fd_slaves;
bool is_connected = true;
set<string> unique_nicknames;
AnalizadorCommands operator_command;
Protocol* protocol_server = new Protocol(0, false, &fd_slaves);

////////////// definition of classes and methods

/**
 * terminal to analyze and interprete the commands from user
 * **/
void terminal_user(){
            while(is_connected){
                string msg_user = "";
                cout << ">> ingresa el comando:" << endl;
                getline(cin, msg_user);
                if(msg_user == "bye-bye"){
                    cout << "Adios" << endl;
                    is_connected = false;
                    return ;
                }
                string operation = operator_command.Analize(msg_user);
                if(operation == "ERROR"){
                    cout << "ERROR: Operacion rechazada." << endl;
                }else{
                   cout << protocol_server->take_action(operation) << endl;
                }
            };
            return ;    
}

/***
 * Connection class
 *  Purpose:
 *      manage the connections of slaves
 * ***/
class Connection{
    private:
        int connection_fd;
        string nickname = "";
        bool is_actived = true;
        Protocol* protocolo;
    public:
        Connection(int fd): connection_fd(fd){
            slaves.push_back(this);
            fd_slaves.push_back(fd);
            this->protocolo = new Protocol(fd, true, 0);
        }
        void receive_msg_from_user(){
            do{
                string buffer(MAX_SIZE_BUFFER, '\0');
                sct__read(this->connection_fd, (char *)buffer.c_str(), MAX_SIZE_BUFFER);
                this->protocolo->take_action(buffer);
            }while(this->is_actived);
        }

};

/***
 * start_server method
 *  Purpose:
 *      - start of master server
 * ***/
int start_server(int port, string ip){
    try{
        //// preparing to connect with server
        int socket_fd;
        struct sockaddr_in sock_addr;
		socket_fd = sct__socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

		memset(&sock_addr, 0, sizeof(struct sockaddr_in));
		sock_addr.sin_family = AF_INET;
		sock_addr.sin_port = htons(port);
		sock_addr.sin_addr.s_addr = INADDR_ANY;

		sct__bind(socket_fd, (struct sockaddr*)&sock_addr, sizeof(struct sockaddr_in));
		sct__listen(socket_fd, 10);
		cout << "Abrimos el servidor!" << endl;
        thread th_user(terminal_user);
		while(is_connected){
			int connect_fd;
			connect_fd = sct__accept(socket_fd, NULL, NULL);
			cout << " ----> conectamos un nuevo cliente: " << connect_fd << endl;
			Connection* c = new Connection(connect_fd);
            thread th(&Connection::receive_msg_from_user, c);
            th.detach();
            cout << ">> ingresa el comando:" << endl;
		}
		close(socket_fd);
    }catch(string msg_error){
        perror(msg_error.c_str());
        exit(EXIT_FAILURE);
    }
}


/////////// main function
int main(int argc, const char** argv){
	int port = 0;
    cout << "Pasar datos necesario del servidor maestro." << endl;
	string ip = "";
	cout << "Pasar el puerto:" << endl;
	cin >> port;
    start_server(port, ip);
	return 0;
}
