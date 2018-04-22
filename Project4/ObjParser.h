#ifndef __OBJPARSER_H__
#define __OBJPARSER_H__

// MTL Parser
#include "MtlParser.h"
class ObjParser{

public:
    // Contructor and destructor
    ObjParser(std::string path);
    ~ObjParser(){};

    void drawObj();
    void applyMtl(){mp->applyMaterial();};
    // float getVertexMult(){return vertexMult;};
    // void setVertexMult(float x){vertexMult = x;};

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

    std::vector< glm::vec3 > vertices;
    std::vector< std::pair<float, float> > texts;
    std::vector< glm::vec3 > normals;
    std::vector< face > faces;
    std::pair<std::string, std::string> mtlInfo = {"", ""};
    MtlParser* mp;
};

#endif