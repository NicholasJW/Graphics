// STL
#include <cmath>
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

#include <GL/glut.h>
#include "ObjParser.h"

int g_width{1360};
int g_height{768};
int g_window{0};

enum DisplayStyle{vert, lined, normal} style;

// Color Vec
glm::vec3 colorVec(1.0f, 1.0f, 1.0f);

// Line width
GLfloat lineWidth = 1.0f;
GLfloat pointSize = 3.0f;
// Zoomer
float zoomer = 1.0;
// Camera
float g_theta{0.f};

// ObjParser
ObjParser *myParser;
// Object Data
std::vector< glm::vec3 > vertices;
std::vector< glm::vec3 > faces;
std::vector< glm::vec3 > normals;

// Frame rate
const unsigned int FPS = 60;
float g_frameRate{0.f};
std::chrono::high_resolution_clock::time_point g_frameTime{std::chrono::high_resolution_clock::now()};
float g_delay{0.f};
float g_framesPerSecond{0.f};

int main(int argc, char* argv[]){
	if(argc < 2){
		std::cerr << "PLease specify the OBJ file." << std::endl;
		exit(1);
	}

	std::cout << "Start Parsing" << std::endl;
	myParser = new ObjParser(argv[1]);
	// TODO change parsing
	vertices = myParser->getVertices();
	faces = myParser->getFaces();
	normals = myParser->getNormals();
	std::cout << "End Parsing" << std::endl;
	delete myParser;

	
}