 
//g++ -o client client.cpp
 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <vector>
#include "utility_connection.cpp"

#define MAX_SIZE_BUFFER 10000

using namespace std;
///////////////////////////// global variables
class Client;
Client* instance;

///////////////////////////// client class

class Client {
    private:
        int connect_fd;
        string buffer;
        bool is_connected = true;
    public:
        ////// constructor
        Client(int fd):connect_fd(fd){
            instance = this;
        }
};

////////////////////////// start clients
void client_start(){
        try{
            struct sockaddr_in sock_addr;
            int sock_fd, port;
            string id;
            /// create socket
            sock_fd = sct__socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

            //// get data to connect socket
            cout << "se creara una connection\nPase el ID del servidor:\n";
            cin >> id;
            cout << "Ahora el puerto:\n";
            cin >> port;
            //// convert variable
            memset(&sock_addr, 0, sizeof(struct sockaddr_in));
            sock_addr.sin_family = AF_INET;
            sock_addr.sin_port = htons(port);
            sct__inet_pton(AF_INET, id.c_str(), &sock_addr.sin_addr);
            //// create socket in server
            sct__connect(sock_fd, (struct sockaddr *)&sock_addr, sizeof(struct sockaddr_in));
            cout << "se creo la connection in server" << endl;
            ///// set up game
            instance = new Client(sock_fd);

            //// close connection
            shutdown(sock_fd, SHUT_RDWR);
            close(sock_fd);
    }catch(string error_msg){
            perror(error_msg.c_str());
            exit(EXIT_FAILURE);
    }
}

/////////////////////// main function
int main(int argc, const char** argv){	
    client_start();
	return 0;
}

   
