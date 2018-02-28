// STL
#include <cmath>
#include <chrono>
#include <iostream>
#include <string>

#include <GL/glut.h>
#include "ObjParser.h"

// Winodw
int g_width{1360};
int g_height{768};
int g_window{0};

struct object{
	std::string filename;
	glm::vec3 DEFAULT_COLOR;
	glm::vec3 color;
	glm::vec3 trans;
	glm::vec3 scale;
	glm::vec4 rotat;
};

struct camera{
	glm::vec3 eyePos;
	glm::vec3 lookAt; // this is centered at the eyeposition
	glm::vec3 upVec;
} cam; // Pameterize by user

struct camVectors{
	glm::vec3 lookVec;
	glm::vec3 rightVec;
	glm::vec3 upVec;
} camVecs; // calculate by cam

// Integer indicate which object to present
int pre = -1;
int angle = 0;

void initializeCam(){
	cam.eyePos = glm::vec3(0.f, 2.f, 20.f); // x, y, z coordinates
	cam.lookAt = glm::vec3(20.0f/* looking radius */, M_PI/* angle to z */, (M_PI/2) + 0.2/* angle to y */);
	cam.upVec = glm::vec3(1.0f/* fixed length up radius*/, 0.f/* fixed angle to lookAt */, 0.f/* angle to up */);
}

// ObjParsers
std::vector<object> objs;
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
	initializeCam();
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

void randomCol(){
	for(size_t i = 0; i < objs.size(); i++){
		objs[i].color.x=(float) rand() / (RAND_MAX);
		objs[i].color.y=(float) rand() / (RAND_MAX);
		objs[i].color.z=(float) rand() / (RAND_MAX);
	}
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

	// calculate camera vectors
	camVecs.lookVec = glm::vec3(cam.lookAt.x*std::sin(cam.lookAt.y)*std::sin(cam.lookAt.z), cam.lookAt.x*std::cos(cam.lookAt.z), cam.lookAt.x*std::cos(cam.lookAt.y)*std::sin(cam.lookAt.z));
	camVecs.rightVec = glm::normalize(glm::cross(camVecs.lookVec, glm::vec3(cam.upVec.x*std::sin(cam.upVec.z), cam.upVec.x*std::cos(cam.upVec.z), 0.f)));
	camVecs.upVec = glm::normalize(glm::cross(camVecs.rightVec, camVecs.lookVec));
	
	gluLookAt(cam.eyePos.x, cam.eyePos.y, cam.eyePos.z, camVecs.lookVec.x+cam.eyePos.x, camVecs.lookVec.y+cam.eyePos.y, camVecs.lookVec.z+cam.eyePos.z, camVecs.upVec.x, camVecs.upVec.y, camVecs.upVec.z);

	// Drawing
	for (size_t i = 0; i < parsers.size(); i++){
		glColor3f(objs[i].color.x, objs[i].color.y, objs[i].color.z);
		glPushMatrix();
		glTranslatef(objs[i].trans.x, objs[i].trans.y, objs[i].trans.z);
		glRotatef(objs[i].rotat.x, objs[i].rotat.y, objs[i].rotat.z, objs[i].rotat.w);
		if (pre == static_cast<int>(i)){
			angle++;
			glRotatef((angle)%360, 0.0f, 1.0f, 0.0f);	
		}
		glScalef(objs[i].scale.x, objs[i].scale.y, objs[i].scale.z);
		parsers[i].drawObj();
		glPopMatrix();
	}

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
	case 119: // w
		cam.eyePos.x += 0.02*camVecs.lookVec.x;
		cam.eyePos.y += 0.02*camVecs.lookVec.y;
		cam.eyePos.z += 0.02*camVecs.lookVec.z;
		break;
	case 115: // s
		cam.eyePos.x -= 0.02*camVecs.lookVec.x;
		cam.eyePos.y -= 0.02*camVecs.lookVec.y;
		cam.eyePos.z -= 0.02*camVecs.lookVec.z;
		break;
	case 97: //a
		cam.eyePos.x += -0.2*camVecs.rightVec.x;
		cam.eyePos.y += -0.2*camVecs.rightVec.y;
		cam.eyePos.z += -0.2*camVecs.rightVec.z;
		break;
	case 100: //d
		cam.eyePos.x -= -0.2*camVecs.rightVec.x;
		cam.eyePos.y -= -0.2*camVecs.rightVec.y;
		cam.eyePos.z -= -0.2*camVecs.rightVec.z;
		break;
	case 113: //q
		cam.upVec.z += 0.02;
		break;
	case 101: //e
		cam.upVec.z -= 0.02;
		break;
	case 114: // r
		randomCol();
		break;
	case 98: // b
		for(size_t i = 0; i < objs.size(); i++){
			objs[i].color = objs[i].DEFAULT_COLOR;
		}
		break;
	case 49:
		pre = 0;
		angle = 0;
		break;
	case 50:
		pre = 1;
		angle = 0;
		break;
	case 51:
		pre = 2;
		angle = 0;
		break;
	case 48:
		pre = -1;
		angle = 0;
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
			cam.lookAt.y += 0.02;
			break;
		case GLUT_KEY_RIGHT:
			cam.lookAt.y -= 0.02;
			break;
		case GLUT_KEY_UP:
			cam.lookAt.z += 0.02;
			break;
		case GLUT_KEY_DOWN:
			cam.lookAt.z -= 0.02;
			break;
		// Unhandled
		default:
			std::cout << "Unhandled special key: " << _key << std::endl;
			break;
  	}
}

void parseConfig(std::string path){
	using namespace std;
	ifstream infile;
	infile.open(path.c_str());
	if(!infile.good()){
		throw invalid_argument("Failed to open config file.");
	}
    string line;
	object co;

	while(getline(infile, line)){
		if (line[0] == '#')
			continue;
		
		if (line.substr(0, 6) == "filep "){
			co.filename = line.substr(6);
		}else if (line.substr(0,6) == "color "){
			istringstream ss(line.substr(6));
			float x, y, z;
			ss >> x;
			ss >> y;
			ss >> z;
			co.color = glm::vec3(x,y,z);
			ss.str("");
		}else if (line.substr(0,6) == "trans "){
			istringstream ss(line.substr(6));
			float x, y, z;
			ss >> x;
			ss >> y;
			ss >> z;
			co.trans = glm::vec3(x,y,z);
			ss.str("");
		}else if (line.substr(0,6) == "scale "){
			istringstream ss(line.substr(6));
			float x, y, z;
			ss >> x;
			ss >> y;
			ss >> z;
			co.scale = glm::vec3(x,y,z);
			ss.str("");
		}else if (line.substr(0,6) == "rotat "){
			istringstream ss(line.substr(6));
			float x, y, z, w;
			ss >> x;
			ss >> y;
			ss >> z;
			ss >> w;
			co.rotat = glm::vec4(x,y,z,w);
			ss.str("");
			objs.push_back(co);
		}
		
	}
}

int main(int argc, char* argv[]){
	if(argc < 2||argc > 2){
		std::cerr << "PLease specify the config file." << std::endl;
		exit(1);
	}

	std::cout << "Start Parsing" << std::endl;
	parseConfig(argv[1]);
	// Set default color
	for(size_t i = 0; i < objs.size(); i++){
		objs[i].DEFAULT_COLOR = objs[i].color;
	}
	// Parse data
	for (size_t i = 0; i < objs.size(); i++){
		parsers.push_back(ObjParser(objs[i].filename));
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

	std::cout << "Start rendering" << std::endl;
	// Go to main loop
	glutMainLoop();
}