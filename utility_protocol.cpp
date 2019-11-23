#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <vector>
#include <signal.h>

using namespace std;

/***
 * split string base to substring based in delimiter
 * **/
vector<string> split_string(string base, string delimiter){
    vector<string> rps;
    int found = base.find(delimiter);
    while(found != string::npos){
        string substring = base.substr(0, base.find(delimiter));
        rps.push_back(substring);
        base.erase(0, base.find(delimiter) + delimiter.size());
        found = base.find(delimiter);
    }
    rps.push_back(base);
    return rps;
}