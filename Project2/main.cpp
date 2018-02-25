// STL
#include <cmath>
#include <chrono>
#include <iostream>
#include <string>

#include <GL/glut.h>
#include "ObjParser.h"

int g_width{1360};
int g_height{768};
int g_window{0};

// Color Vec
glm::vec3 colorVec(1.0f, 1.0f, 1.0f);

// Zoomer
float zoomer = 1.0;

// Camera
float g_theta{0.f};

// ObjParser
std::vector<ObjParser> parsers;

// Frame rate
const unsigned int FPS = 60;
float g_frameRate{0.f};
std::chrono::high_resolution_clock::time_point g_frameTime{std::chrono::high_resolution_clock::now()};
float g_delay{0.f};
float g_framesPerSecond{0.f};

void initialize() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.f);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
}

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

void timer(int _v) {
	if(g_window != 0) {
		glutPostRedisplay();

    	g_delay = std::max(0.f, 1.f/FPS - g_frameRate);
    	glutTimerFunc((unsigned int)(1000.f*g_delay), timer, 0);
	}
  	else
    	exit(0);
}

void draw(){
	using namespace std::chrono;
	// std::cout << "Start drawing" << std::endl;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static GLfloat lightPosition[] = { 0.5f, 1.0f, 1.5f, 0.0f };
	static GLfloat whiteLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	static GLfloat darkLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, darkLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(30*std::sin(g_theta), 0.f, 30*std::cos(g_theta), 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);

	// World
	glPushMatrix();
	glTranslatef(-7.0f, 0.f, 0.f);
	// Spider
	glPushMatrix();
	glColor3f(1.f, 0.f, 0.f);
	glTranslatef(6.5f, -1.f, -2.5f);
	glScalef(0.25f, 0.25f, 0.25f);
	parsers[0].drawObj();
	glPopMatrix();
	// Drumset
	glPushMatrix();
	glTranslatef(-30.f, 0.f, 0.f);
	glColor3f(1.f, 1.f, 1.f);
	parsers[1].drawObj();
	glPopMatrix();
	glPopMatrix();


	glutSwapBuffers();

	std::chrono::high_resolution_clock::time_point time = std::chrono::high_resolution_clock::now();
	g_frameRate = duration_cast<duration<float>>(time - g_frameTime).count();
	g_frameTime = time;
	g_framesPerSecond = 1.f/(g_delay + g_frameRate);
}

void keyPressed(GLubyte _key, GLint _x, GLint _y) {
	switch(_key){
    // Escape key : quit application
    case 27:
    	std::cout << "Destroying window: " << g_window << std::endl;
    	glutDestroyWindow(g_window);
    	g_window = 0;
    	break;
    // Unhandled
    default:
    	std::cout << "Unhandled key: " << (int)(_key) << std::endl;
    	break;
	}
}

void specialKeyPressed(GLint _key, GLint _x, GLint _y) {
  	switch(_key) {
    	// Arrow keys
		case GLUT_KEY_LEFT:
			g_theta -= 0.02;
			break;
		case GLUT_KEY_RIGHT:
			g_theta += 0.02;
			break;
		case GLUT_KEY_UP:
			zoomer += 0.001;
			break;
		case GLUT_KEY_DOWN:
			zoomer -= 0.001;
			break;
		// Unhandled
		default:
			std::cout << "Unhandled special key: " << _key << std::endl;
			break;
  	}
}


int main(int argc, char* argv[]){
	if(argc < 2){
		std::cerr << "PLease specify the OBJ file." << std::endl;
		exit(1);
	}

	std::cout << "Start Parsing" << std::endl;
	for (int i = 1; i < argc; i++){
		parsers.push_back(ObjParser(argv[i]));
		// switch(i){
		// 	case 1:
		// 		parsers[i-1].setVertexMult(1.0f);
		// 		break;
		// 	case 2:
		// 		parsers[i-1].setVertexMult(1.0f);
		// 		break;
		// }
	}
	std::cout << "End Parsing" << std::endl;

	std::cout << "Initializing window" << std::endl;

	// GLut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(g_width, g_height); // HD size
	g_window = glutCreateWindow("Maverinick");

	// GL
	initialize();
	glutReshapeFunc(resize);
	glutDisplayFunc(draw);
	glutKeyboardFunc(keyPressed);
	glutSpecialFunc(specialKeyPressed);
	glutTimerFunc(1000/FPS, timer, 0);

	// for (size_t i = 0; i < parsers.size(); i++){
	// 	std::cout << parsers[i].getVertexMult() << std::endl;
	// }

	std::cout << "Start rendering" << std::endl;
	// Go to main loop
	glutMainLoop();
	// delete myParser;
}