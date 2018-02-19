#ifndef __OBJPARSER_H__
#define __OBJPARSER_H__
// GLM
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
    const std::vector< glm::vec3 >& getVertices(){return vertices;};
    std::vector< glm::vec3 > getFaces(){return faces;};
    std::vector< glm::vec3 > getNormals(){return normals;};
    
private:
    int vertexMult = 5;
    // Private helper method
    std::string cleanString(std::string s);
    std::vector< glm::vec3 > vertices;
    std::vector< glm::vec3 > faces;
    std::vector< glm::vec3 > normals;

};

#endif