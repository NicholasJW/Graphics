// For implementing this class I learned from http://www.videotutorialsrock.com/opengl_tutorial/particle_system/home.php

#ifndef __ParticleSystem_H__
#define __ParticleSystem_H__
// STL
#include <cmath>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class ParticleSystem{
private:
    float ptMass;
    glm::vec3 acceleration;
    // Record current time
    std::chrono::high_resolution_clock::time_point cTime;

    struct Particle{
	    glm::vec3 pos;
	    glm::vec3 vel;
	    glm::vec3 col;
    };
    int maxNumParts;
    std::vector<Particle> parts;
    
    // Random float from 0 to 10
    float randomStart() {
	    return (float) (rand() / ((float)RAND_MAX + 1))*10;
    };

    // glm::vec3 getVelo();
    void getNewAcce();

public:
    std::vector<glm::vec3> forces;
    ParticleSystem(){};
    ParticleSystem(int _maxNumParts, float _mass, std::vector<glm::vec3> _forces);

    void setMaxNum(int _maxNumParts){ maxNumParts = _maxNumParts;};
    void setMass(int _mass){ ptMass = _mass;};
    void initializeParticles();
    void draw();
    void update();


};

#endif