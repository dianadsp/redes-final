#include <iostream>
#include "src/client.h"

using namespace std;

int main(int argc, char const *argv[]){
    int port=8000;
    string ip="127.0.0.1";

    CClient *client=new CClient(port,ip);
    client->inicializar();

    delete client;
    return 0;
}
