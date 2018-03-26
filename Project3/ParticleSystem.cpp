#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(int _maxNumParts, float _mass, std::vector<glm::vec3> _forces){
    maxNumParts = _maxNumParts;
    ptMass = _mass;
    initializeParticles();
    forces = _forces;
}

void ParticleSystem::initializeParticles(){
    cTime = std::chrono::high_resolution_clock::now();
    
    for(int i=0; i<maxNumParts; i++){
        Particle p;
        // Raodomize color
        p.col.x = (float) rand() / (RAND_MAX);
        p.col.y = (float) rand() / (RAND_MAX);
        p.col.z = (float) rand() / (RAND_MAX);
        p.pos = glm::vec3(randomStart(), randomStart(), 0.f);
        p.vel = glm::vec3(0.f, 0.f, 0.f);
        parts.push_back(p);
    }
}

void ParticleSystem::getNewAcce(){
    glm::vec3 sumForce = glm::vec3(0.f, 0.f, 0.f);
    for(size_t i=0; i<forces.size(); i++){
        sumForce.x += forces[i].x;
        sumForce.y += forces[i].y;
        sumForce.z += forces[i].z;
    }

    acceleration.x = sumForce.x/ptMass;
    acceleration.y = sumForce.y/ptMass;
    acceleration.z = sumForce.z/ptMass;
}

void ParticleSystem::draw(){
    for(size_t i=0; i<parts.size(); i++){
        // glLoadIdentity();
        glPushMatrix();
        glColor3f(parts[i].col.x, parts[i].col.y, parts[i].col.z);
        glTranslatef(parts[i].pos.x, parts[i].pos.y, parts[i].pos.z);
        glutSolidSphere(1, 20, 20);
        glPopMatrix();
    }
}

void ParticleSystem::update(){
    std::chrono::high_resolution_clock::time_point newTime = std::chrono::high_resolution_clock::now();

    float period = std::chrono::duration_cast<std::chrono::duration<float>>(newTime - cTime).count();

    getNewAcce();
    for(size_t i=0; i<parts.size(); i++){
        // Update position
        parts[i].pos.x += parts[i].vel.x*period;
        parts[i].pos.y += parts[i].vel.y*period;
        parts[i].pos.z += parts[i].vel.z*period;
        // Update Velocity
        parts[i].vel.x += acceleration.x*period;
        parts[i].vel.y += acceleration.y*period;
        parts[i].vel.z += acceleration.z*period;
    }

}; 