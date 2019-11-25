#ifndef _PROTOCOLO_H
#define _PROTOCOLO_H

#include <vector>
#include <iostream>

using namespace std;

class CProtocol{
    public:
        int tam_buffer;
        int bits;
        
        CProtocol(int);
        void enviar(string,int,string);//mensaje, socket,tipo de consulta
        vector<string> dividir_query(string);
};

CProtocol::CProtocol(int _tam){
    this->tam_buffer=_tam;
    this->bits=to_string(_tam).size();
}

vector<string> CProtocol::dividir_query(string mensaje){
    //-----------------completar
}

void CProtocol::enviar(string mensaje,int socket,string tipo_sql){
    string tmp=tipo_sql+mensaje;
    //-----------------completar

}


#endif