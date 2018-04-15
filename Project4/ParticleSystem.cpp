#include "ParticleSystem.h"

// Only for debugging use
ParticleSystem::ParticleSystem(int _maxNumParts, float _mass, std::vector<glm::vec3> _forces){
    maxNumParts = _maxNumParts;
    ptMass = _mass;
    initializeParticles();
    forces = _forces;
}

void ParticleSystem::initializeParticles(){
    // period = 0.01;

    for(int i=0; i<maxNumParts; i++){
        Particle p;
        // Raodomize color
        if(useCol){
            p.col.x = color.x;
            p.col.y = color.y;
            p.col.z = color.z;
        }else{
            p.col.x = (float) rand() / (RAND_MAX);
            p.col.y = (float) rand() / (RAND_MAX);
            p.col.z = (float) rand() / (RAND_MAX);
        }
        
        p.pos = glm::vec3(randomStart()-5, randomStart()-5, randomStart()-5);
        p.vel = glm::vec3(0.f, 0.f, 0.f);
        parts.push_back(p);
    }
    // getNewAcce();
}

void ParticleSystem::getNewAcce(Particle &p){
    glm::vec3 sumForce = glm::vec3(0.f, 0.f, 0.f);
    // Forces
    for(size_t i=0; i<forces.size(); i++){
        sumForce.x += forces[i].x;
        sumForce.y += forces[i].y;
        sumForce.z += forces[i].z;
    }
    // Attractors
    for(size_t i=0; i<attractors.size(); i++){
        // std::cout<< "Get attraction." << std::endl;
        glm::vec3 dirVec;
        dirVec.x = attractors[i].x - p.pos.x;
        dirVec.y = attractors[i].y - p.pos.y;
        dirVec.z = attractors[i].z - p.pos.z;
        float force = 0;
        force = (attractors[i].w)*(ptMass)/(dirVec.x*dirVec.x + dirVec.y*dirVec.y + dirVec.z*dirVec.z);
        sumForce.x += force*dirVec.x;
        sumForce.y += force*dirVec.y;
        sumForce.z += force*dirVec.z;
    }
    // std::cout<< "End attraction." << std::endl;
    // Repulsers
    for(size_t i=0; i<repulsers.size(); i++){
        glm::vec3 dirVec;
        dirVec.x = repulsers[i].x - p.pos.x;
        dirVec.y = repulsers[i].y - p.pos.y;
        dirVec.z = repulsers[i].z - p.pos.z;
        float force = 0;
        // std::cout << repulsers[i].w << std::endl;
        force = (repulsers[i].w)*(ptMass)/(dirVec.x*dirVec.x + dirVec.y*dirVec.y + dirVec.z*dirVec.z);
        sumForce.x -= force*dirVec.x;
        sumForce.y -= force*dirVec.y;
        sumForce.z -= force*dirVec.z;
    }

    sumForce.y -= ef.gravity;
    sumForce.x += ef.hWind;

    p.acc.x = sumForce.x/ptMass;
    p.acc.y = sumForce.y/ptMass;
    p.acc.z = sumForce.z/ptMass;
}

void ParticleSystem::draw(){
    for(size_t i=0; i<parts.size(); i++){
        // glLoadIdentity();
        glPushMatrix();
        glColor3f(parts[i].col.x, parts[i].col.y, parts[i].col.z);
        glTranslatef(parts[i].pos.x, parts[i].pos.y, parts[i].pos.z);
        glutSolidSphere(0.1, 20, 20);
        glPopMatrix();
    }
}

void ParticleSystem::update(){
    
    for(size_t i=0; i<parts.size(); i++){

        getNewAcce(parts[i]);
        // Update position
        parts[i].pos.x += parts[i].vel.x*period;
        parts[i].pos.y += parts[i].vel.y*period;
        parts[i].pos.z += parts[i].vel.z*period;
        if(!checkPos(parts[i])){
            Particle p;
            // Raodomize color
            if(useCol){
                p.col.x = color.x;
                p.col.y = color.y;
                p.col.z = color.z;
            }else{
                p.col.x = (float) rand() / (RAND_MAX);
                p.col.y = (float) rand() / (RAND_MAX);
                p.col.z = (float) rand() / (RAND_MAX);
            }
            // p.pos = glm::vec3(randomStart()-5, randomStart()-5, randomStart()-5);
            int index = (rand() % static_cast<int>(generators.size()));
            // std::cout << index << std::endl;
            p.pos = glm::vec3(generators[index].x, generators[index].y, generators[index].z);
            glm::vec3 goTo = glm::vec3(randomStart()-5, randomStart()-5, randomStart()-5);
            // generators[index].w += ef.gSpeed;
            p.vel.x = (generators[index].w+ef.gSpeed)*(goTo.x-p.pos.x);
            p.vel.y = (generators[index].w+ef.gSpeed)*(goTo.y-p.pos.y);
            p.vel.z = (generators[index].w+ef.gSpeed)*(goTo.z-p.pos.z);
            p.acc.x = 0.f;
            p.acc.y = 0.f;
            p.acc.z = 0.f;

            parts[i] = p;
            continue;
        }

        // Update Velocity
        parts[i].vel.x += parts[i].acc.x*period;
        parts[i].vel.y += parts[i].acc.y*period;
        parts[i].vel.z += parts[i].acc.z*period;
    }
}

// Check if particle is out of bound
bool ParticleSystem::checkPos(Particle p){
    if(p.pos.x<=5.0f && p.pos.x>=-5.0f)
        if(p.pos.y<=5.0f && p.pos.y>=-5.0f)
            if(p.pos.z<=5.0f && p.pos.z>=-5.0f)
                return true;
    
    return false;
}