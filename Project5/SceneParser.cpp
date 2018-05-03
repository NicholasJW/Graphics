#include "SceneParser.h"
#include <iostream>

using namespace std;
SceneParser::SceneParser(const char* scenefile){

    ifstream infile;
    infile.open(scenefile);
    if(!infile.good()){
        throw invalid_argument("Failed to open scene file.");
    }

    string line;
    while(getline(infile, line)){
        // cout << line << endl;
        if(line.substr(0, 7) == "global:"){
            parseGlobal(infile);
        }else if(line.substr(0, 2) == "s:"){
            parseSphere(infile);
        }else if(line.substr(0, 2) == "l:"){
            parseLight(infile);
        }
    }
    infile.close();
}

void SceneParser::parseGlobal(std::ifstream& infile){
    string line;
    while(getline(infile, line)){
        if(line[1] == '#') continue;

        if(line.substr(0, 6) == "depth "){
            depth = stoi(line.substr(6));
        }else if(line.substr(0, 8) == "camDist "){
            dist = stof(line.substr(8));
        }else if(line.substr(0, 9) == "endGlobal"){
            break;
        }
    }    
}

void SceneParser::parseSphere(std::ifstream& infile){
    string line;
    Vec3f pos;
    float radius;
    Vec3f color;
    float ref;
    float tra;
    while(getline(infile, line)){
        if(line[1] == '#') continue;

        if(line.substr(0, 4) == "pos "){
            float x, y, z;
            istringstream vs(line.substr(4));
            vs>>x, vs>>y, vs>>z;
            pos = Vec3f(x, y, z);
        }else if(line.substr(0, 4) == "rad "){
            radius = stof(line.substr(4));
        }else if(line.substr(0, 4) == "col "){
            float x, y, z;
            istringstream vs(line.substr(4));
            vs>>x, vs>>y, vs>>z;
            color = Vec3f(x, y, z);
        }else if(line.substr(0, 4) == "ref "){
            ref = stof(line.substr(4));
        }else if(line.substr(0, 4) == "tra "){
            tra = stof(line.substr(4));
        }else if(line.substr(0, 9) == "endSphere"){
            break;
        }
    }    
    objs.push_back(Sphere(pos, radius, color, ref, tra)); 
}

void SceneParser::parseLight(std::ifstream& infile){
    string line;
    Vec3f pos;
    Vec3f amb;
    Vec3f dif;
    Vec3f spe;
    while(getline(infile, line)){
        if(line[1] == '#') continue;

        if(line.substr(0, 4) == "pos "){
            float x, y, z;
            istringstream vs(line.substr(4));
            vs>>x, vs>>y, vs>>z;
            pos = Vec3f(x, y, z);
        }else if(line.substr(0, 4) == "amb "){
            float x, y, z;
            istringstream vs(line.substr(4));
            vs>>x, vs>>y, vs>>z;
            amb = Vec3f(x, y, z);
        }else if(line.substr(0, 4) == "dif "){
            float x, y, z;
            istringstream vs(line.substr(4));
            vs>>x, vs>>y, vs>>z;
            dif = Vec3f(x, y, z);
        }else if(line.substr(0, 4) == "spe "){
            float x, y, z;
            istringstream vs(line.substr(4));
            vs>>x, vs>>y, vs>>z;
            spe = Vec3f(x, y, z);
        }else if(line.substr(0, 8) == "endLight"){
            break;
        }
    }
    
    ambient += amb;
    objs.push_back(Sphere(pos, 0.5, Vec3f(0.0), 0, 0, dif));
    // objs.push_back(Sphere(Vec3f( 30.0,  0, 0),  0.5, Vec3f(0.00, 0.00, 0.00), 0, 0.0, Vec3f(1)));
    cout << dif.x << endl; 
    cout << dif.y << endl; 
    cout << dif.z << endl; 
}
