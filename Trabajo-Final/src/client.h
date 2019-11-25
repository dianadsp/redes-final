#ifndef _CLIENT_H_
#define _CLIENT_H_

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
#include <utility>
#include "utils.h"

using namespace std;

class CClient{
    public:
        int m_profundidad;
        int m_replicar;
        int bits;
        int port;
        string ip;
        vector<pair<int,string > > m_listSlaves; //vector de esclavos pair<port,ip>
        CAddress m_conexion; //CAdress contiene socket y struct de socket (utils.h)
        vector<pair<string,string> > m_resultados;

        CClient();
        CClient(int,string);
        ~CClient(){};

        void inicializar();
        void crear_slaves();
        void comunicacion();
        void leer_consulta(string);
        void escuchar();
        void cargar_grafo();

};

CClient::CClient(int _port, string _ip){
    this->port=_port;
    this->ip=_ip;
    this->bits=64;
    this->m_profundidad=0;
    this->m_replicar=0;
    this->m_conexion.m_socket=socket(PF_INET,SOCK_STREAM,0);
}

void CClient::crear_slaves(){
    int n_slave,port;//n_slave:numero de slave
    string ip;
    cout<<"Ingrese numero de slaves: ";
    cin>>n_slave;
    pair<int, string > slave;
    
    for (int i = 0; i < n_slave; i++){
        cout<<"SLAVE "<<i+1<<endl;
        cout<<"\tPort: ";
        cin>>port;
        cout<<"\tIP: ";
        cin>>ip;
        slave.first=port;
        slave.second=ip;
        
        m_listSlaves.push_back(slave);
    }    
}

void CClient::cargar_grafo(){
    string frase="I N U 1\nI N B 2\nI N A 3\nI N D 4\nI N C 5\nI N T 6\nI N E 7\nI R U B\nI R U D\nI R B A\nI R B D\nI R D C\nI R D T";
    vector<string > grafo=dividir_mensaje(frase,"\n");
    string query;
    for (int i = 0; i < grafo.size(); i++){
        query=grafo[i];
        cout<<i<<"->"<<query<<endl;
        leer_consulta(query);
    }
}

void CClient::leer_consulta(string query){
    vector<string> parse;
    int numHash;
    parse=dividir_mensaje(query," ");
    string sql;
    switch(parse.size()){
        case 4:{
            switch(query[0]){
                case 'I':{
                    numHash=calcular_hash(parse[2],m_listSlaves.size());
                    if (parse[1]=="N"){
                        sql="|insert into NODE(NAME, ATTRIBUTE) VALUES ('" + parse[2] + "', '" + parse[3] +"');";
                        conectar_enviar(m_listSlaves[numHash],toString(port,ip)+sql,bits,"I");
                    }
                    
                }
            }
        }
    }    
}

void CClient::comunicacion(){
    string comando;
    while(true){
        cout<<">> ";
        getline(cin,comando);
        if(comando=="cargar"){
            cargar_grafo();
        }
        else{
            leer_consulta(comando);//lee consulta y lo va dividiendo 
        }
    }   
}

void CClient::escuchar(){

}

void CClient::inicializar(){
    memset(&m_conexion.m_direccion,0,sizeof(struct sockaddr_in));

    m_conexion.m_direccion.sin_family=AF_INET;
    m_conexion.m_direccion.sin_port=htons(port);
    m_conexion.m_direccion.sin_addr.s_addr=INADDR_ANY;

    bind(m_conexion.m_socket,(const struct sockaddr *)& m_conexion.m_direccion,sizeof(struct sockaddr_in));
    listen(m_conexion.m_socket,10);

    crear_slaves();

    thread conversar(&CClient::comunicacion,this);
    thread listen(&CClient::escuchar,this);

    conversar.join();
    listen.join();

}
#endif