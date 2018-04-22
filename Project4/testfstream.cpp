#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

using namespace std;

std::pair<std::string, std::string> mtlInfo = {"", ""};

int main(){
    cout << "Start testing obj parsing" << endl;
    string objFile("Key/key.obj");
    ifstream objInfile;
    objInfile.open(objFile.c_str());
    string line;
    while(getline(objInfile, line)){
        if(line.substr(0, 7) == "mtllib "){
            mtlInfo.first = line.substr(7);
            cout << mtlInfo.first.size() << endl;
            cout << mtlInfo.first << endl;
        }else if(line.substr(0, 7) == "usemtl "){
            mtlInfo.second = line.substr(7);
            cout << mtlInfo.second << endl;
        }
    }

    cout << "Start testing Key/ket.mtl" << endl;
    // string filep("Key/key.mtl");
    ifstream mtlfile;
    mtlfile.open(mtlInfo.first.c_str());
    if(mtlfile.good())
        cout << "good file" << endl;
    else{
        throw invalid_argument("Failed to open MTL file.");
    }

    mtlfile.close();
}