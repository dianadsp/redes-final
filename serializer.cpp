#include <string>
#include <sstream>
#include <iostream>
#include <map>

using namespace std;

class JSON{
    private:
        map<string, string> fields;
    public:
        JSON(){

        }
        void add_fields(string name, string value){
            this->fields[name] = value;
            return ;
        }
        string serialize(){
            string rps = "{\n";
            for(){}
            return rps;
        }
        void deserialize(string){

        }
};