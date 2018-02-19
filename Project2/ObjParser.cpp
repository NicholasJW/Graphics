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
            x = x/vertexMult;
            y = y/vertexMult;
            z = z/vertexMult;
            vert = glm::vec3(x, y, z);
            vertices.push_back(vert);
        }else if (line.substr(0,2) == "f "){
            line = cleanString(line);
            int a,b,c;
            istringstream vs(line.substr(2));
            glm::vec3 indices;
            vs>>a; vs>>b; vs>>c;
            indices = glm::vec3(a, b, c);
            faces.push_back(indices);
        }//So far only care about vertex and face   
	}
    infile.close();

    // Calculate normals
    for(unsigned int i = 0; i < faces.size(); i++){
        glm::vec3 p1 = vertices[faces.at(i).x - 1];
        glm::vec3 p2 = vertices[faces.at(i).y - 1];
        glm::vec3 p3 = vertices[faces.at(i).z - 1];

        glm::vec3 u = p2 - p1;
        glm::vec3 v = p3 - p1;
        
        glm::vec3 norm(u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x);
        float mag = pow(norm.x, 2) + pow(norm.y, 2) + pow(norm.z, 2);
        mag = sqrt(mag);
        norm.x = (norm.x)/mag;
        norm.y = (norm.y)/mag;
        norm.z = (norm.z)/mag;
        normals.push_back(norm);
    }

    if(!(normals.size() == faces.size())){
        std::cerr << "Something funky when parsing." << std::endl;
    }
}

std::string ObjParser::cleanString(std::string s){
    std::string theS = s;
    char lookFor = '/';
    int start = -1;
    int length = 0;
    for(unsigned int i = 0; i < theS.length(); i++){
        if (theS[i] == '/'){
            if (lookFor == '/'){
                lookFor = ' ';
                start = i;
            }
            length ++;
        }else if (theS[i] == ' '){
            if (lookFor == ' '){
                length ++;
                theS = theS.substr(0, start) + ' ' +  theS.substr(start+length - 1);
                lookFor = '/';
                start = -1;
                length = 0;
            }
        }else{
            if (lookFor == ' '){
                length ++;
            }
        }
    }
    
    if(theS.find("/") != std::string::npos){
        theS = theS.substr(0, theS.find("/"));
    }

    return theS;
}   