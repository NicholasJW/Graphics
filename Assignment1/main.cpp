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

// Camera
float g_theta{0.f};

////////////////////////////////////////////////////////////////////////////////
/// @brief Initialize GL settings
void initialize() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // glClearDepth(1.0f);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Callback for resize of window
///
/// Responsible for setting window size (viewport) and projection matrix.
void resize(GLint _w, GLint _h) {
    g_width = _w;
    g_height = _h;

    // Viewport
    glViewport(0, 0, g_width, g_height);

    // Projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.f, GLfloat(g_width)/g_height, 0.01f, 100.f);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Callback for drawing
///
/// Responsible for drawing the object
void displayObj() {
    using namespace std::chrono;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Single directional light
    static GLfloat lightPosition[] = { 0.5f, 1.0f, 1.5f, 0.0f };
    static GLfloat whiteLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    static GLfloat darkLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, darkLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);

    // Camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(10*std::sin(g_theta), 0.f, 10*std::cos(g_theta), 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);

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
    glutReshapeFunc(resize);
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