#ifndef _UTILS_H_
#define _UTILS_H_
#include <netinet/in.h>
#include <boost/algorithm/string.hpp> 

#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <sstream>
#include <string>
#include "protocolo.h"

using namespace std;
struct CAddress{
	int m_socket; //socket
    struct sockaddr_in m_direccion;//direccion de socker
};

vector<string> dividir_mensaje(string query, string separador){
    vector<string > parse;
    boost::split(parse,query, boost::is_any_of(separador));
    return parse;
}

string leer_grafo(string filename){
    string palabra;
    ifstream MyReadFile(filename);
    while (getline (MyReadFile, palabra)) {
        cout << palabra;
    }
    MyReadFile.close();
    //return res;
}

int calcular_hash(string nodo, int modulo){
    int thash = 0;
    for(char c: nodo){
        thash += c;
    }
    //cout<<"valor letra"<<thash<<endl;
    return thash%modulo;
}

string toString(int port,string ip){
    return to_string(port) + "|" + ip;
}

void conectar_enviar(pair<int,string> slave,string mensaje,int bits, string tipo_consulta){
    struct sockaddr_in sockAddr;
    //ESTABLECER CONEXION
    int mSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(mSocket < 0){
        perror("No puedo crear socket");
        exit(0);
    }

    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(slave.first);
    
    //cout<<slave.first<<"--"<<slave.second<<endl;

    int nr = inet_pton(AF_INET, slave.second.c_str(), &sockAddr.sin_addr);
    //cout<<"nr: "<<nr<<endl;
 
    if (0 > nr){
        perror("error: first parameter is not a valid address family");
        close(mSocket);
        exit(EXIT_FAILURE);
    }
    else if (0 == nr){
        perror("char string (second parameter does not contain valid ipaddress");
        close(mSocket);
        exit(EXIT_FAILURE);
    }

    //ENVIAR MENSAJE CON PROTOCOLO
    nr = connect(mSocket, (const struct sockaddr *)&sockAddr, sizeof(struct sockaddr_in));
    if(nr ==-1){
        CProtocol prot(bits);
        prot.enviar(mensaje, mSocket, tipo_consulta);
    }

    shutdown(mSocket, SHUT_RDWR);
    close(mSocket);

}

#endif