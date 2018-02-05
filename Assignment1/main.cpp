#define CHECK std::cout << "Check" << std::endl; 

// STL
#include <cmath>
#include <chrono>
#include <iostream>

// GL
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>

// Other classes
#include "ObjParser.h"

// Window
int g_width{1360};
int g_height{768};
int g_window{0};

// Camera
float g_theta{0.f};

// Triangle
static const GLfloat g_vertex_buffer_data[] = { 
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
};

std::string objPath = "../SampleOBJ/diamond.obj";
ObjParser *myParser;
GLuint VertexArrayID;
GLuint vertexbuffer;

////////////////////////////////////////////////////////////////////////////////
/// @brief Initialize GL settings
void initialize() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);

    // VAO
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Vertex Array Buffer
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

}

void draw(){

    // glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(10*std::sin(g_theta), 0.f, 10*std::cos(g_theta),
            0.f, 0.f, 0.f, 0.f, 1.f, 0.f);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glDisableVertexAttribArray(0);

    glColor3f(1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glutSwapBuffers();
}


int main(int argc, char* argv[]){
    std::cout << "Initializing GLUTWindow" << std::endl;
    // GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(50, 100);
    glutInitWindowSize(g_width, g_height); // HD size
    g_window = glutCreateWindow("MAVERINICK");
    
    glewExperimental = GL_TRUE; 
    glewInit();
    initialize();
    glutDisplayFunc(draw);
    // Start application
    std::cout << "Starting Application" << std::endl;
    glutMainLoop();

    glDeleteBuffers(1, &vertexbuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    return 0;
}
