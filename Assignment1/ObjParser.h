#ifndef __OBJPARSER_H__
#define __OBJPARSER_H__
// GLM
#include <glm/glm.hpp>
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

private:

    std::vector< glm::vec3 > vertices;
    std::vector< glm::vec3 > normals;

};

#endif