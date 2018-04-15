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
            // x = x/30;
            // y = y/30;
            // z = z/30;
            vert = glm::vec3(x, y, z);
            vertices.push_back(vert);
        }else if (line.substr(0,3) == "vn "){
            float x,y,z;
            istringstream vs(line.substr(3));
            glm::vec3 vert;
            vs>>x; vs>>y; vs>>z;
            vert = glm::vec3(x, y, z);
            normals.push_back(vert);
        }else if (line.substr(0,3) == "vt "){
            float x,y,z;
            istringstream vs(line.substr(3));
            glm::vec3 vert;
            vs>>x; vs>>y; vs>>z;
            vert = glm::vec3(x, y, z);
            texts.push_back(vert);
        }else if (line.substr(0,2) == "f "){
            string p1, p2, p3;
            istringstream vs(line.substr(2));
            vs>>p1; vs>>p2; vs>>p3;
            stringstream ss(p1);
            face cf;
            for (int i = 0; i < 3; i++){
                string v,t,n;
                getline(ss, v, '/');
                getline(ss, t, '/');
                getline(ss, n, '/');
                vertex cv;
                cv.vIndex = stoi(v)-1;
                cv.tIndex = stoi(t)-1;
                cv.nIndex = stoi(n)-1;
                switch(i){
                    case 0:
                        cf.v1 = cv;
                        ss.clear();
                        ss.str(p2);
                        break;
                    case 1:
                        cf.v2 = cv;
                        ss.clear();
                        ss.str(p3);
                        break;
                    case 2:
                        cf.v3 = cv;
                        break;
                }
            }
            ss.clear();
            faces.push_back(cf);
        }
	}

    infile.close();

}

void ObjParser::drawObj(){
    // glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    for(unsigned int i = 0; i < faces.size(); i++){        
        // First point
        glNormal3f(normals[faces[i].v1.nIndex].x, normals[faces[i].v1.nIndex].x, normals[faces[i].v1.nIndex].x);
        glVertex3f(vertices[faces[i].v1.vIndex].x, vertices[faces[i].v1.vIndex].y, vertices[faces[i].v1.vIndex].z);
        // Second point
        glNormal3f(normals[faces[i].v2.nIndex].x, normals[faces[i].v2.nIndex].x, normals[faces[i].v2.nIndex].x);
        glVertex3f(vertices[faces[i].v2.vIndex].x, vertices[faces[i].v2.vIndex].y, vertices[faces[i].v2.vIndex].z);
        // Third point
        glNormal3f(normals[faces[i].v3.nIndex].x, normals[faces[i].v3.nIndex].x, normals[faces[i].v3.nIndex].x);
        glVertex3f(vertices[faces[i].v3.vIndex].x, vertices[faces[i].v3.vIndex].y, vertices[faces[i].v3.vIndex].z);

    }
    glEnd();

}
