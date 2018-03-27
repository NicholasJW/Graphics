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
    float period;
    struct ExtraForce{
        float gravity = 0;
        float hWind = 0;
        float gSpeed = 0;
    };
    
    // glm::vec3 acceleration;
    struct Particle{
	    glm::vec3 pos;
	    glm::vec3 vel;
	    glm::vec3 col;
        glm::vec3 acc;
    };
    int maxNumParts;
    std::vector<Particle> parts;
    
    // Random float from 0 to 10
    float randomStart() {
	    return (float) (rand() / ((float)RAND_MAX))*10;
    };

    // glm::vec3 getVelo();
    void getNewAcce(Particle &p);
    bool checkPos(Particle p);

public:
    bool useCol = false;
    glm::vec3 color;
    std::vector<glm::vec3> forces;
    std::vector<glm::vec4> attractors;
    std::vector<glm::vec4> repulsers;
    std::vector<glm::vec4> generators;
    
    ParticleSystem(){};
    ParticleSystem(int _maxNumParts, float _mass, std::vector<glm::vec3> _forces);

    void setMaxNum(int _maxNumParts){ maxNumParts = _maxNumParts;};
    void setMass(float _mass){ ptMass = _mass;};
    void setTime(float _time){ period = _time;};
    void initializeParticles();
    void draw();
    void update();
    ExtraForce ef;

};

#endif