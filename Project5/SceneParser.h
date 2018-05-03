#ifndef __SCENEPARSER_H__
#define __SCENEPARSER_H__
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "RayTracer.h"

class SceneParser{
private:
    std::vector<Sphere> objs;
    int depth;
    Vec3f ambient;
    float dist;
    void parseGlobal(std::ifstream& infile);
    void parseSphere(std::ifstream& infile);
    void parseLight(std::ifstream& infile);


public:
    SceneParser(const char* scenefile);
    std::vector<Sphere>& getSpheres(){return objs;};
    int getDepth(){return depth;};
    Vec3f getAmbient(){return ambient;};
    float getCamDist(){return dist;};
};

#endif