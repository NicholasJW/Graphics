#include "ObjParser.h"

ObjParser::ObjParser(std::string path){
    using namespace std;

    // Open the file
    ifstream infile;
	infile.open(path.c_str());
	if(!infile.good()){
		throw invalid_argument("Failed to open OBJ file.");
	}
    string line;

    while(getline(infile, line)){
		if (line.substr(0,2) == "v "){
            float x,y,z;
            istringstream vs(line.substr(2));
            glm::vec3 vert;
            vs>>x; vs>>y; vs>>z;
            vert = glm::vec3(x, y, z);
            vertices.push_back(vert);
        }else if (line.substr(0,2) == "f "){
            int a,b,c;
            istringstream vs(line.substr(2));
            glm::vec3 indices;
            vs>>a; vs>>b; vs>>c;
            indices = glm::vec3(a, b, c);
            normals.push_back(indices);
        }//So far only care about vertex and face
        
	}


    infile.close();
}