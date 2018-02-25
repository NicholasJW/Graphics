#ifndef __OBJPARSER_H__
#define __OBJPARSER_H__
// GLM
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

// STL
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

class ObjParser{

public:
    // Contructor and destructor
    ObjParser(std::string path);
    ~ObjParser(){};

    void drawObj();
    float getVertexMult(){return vertexMult;};
    void setVertexMult(float x){vertexMult = x;};

private:
    struct vertex{
        int vIndex;
        int tIndex;
        int nIndex;
    };

    struct face{
        vertex v1;
        vertex v2;
        vertex v3;
    };

    float vertexMult = 30;
    
    std::vector< glm::vec3 > vertices;
    std::vector< glm::vec3 > texts;
    std::vector< glm::vec3 > normals;
    std::vector< face > faces;

};

#endif