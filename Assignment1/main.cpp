// STL
#include <cmath>
#include <chrono>
#include <iostream>

#include <GL/glut.h>
#include <glm/glm.hpp>

// Other classes
#include "Model.h"
#include "ObjParser.h"
// #include "Display.h"

// Window
int g_width{1360};
int g_height{768};
int g_window{0};

////////////////////////////////////////////////////////////////////////////////
/// @brief Initialize GL settings
void initialize() {
    glClearColor(0.f, 0.f, 0.4f, 0.f);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
}

void displayObj() {
    using namespace std::chrono;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

int main(int argc, char* argv[]){
    // using namespace std;
    std::cout << "Checking arguments" << std::endl;
    if(argc < 2){
		  std::cerr << "PLease specify the OBJ file." << std::endl;
		  exit(1);
	  }
    std::string modelPath = argv[1];

    // Parsing
    std::cout << "Start parsing" << std::endl;
    ObjParser *myParser = new ObjParser(modelPath);
    std::cout << "End parsing" << std::endl;

    // Glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(g_width, g_height); // HD size
    g_window = glutCreateWindow("Maverinick: I still don't know what this is.");

    // GL
    initialize();

    //////////////////////////////////////////////////////////////////////////////
    // Assign callback functions
    std::cout << "Assigning Callback functions" << std::endl;
    glutDisplayFunc(displayObj);

    // Start Application
    std::cout << "Starting Application" << std::endl;
    glutMainLoop();

    delete myParser;
    return 0;
}

// #if   defined(OSX)
// #pragma clang diagnostic pop
// #endif