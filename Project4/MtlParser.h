#ifndef __MTLPARSER_H__
#define __MTLPARSER_H__
// GLM
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>

// STL
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

class MtlParser{
public:
    MtlParser(std::string file, std::string mtl);
    ~MtlParser(){};

    void applyMaterial();

private:
    GLfloat mat_amb[4];
    GLfloat mat_dif[4];
    GLfloat mat_spe[4];
    GLfloat shin[1];
    
    std::string mtlName;
};
#endif