// STL
#include <cmath>
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

#include <GL/glut.h>
#include "ObjParser.h"

// Window
int g_width{1360};
int g_height{768};
int g_window{0};

// Booleans for displaystyle
bool isVert = false;
bool isLined = false;
bool isNormal = true;
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
std::chrono::high_resolution_clock::time_point g_frameTime{
  std::chrono::high_resolution_clock::now()};
float g_delay{0.f};
float g_framesPerSecond{0.f};

////////////////////////////////////////////////////////////////////////////////
// Functions

////////////////////////////////////////////////////////////////////////////////
/// @brief Initialize GL settings
void initialize() {
  glClearColor(0.0f, 0.0f, 0.0f, 0.f);
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
/// @brief Timer function to fix framerate in a GLUT application
/// @param _v Value (not used here)
///
/// Note, this is rudametary and fragile.
void timer(int _v) {
  if(g_window != 0) {
    glutPostRedisplay();

    g_delay = std::max(0.f, 1.f/FPS - g_frameRate);
    glutTimerFunc((unsigned int)(1000.f*g_delay), timer, 0);
  }
  else
    exit(0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Draw function for single frame
void draw() {
  using namespace std::chrono;

  //////////////////////////////////////////////////////////////////////////////
  // Clear
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //////////////////////////////////////////////////////////////////////////////
  // Draw

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
  gluLookAt(50*std::sin(g_theta), 0.f, 50*std::cos(g_theta),
            0.f, 0.f, 0.f, 0.f, 1.f, 0.f);

  for (unsigned int i = 0; i < vertices.size(); i++){
    vertices.at(i).x = vertices.at(i).x*zoomer;
    vertices.at(i).y = vertices.at(i).y*zoomer;
    vertices.at(i).z = vertices.at(i).z*zoomer;
  }

  // Model of cube
  if(isNormal){
    glColor3f(colorVec.x, colorVec.y, colorVec.z);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_TRIANGLES);
    for(unsigned int i = 0; i<faces.size(); i++){
      glNormal3f(normals.at(i).x, normals.at(i).y, normals.at(i).z);
      glm::vec3 p1 = vertices[faces.at(i).x - 1];
      glm::vec3 p2 = vertices[faces.at(i).y - 1];
      glm::vec3 p3 = vertices[faces.at(i).z - 1];
      glVertex3f(p1.x, p1.y, p1.z);
      glVertex3f(p2.x, p2.y, p2.z);
      glVertex3f(p3.x, p3.y, p3.z);
    }
    glEnd();
  }else if(isLined){
    glColor3f(colorVec.x, colorVec.y, colorVec.z);
    glLineWidth(lineWidth);
    glBegin(GL_LINE_LOOP);
      for(unsigned int i = 0; i<faces.size(); i++){
        // glNormal3f(normals.at(i).x, normals.at(i).y, normals.at(i).z);
        glm::vec3 p1 = vertices[faces.at(i).x - 1];
        glm::vec3 p2 = vertices[faces.at(i).y - 1];
        glm::vec3 p3 = vertices[faces.at(i).z - 1];
        glVertex3f(p1.x, p1.y, p1.z);
        glVertex3f(p2.x, p2.y, p2.z);
        glVertex3f(p3.x, p3.y, p3.z);
      }
    glEnd();
  }else{
    glColor3f(colorVec.x, colorVec.y, colorVec.z);
    glEnable(GL_PROGRAM_POINT_SIZE_EXT);
    glPointSize(pointSize);
    glBegin(GL_POINTS);
      for(unsigned int i = 0; i<vertices.size(); i++){
        glVertex3f(vertices.at(i).x, vertices.at(i).y, vertices.at(i).z);
      }
    glEnd();
  }
  //////////////////////////////////////////////////////////////////////////////
  // Show
  glutSwapBuffers();

  //////////////////////////////////////////////////////////////////////////////
  // Record frame time
  std::chrono::high_resolution_clock::time_point time = std::chrono::high_resolution_clock::now();
  g_frameRate = duration_cast<duration<float>>(time - g_frameTime).count();
  g_frameTime = time;
  g_framesPerSecond = 1.f/(g_delay + g_frameRate);
  // printf("FPS: %6.2f\n", g_framesPerSecond);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Callback function for keyboard presses
/// @param _key Key
/// @param _x X position of mouse
/// @param _y Y position of mouse
void keyPressed(GLubyte _key, GLint _x, GLint _y) {
  switch(_key) {
    // Escape key : quit application
    case 27:
      std::cout << "Destroying window: " << g_window << std::endl;
      glutDestroyWindow(g_window);
      g_window = 0;
      break;
    case 86:
      isVert = true;
      isLined = false;
      isNormal = false;
      break;
    case 76:
      isVert = false;
      isLined = true;
      isNormal = false;
      break;
    case 78:
      isVert = false;
      isLined = false;
      isNormal = true;
      break;
    case 118:
      isVert = true;
      isLined = false;
      isNormal = false;
      break;
    case 108:
      isVert = false;
      isLined = true;
      isNormal = false;
      break;
    case 110:
      isVert = false;
      isLined = false;
      isNormal = true;
      break;
    case 82:
      colorVec.x = 1.0f;
      colorVec.y = 0.0f;
      colorVec.z = 0.0f;
      break;
    case 71:
      colorVec.x = 0.0f;
      colorVec.y = 1.0f;
      colorVec.z = 0.0f;
      break;
    case 66:
      colorVec.x = 0.0f;
      colorVec.y = 0.0f;
      colorVec.z = 1.0f;
      break;
    case 87:
      colorVec.x = 1.0f;
      colorVec.y = 1.0f;
      colorVec.z = 1.0f;
      break;
    case 43:
      if(isLined){
        lineWidth += 0.1;
      }else if(isVert){
        pointSize += 0.1;
      }
      break;
    case 45:
      if(isLined){
        lineWidth -= 0.1;
      }else if(isVert){
        pointSize -= 0.1;
      }
      break;
    // Unhandled
    default:
      std::cout << "Unhandled key: " << (int)(_key) << std::endl;
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Callback function for keyboard presses of special keys
/// @param _key Key
/// @param _x X position of mouse
/// @param _y Y position of mouse
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

// Private debugging method
void printArray(std::string filePath){
  std::stringstream ss;
  ss << "Normal vectors: \n";
  for (unsigned int i = 0; i < normals.size(); i++){
    ss << normals.at(i).x << ' ' << normals.at(i).y << ' ' << normals.at(i).z << " \n";
  }
  ss << "Face indices: \n";
  for (unsigned int i = 0; i < faces.size(); i++){
    ss << faces.at(i).x << ' ' << faces.at(i).y << ' ' << faces.at(i).z << " \n";
  }
  ss << "Vertices: \n";
  for (unsigned int i = 0; i < vertices.size(); i++){
    ss << std::to_string(i+1) << ':' << vertices.at(i).x << ' ' << vertices.at(i).y << ' ' << vertices.at(i).z << " \n";
  }
  
  if (filePath == " "){
    std::cout << ss.str() << std::endl;
  }else{
    std::ofstream outFile;
    outFile.open(filePath);
    outFile << ss.str();
    outFile.close();
  }
  // ss.close();
}

////////////////////////////////////////////////////////////////////////////////
// Main

////////////////////////////////////////////////////////////////////////////////
/// @brief main
/// @param _argc Count of command line arguments
/// @param _argv Command line arguments
/// @return Application success status
int main(int argc, char* argv[]) {
  if(argc < 2){
		std::cerr << "PLease specify the OBJ file." << std::endl;
		exit(1);
	}

  std::cout << "Start Parsing" << std::endl;
  myParser = new ObjParser(argv[1]);
  vertices = myParser->getVertices();
  faces = myParser->getFaces();
  normals = myParser->getNormals();
  std::cout << "End Parsing" << std::endl;
  delete myParser;

  // Just for debugging
  std::string outputFile = "output.txt";
  printArray(outputFile);

  //////////////////////////////////////////////////////////////////////////////
  // Initialize GLUT Window
  std::cout << "Initializing GLUTWindow" << std::endl;
  // GLUT
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition(50, 100);
  glutInitWindowSize(g_width, g_height); // HD size
  g_window = glutCreateWindow("Maverinick");

  // GL
  initialize();

  //////////////////////////////////////////////////////////////////////////////
  // Assign callback functions
  std::cout << "Assigning Callback functions" << std::endl;
  glutReshapeFunc(resize);
  glutDisplayFunc(draw);
  glutKeyboardFunc(keyPressed);
  glutSpecialFunc(specialKeyPressed);
  glutTimerFunc(1000/FPS, timer, 0);

  // Start application
  std::cout << "Starting Application" << std::endl;
  glutMainLoop();

  return 0;
}

