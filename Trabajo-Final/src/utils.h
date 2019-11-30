#ifndef _UTILS_H_
#define _UTILS_H_

#include <boost/algorithm/string.hpp> 
#include <netinet/in.h>

#include <sys/stat.h>
#include <utility>
#include <fstream>

#include "protocol.h"

using namespace std;

struct CInfo{//informacion del slave su port e ip
	int port;
	string ip;
};

struct CAddress{
	int socket;	//socket
    struct sockaddr_in direccion; //direccion de socket
};

string info_to_string(CInfo info){//retorna string de port e ip
	return to_string(info.port) + "|" + info.ip;
}

void conectar_enviar(CInfo &slave, string mensaje,string tipo_consulta, int bits){
    struct sockaddr_in sockAddr;
    int msock = socket(AF_INET, SOCK_STREAM, 0);
    if(msock < 0){
        perror("Can't Create Socket!");
        exit(0);
    }

    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(slave.port);
 
    int nr = inet_pton(AF_INET, slave.ip.c_str(), &sockAddr.sin_addr);
 
    if(nr <= 0){
        perror("error: Not a valid address family");
        close(msock);
        exit(0);
    }

    nr = connect(msock, (const struct sockaddr *)&sockAddr, sizeof(struct sockaddr_in));
    if(nr >= 0){
        CProtocol prot(bits);
        prot.enviar(mensaje, msock, tipo_consulta);
    }

    shutdown(msock, SHUT_RDWR);
    close(msock);
}

void configurar_esclavo(vector<string> &parse, CInfo &info, string &sql){
    if(parse.size() == 3){
        info.port=stoi(parse[0]);
        info.ip=parse[1];
        sql=parse[2];
    }
    else{
        cout<<"Colocar port, ip y consulta"<<endl;
    }
}

vector<string> dividir_mensaje(string mensaje, string _separator){
    vector<string> result; 
    boost::split(result, mensaje, boost::is_any_of(_separator));
    return result;
}

int calcular_hash(string nodo, int modulo){
    int hash = 0;
    for(char c: nodo){
        hash += c;
    }
    return hash%modulo;
}


#endif