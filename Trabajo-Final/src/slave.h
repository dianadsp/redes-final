#ifndef _SLAVE_H_
#define _SLAVE_H_

#include <string.h>
#include <iostream>

#include "database.h"
#include "protocol.h"
#include "utils.h"

using namespace std;

class CSlave{
    public:
        int bits;
        CInfo m_info; //port , ip
        CAddress m_conexion; //CAdress contiene socket y struct de socket (utils.h)
        CDatabase *m_db; //puntero a BD

        void escuchar();
        void inicializar();

        CSlave(int, string, string);
        ~CSlave();
};

CSlave::CSlave(int port, string ip, string dbname){
	this->bits = 64;
	this->m_info = {port, ip};
	this->m_conexion.socket = socket(AF_INET, SOCK_STREAM, 0);
    this->m_db = new CDatabase(dbname);
}


void CSlave::escuchar(){
    socklen_t qsize = sizeof(sockaddr_in);
    CAddress conexion;
    string comando, sql;

    vector<string> parse;

    CInfo info;
    CProtocol prot(bits);    

    while(true){
        conexion.socket = accept(m_conexion.socket, (struct sockaddr *) &conexion.direccion, &qsize);
        if(conexion.socket < 0)
            perror("Error on accept");
        else{           
            comando = prot.recibir(conexion.socket);
            cout << comando << "\n";
            switch(comando[0]){
                case 'I':{
                    parse = dividir_mensaje(comando.substr(1), "|");
	    			configurar_esclavo(parse, info, sql);
                    if(m_db->execute(sql)){
                        conectar_enviar(info, "Create", "O", bits);
                    }
                    else{
                        conectar_enviar(info, "Create", "E", bits);
                    }
                    break;
                }                
            }
        }
        shutdown(conexion.socket, SHUT_RDWR);
        close(conexion.socket);
    }

    shutdown(m_conexion.socket, SHUT_RDWR);
    close(m_conexion.socket);
}

void CSlave::inicializar(){
	int reuse = 1;
    memset(&m_conexion.direccion, 0, sizeof(sockaddr_in));

    m_conexion.direccion.sin_family = AF_INET;
    m_conexion.direccion.sin_addr.s_addr = INADDR_ANY;
    m_conexion.direccion.sin_port = htons(m_info.port);

    setsockopt(m_conexion.socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));

    int nr = bind(m_conexion.socket, (struct sockaddr *) &m_conexion.direccion, sizeof(sockaddr_in));
    if(nr < 0){
        perror("Failed to bind");
    }
    listen(m_conexion.socket, 10);
    
	escuchar();
}

CSlave::~CSlave(){
}
#endif