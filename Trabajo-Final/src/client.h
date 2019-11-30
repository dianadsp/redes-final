#ifndef _FRONT_H_
#define _FRONT_H_

#include <vector>
#include <string.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <utility>

#include "protocol.h"
#include "utils.h"

using namespace std;

class CClient{
	public:
		static int m_bits;
		static vector<CInfo> m_listSlaves;//vector de esclavos port e ip
		static CInfo m_info;// info <port, ip>
		static CAddress m_conexion;//CAdress contiene socket y struct de socket (utils.h)
		static int m_profundidad;
		static vector<pair<string, string> > result_profundidad;
	
		static void comunicacion();
		static void cargar_grafo();
		static void leer_consulta(string);
		static void escuchar();

		void inicializar();
		void crear_slaves();
		CClient(int, string);
		~CClient();
};

int CClient::m_profundidad;
int CClient::m_bits;
vector<CInfo> CClient::m_listSlaves;
CInfo CClient::m_info;
CAddress CClient::m_conexion;
vector<pair<string, string> > CClient::result_profundidad;

CClient::CClient(int _port, string _ip){
	this->m_bits = 64;
	this->m_info = {_port, _ip};
	this->m_conexion.socket = socket(AF_INET, SOCK_STREAM, 0);
	this->m_profundidad = 0;
}

void CClient::crear_slaves(){	
	int n_slaves;
	cout <<"Ingrese numero de slaves: "; 
	cin >> n_slaves;

	int port;
	string ip;
	for(int i=0; i<n_slaves; i++){
		cout << "SLAVE "<< i+1 <<endl;
		cout << "\tPort: "; 
		cin >> port;
		cout << "\tIP: "; 
		cin >> ip;

		m_listSlaves.push_back({port, ip});
	}
}

void CClient::cargar_grafo(){
	string frase="I N U 1\nI N B 2\nI N A 3\nI N D 4\nI N C 5\nI N T 6\nI N E 7\nI R U B\nI R U D\nI R B A\nI R B D\nI R D C\nI R D T";	
	vector<string> grafo = dividir_mensaje(frase, "\n");
	string query;
	for(uint i=0; i<grafo.size(); i++){
		query = grafo[i];
		cout<<i<<"->"<<query<<endl;
		leer_consulta(query);
	}
}

void CClient::leer_consulta(string query){
	vector<string> parse;
	string sql;
	int numHash;
	bool state = true;

		exit(0);
	if(query == "exit"){
	}
	else{
		parse = dividir_mensaje(query, " ");
		switch(parse.size()){

			case 4:{
				switch(query[0]){
					case 'I':{
						numHash = calcular_hash(parse[2], m_listSlaves.size());
						if(parse[1] == "N"){
							sql = "|insert into NODE(NAME, ATTRIBUTE) VALUES ('" + parse[2] + "', '" + parse[3] +"');";//formo consulta sql, para enviar a sqlite
							conectar_enviar(m_listSlaves[numHash], info_to_string(m_info) + sql, "I", m_bits);
						}
						else if(parse[1] == "R"){
							numHash = calcular_hash(parse[2], m_listSlaves.size());
							sql = "|insert into RELATION(NAME1, NAME2) VALUES ('" + parse[2] + "', '" + parse[3] +"');";
							conectar_enviar(m_listSlaves[numHash], info_to_string(m_info) + sql, "I", m_bits);

							numHash = calcular_hash(parse[3], m_listSlaves.size());
							sql = "|insert into RELATION(NAME1, NAME2) VALUES ('" + parse[3] + "', '" + parse[2] +"');";
							conectar_enviar(m_listSlaves[numHash], info_to_string(m_info) + sql, "I", m_bits);
						}
						else{
							cout << "Ingresaste mal la consulta";
						}
						break;
					}
					//
					/*case 'Q':{
						numHash= mhash(parse[2], m_listSlaves.size());
						if(parse[1] == "R"){
							m_profundidad=stoi(parse[3]);
							result_profundidad.clear();
							result_profundidad.push_back({"]", parse[2]});
							sql = "|select * from RELATION where NAME1 = '|" + parse[2];
							conectar_enviar(m_listSlaves[numHash], info_to_string(m_info) + sql, "R", m_bits);
						}
						break;
					}*/
				}
				break;
			}
		}
	}
}

void CClient::comunicacion(){
	string comando;
	comando = "";
	while(true){
		cout << ">> ";
		getline(cin, comando);
		if(comando == "cargar"){
			cargar_grafo();
		}
		else{
			leer_consulta(comando);
		}
	}
}

void CClient::escuchar(){
	socklen_t qsize = sizeof(sockaddr_in);
    CAddress conexion;
    string comando, sql;

    CProtocol prot(m_bits);
    CInfo tinfo;

    while(true){
        conexion.socket = accept(m_conexion.socket, (struct sockaddr *) &conexion.direccion, &qsize);
        if(conexion.socket < 0)
            perror("Error on accept");
        else{
            comando = prot.recibir(conexion.socket);//comando si creo con error o no
			//cout<<"COMANDO: "<<comando<<endl;
            switch(comando[0]){
                case 'O':{
					cout << "   " << comando.substr(1) << " :<Ok>"<<endl;
                    break;
                }
                case 'E':{
                    cout << "   " << comando.substr(1) << " :<Error>"<<endl;
                    break;
                }                
                default:
                    break;
            }
        }
        shutdown(conexion.socket, SHUT_RDWR);
        close(conexion.socket);
    }

    shutdown(m_conexion.socket, SHUT_RDWR);
    close(m_conexion.socket);
}

void CClient::inicializar(){
	int reuse = 1;
    memset(&m_conexion.direccion, 0, sizeof(sockaddr_in));

    m_conexion.direccion.sin_family = AF_INET;
    m_conexion.direccion.sin_addr.s_addr = INADDR_ANY;
    m_conexion.direccion.sin_port = htons(m_info.port);

    setsockopt(m_conexion.socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));//reusa direccion y port

    int nr = bind(m_conexion.socket, (struct sockaddr *) &m_conexion.direccion, sizeof(sockaddr_in));
    if(nr < 0){
        perror("Failed to bind");
    }
    listen(m_conexion.socket, 10);

	crear_slaves();

	thread tconversar(comunicacion);
    thread tescuchar(escuchar);

    tconversar.join();
    tescuchar.join();

    close(m_conexion.socket);
}

CClient::~CClient(){
	m_listSlaves.clear();
}

#endif