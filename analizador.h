#ifndef ANALIZADOR_COMMANDS_H
#define ANALIZADOR_COMMANDS_H

#include <string>
#include <iostream>
#include <vector>

#include "utility_connection.h"
#include "serializer.h"

using namespace std;

class AnalizadorCommands{
    private:
        JSON format_msg;
    public:
        AnalizadorCommands(){

        }
        /**
         * Analyze the command of server to convert a JSON operation.
         * ***/
        string Analize(string msg){
            this->format_msg.clean_format();
            vector<string> substring = split_string(msg, " ");
            ///// command to send msg all clients
            if(substring[0] == "send_all" and substring.size() == 2 ){
                this->format_msg.add_fields("code", substring[0]);
                this->format_msg.add_fields("msg", substring[1]);
                this->format_msg.add_fields("iterate", "-1");
                return this->format_msg.serialize();
            }else{
                cout << "Error: no es un comando valido." << endl;
                return "ERROR";
            }
        }
};




#endif
