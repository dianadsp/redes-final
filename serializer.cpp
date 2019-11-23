#include <string>
#include <sstream>
#include <iostream>
#include <map>

#include "utility_protocol.cpp"

using namespace std;

class JSON{
    private:
        map<string, string> fields;
    public:
        JSON(){

        }
        /**
         * add fields in JSON format of data
         * **/
        void add_fields(string name, string value){
            this->fields[name] = value;
            return ;
        }
        /**
         * serialize JSON data to normalized string
         * **/
        string serialize(){
            string rps = "{\n";
            map<string, string>::iterator it = this->fields.begin();
            for(; it != fields.end(); it++){
                rps += it->first + " : " + it->second + ",\n";
            }
            rps += "}";
            return rps;
        }
        /**
         * deserialize normalized string to JSON data
         * **/
        void deserialize(string data){
            this->fields.clear();
            vector<string> rps = split_string(data, "\n");
            for(int i = 1; i < rps.size() - 1 ; i++ ){
                vector<string> data_fields = split_string(rps[i], " : ");
                this->fields[data_fields[0]] = data_fields[1];
            }
            return ;
        }
        /**
         * display JSON to console
         * **/
        void display_console(){
            string rps = this->serialize();
            cout << rps << endl;
            return ;
        }
        /**
         * get value of field
         * **/
        string get_field(string field){
            return this->fields[field];
        }
        /**
         * set value of field
         * **/
        string set_field(string field, string value){
            return this->fields[field] = value;
        }
};