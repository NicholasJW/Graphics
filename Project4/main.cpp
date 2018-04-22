// I received help from Tianchang Yang on this project

// STL
#include <cmath>
#include <chrono>
#include <iostream>
#include <string>

#include <GL/glut.h>
#include "ObjParser.h"
#include "ParticleSystem.h"
#include "FreeImage.h"

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
	cam.eyePos = glm::vec3(0.f, 0.f, -20.f); // x, y, z coordinates
	cam.lookAt = glm::vec3(20.0f/* looking radius */, 0/* angle to z */, (M_PI/2)/* angle to y */);
	cam.upVec = glm::vec3(1.0f/* fixed length up radius*/, 0.f/* fixed angle to lookAt */, 0.f/* angle to up */);
}

// ObjParsers
std::vector<object> objs;
std::vector<ObjParser> parsers;

// ParticleSystem
bool isPs = false;
ParticleSystem *ps;

// Textures
// texture width and height
GLint width, height;
GLuint tex1, tex2;

// Frame rate
const unsigned int FPS = 60;
float g_frameRate{0.f};
std::chrono::high_resolution_clock::time_point g_frameTime{std::chrono::high_resolution_clock::now()};
float g_delay{0.f};
float g_framesPerSecond{0.f};


// load image
unsigned char* imgBuffer;

static bool loadImage(const char* fname)
{
    FreeImage_Initialise(TRUE);

    // unknown format
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

    // get format
    fif = FreeImage_GetFileType(fname, 0);

    // read image
    FIBITMAP* bitmap = FreeImage_Load(fif, fname, 0);

    if (!bitmap)
    {
        std::cout << "Load Error!" << std::endl;
        return false;
    }

    int x, y;
    RGBQUAD m_rgb;

    // get width and height
    width = (GLint)FreeImage_GetWidth(bitmap);
    height = (GLint)FreeImage_GetHeight(bitmap);

    imgBuffer = new unsigned char[width * height * 4];

    // get RGB values
    for (y = 0; y < height; ++y)
    {
        for (x = 0; x < width; ++x)
        {
            // get pixel
            FreeImage_GetPixelColor(bitmap, x, y, &m_rgb);

            // save RGB value into image buffer
            imgBuffer[y * width * 4 + x * 4 + 0] = m_rgb.rgbRed;
            imgBuffer[y * width * 4 + x * 4 + 1] = m_rgb.rgbGreen;
            imgBuffer[y * width * 4 + x * 4 + 2] = m_rgb.rgbBlue;

            // get alpha value
            if (FreeImage_IsTransparent(bitmap))
                imgBuffer[y * width * 4 + x * 4 + 3] = m_rgb.rgbReserved;
            else
                imgBuffer[y * width * 4 + x * 4 + 3] = 255;
        }
    }
    // free memory
    FreeImage_Unload(bitmap);
    return true;
}


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

void drawSphere(float x, float y, float z){
	glPushMatrix();
	glTranslatef(x, y, z);
	glutSolidSphere(0.1f, 20, 20);
	glPopMatrix();
}

void applyLight(){
	
	glDisable(GL_LIGHTING);
	// glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
	// glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	// glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);
	glEnable(GL_LIGHT5);
	// glEnable(GL_LIGHT6);	
	glEnable(GL_LIGHT7);

	// Directional grey light
	// Facing negative-y axis
	GLfloat light0_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light0_diffuse[] = { 0.6, 0.6, 0.6, 1.0 };
	GLfloat light0_specular[] = { 0.6, 0.6, 0.6, 1.0 };
	GLfloat light0_position[] = { 0.0, 1.0, 0.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	
	// Spot light 1
	GLfloat light1_diffuse[] = {1.f, 1.f, 1.f, 1.f};
	GLfloat light1_specular[] = {1.f, 1.f, 1.f, 1.f};
	GLfloat spot1_direction[] = {-0.5f, -1.f, 0.f};
	GLfloat light1_position[] = {5.f, 10.f, 0.f, 1.0f};
	drawSphere(5, 10, 0);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.f);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot1_direction);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 1.0);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.5);

	// Spot light 2
	GLfloat light2_diffuse[] = {1.f, 1.f, 1.f, 1.f};
	GLfloat light2_specular[] = {1.f, 1.f, 1.f, 1.f};
	GLfloat spot2_direction[] = {0.5f, -1.f, 0.f};
	GLfloat light2_position[] = {-5.f, 10.f, 0.f, 1.0f};
	drawSphere(-5, 10, 0);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
	glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30.f);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot2_direction);
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 1.0);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.5);

	// Point Light
	GLfloat light3_diffuse[] = {1.f, 1.f, 1.f, 1.f};
	GLfloat light3_specular[] = {1.f, 1.f, 1.f, 1.f};
	GLfloat light3_position[] = {0.1, 3, 1.5, 1.0};
	// drawSphere(0.1, 3, 1.5);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light3_specular);
	glLightfv(GL_LIGHT3, GL_POSITION, light3_position);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light3_diffuse);
	glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 3.0);
	glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 1.0);
	glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.5);

	// Spot light 4
	GLfloat light4_diffuse[] = {1.0f, 1.f, 1.0f, 1.f};
	GLfloat light4_specular[] = {1.0f, 1.f, 1.0f, 1.f};
	GLfloat spot4_direction[] = {0.0f, 0.0f, -1.0f};
	GLfloat light4_position[] = {0.1f, 0.f, 30.f, 1.0f};
	drawSphere(0, 0, 30);
	glLightfv(GL_LIGHT4, GL_SPECULAR, light4_specular);
	glLightfv(GL_LIGHT4, GL_POSITION, light4_position);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, light4_diffuse);
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 15.f);
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, spot4_direction);
	// glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION, 2);
	// glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, 1.0);
	// glLightf(GL_LIGHT4, GL_QUADRATIC_ATTENUATION, 0.5);

	// Spot light 5
	GLfloat light5_diffuse[] = {1.f, 1.f, 0.f, 1.f};
	GLfloat light5_specular[] = {1.f, 1.f, 0.f, 1.f};
	GLfloat spot5_direction[] = {0.f, 5.f, 3.f};
	GLfloat light5_position[] = {0.1f, -5.f, -3.f, 1.0f};
	drawSphere(0.1, -5, -3);
	glLightfv(GL_LIGHT5, GL_SPECULAR, light5_specular);
	glLightfv(GL_LIGHT5, GL_POSITION, light5_position);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, light5_diffuse);
	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 60.f);
	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, spot5_direction);
	// glLightf(GL_LIGHT5, GL_CONSTANT_ATTENUATION, 3.0);
	// glLightf(GL_LIGHT5, GL_QUADRATIC_ATTENUATION, 1.5);
	// glLightf(GL_LIGHT5, GL_LINEAR_ATTENUATION, 3.0);

	GLfloat light6_diffuse[] = {0.2f, 0.2f, 0.2f, 1.f};
	glLightfv(GL_LIGHT6, GL_DIFFUSE, light6_diffuse);

	GLfloat light7_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light7_diffuse[] = { 0.6, 0.6, 0.6, 1.0 };
	GLfloat light7_specular[] = { 0.6, 0.6, 0.6, 1.0 };
	GLfloat light7_position[] = { 0.0, 0.0, -1.0, 0.0 };
	glLightfv(GL_LIGHT7, GL_AMBIENT, light7_ambient);
	glLightfv(GL_LIGHT7, GL_POSITION, light7_position);
	glLightfv(GL_LIGHT7, GL_DIFFUSE, light7_diffuse);
	glLightfv(GL_LIGHT7, GL_SPECULAR, light7_specular);
}
void applyColorLight(){
	glDisable(GL_LIGHTING);
	// glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
	// glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);
	glEnable(GL_LIGHT5);
	// glEnable(GL_LIGHT6);
	// glEnable(GL_LIGHT7);

	GLfloat light0_diffuse[] = {0.1f, 0.1f, 0.1f, 1.f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);

	// Purple Spot light
	GLfloat light1_diffuse[] = {1.f, 0.f, 1.f, 1.f};
	GLfloat light1_specular[] = {1.f, 0.f, 1.f, 1.f};
	GLfloat spot1_direction[] = {0.5f, -1.f, 0.f};
	GLfloat light1_position[] = {-3.f, 6.f, 0.f, 1.0f};
	drawSphere(-3, 6, 0);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60.f);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot1_direction);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 1.0);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.5);

	// Green Spot light
	GLfloat light2_diffuse[] = {0.f, 1.f, 0.f, 1.f};
	GLfloat light2_specular[] = {0.f, 1.f, 0.f, 1.f};
	GLfloat spot2_direction[] = {-0.5f, 1.f, 0.f};
	GLfloat light2_position[] = {3.f, -6.f, 0.f, 1.0f};
	drawSphere(3, -6, 0);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
	glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 60.f);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot2_direction);
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 1.0);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.5);

	// Point Light
	GLfloat light3_diffuse[] = {0.0f, 0.2f, 0.8f, 1.f};
	GLfloat light3_specular[] = {0.0f, 0.2f, 0.8f, 1.f};
	GLfloat light3_position[] = {3, 0, 3, 1.0};
	drawSphere(3, 0, 3);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light3_specular);
	glLightfv(GL_LIGHT3, GL_POSITION, light3_position);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light3_diffuse);
	glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 113.0);
	glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 1.0);
	glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.5);

	// Point Light
	GLfloat light4_diffuse[] = {0.0f, 0.2f, 0.8f, 1.f};
	GLfloat light4_specular[] = {0.0f, 0.2f, 0.8f, 1.f};
	GLfloat light4_position[] = {-3, 0, -3, 1.0};
	drawSphere(-3, 0, -3);
	glLightfv(GL_LIGHT4, GL_SPECULAR, light4_specular);
	glLightfv(GL_LIGHT4, GL_POSITION, light4_position);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, light4_diffuse);
	glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION, 113.0);
	glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, 1.0);
	glLightf(GL_LIGHT4, GL_QUADRATIC_ATTENUATION, 0.5);

	// Green Spot light
	GLfloat light5_diffuse[] = {0.f, 0.f, 1.f, 1.f};
	GLfloat light5_specular[] = {0.f, 0.f, 1.f, 1.f};
	GLfloat spot5_direction[] = {-1.0f, 0.f, 0.f};
	GLfloat light5_position[] = {16.f, 0.f, 0.f, 1.0f};
	drawSphere(16, 0, 0);
	glLightfv(GL_LIGHT5, GL_SPECULAR, light5_specular);
	glLightfv(GL_LIGHT5, GL_POSITION, light5_position);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, light5_diffuse);
	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 15.f);
	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, spot5_direction);
	glLightf(GL_LIGHT5, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT5, GL_LINEAR_ATTENUATION, 1.0);
	glLightf(GL_LIGHT5, GL_QUADRATIC_ATTENUATION, 0.5);
}

void drawAxies(){
	glColor3f(1.0,1.0,1.0);
	glLineWidth(5.0);
	
    glBegin(GL_LINES);
    // x aix
 
    glVertex3f(-4.0, 0.0f, 0.0f);
    glVertex3f(4.0, 0.0f, 0.0f);
 
    // arrow
    glVertex3f(4.0, 0.0f, 0.0f);
    glVertex3f(3.0, 1.0f, 0.0f);
 
    glVertex3f(4.0, 0.0f, 0.0f);
    glVertex3f(3.0, -1.0f, 0.0f);
    glEnd();
 
    // y 
    glBegin(GL_LINES);
    glVertex3f(0.0, -4.0f, 0.0f);
    glVertex3f(0.0, 4.0f, 0.0f);
 
    // arrow
    glVertex3f(0.0, 4.0f, 0.0f);
    glVertex3f(1.0, 3.0f, 0.0f);
 
    glVertex3f(0.0, 4.0f, 0.0f);
    glVertex3f(-1.0, 3.0f, 0.0f);
    glEnd();

    // z 
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0f ,-4.0f );
    glVertex3f(0.0, 0.0f ,4.0f );
 
    // arrow
    glVertex3f(0.0, 0.0f ,4.0f );
    glVertex3f(0.0, 1.0f ,3.0f );
 
    glVertex3f(0.0, 0.0f ,4.0f );
    glVertex3f(0.0, -1.0f ,3.0f );
    glEnd();

}

void draw(){
	using namespace std::chrono;
	// std::cout << "Start drawing" << std::endl;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// calculate camera vectors
	camVecs.lookVec = glm::vec3(cam.lookAt.x*std::sin(cam.lookAt.y)*std::sin(cam.lookAt.z), cam.lookAt.x*std::cos(cam.lookAt.z), cam.lookAt.x*std::cos(cam.lookAt.y)*std::sin(cam.lookAt.z));
	camVecs.rightVec = glm::normalize(glm::cross(camVecs.lookVec, glm::vec3(cam.upVec.x*std::sin(cam.upVec.z), cam.upVec.x*std::cos(cam.upVec.z), 0.f)));
	camVecs.upVec = glm::normalize(glm::cross(camVecs.rightVec, camVecs.lookVec));
	
	gluLookAt(cam.eyePos.x, cam.eyePos.y, cam.eyePos.z, camVecs.lookVec.x+cam.eyePos.x, camVecs.lookVec.y+cam.eyePos.y, camVecs.lookVec.z+cam.eyePos.z, camVecs.upVec.x, camVecs.upVec.y, camVecs.upVec.z);

	applyLight();
	// applyColorLight();
	// drawAxies();
	// drawSphere();
	// glutSolidSphere(3, 20, 20);
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
		parsers[i].applyMtl();
	    glEnable(GL_TEXTURE_2D);
		if(i==0)
	    	glBindTexture(GL_TEXTURE_2D, tex1);
		else
			glBindTexture(GL_TEXTURE_2D, tex2);
		parsers[i].drawObj();
		glBindTexture(GL_TEXTURE_2D, 0);
    	glDisable( GL_TEXTURE_2D );
		glPopMatrix();
	}

	// Particle System
	if(isPs){
		ps->update();
		ps->draw();
	}
		
	glutSwapBuffers();

	std::chrono::high_resolution_clock::time_point time = std::chrono::high_resolution_clock::now();
	g_frameRate = duration_cast<duration<float>>(time - g_frameTime).count();
	g_frameTime = time;
	g_framesPerSecond = 1.f/(g_delay + g_frameRate);
	// printf("FPS: %6.2f\n", g_framesPerSecond);
}

void keyPressed(GLubyte _key, GLint _x, GLint _y) {
	switch(_key){
    // Escape key : quit application
    case 27:
    	std::cout << "Destroying window: " << g_window << std::endl;
    	glutDestroyWindow(g_window);
    	g_window = 0;
		if(isPs)
			delete(ps);
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
	case 105:
		ps->ef.gravity -= 0.1;
		break;
	case 107:
		ps->ef.gravity += 0.1;
		break;
	case 106:
		ps->ef.hWind -= 0.1;
		break;
	case 108:
		ps->ef.hWind += 0.1;
		break;
	case 43:
		ps->ef.gSpeed += 0.01;
		break;
	case 95:
		ps->ef.gSpeed -= 0.01;
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
		}else if (line == "Particle:"){
			// cout << "Start parsing parti sys." << endl;
			isPs = true;
			ps = new ParticleSystem();
			int maxN;
			float mass;
			float timeStamp;
			while(getline(infile, line)){
				if (line[0] == '#')
					continue;
				if(line == "EndParticle;"){
					ps->setMaxNum(maxN);
					ps->setMass(mass);
					ps->setTime(timeStamp);
					ps->initializeParticles();
					break;
				}
				else if(line.substr(0,9) == "maxNum = "){
					maxN = std::atoi(line.substr(9).c_str());
				}else if(line.substr(0,7) == "mass = "){
					mass = std::atof(line.substr(7).c_str());
				}else if(line.substr(0,9) == "timeStamp"){
					timeStamp = std::atof(line.substr(12).c_str());
				}
				else if(line[0] == 'f'){
					// std::cout<< "forces" << std::endl;
					istringstream ss(line.substr(2));
					float x, y, z;
					ss >> x;
					ss >> y;
					ss >> z;
					ps -> forces.push_back(glm::vec3(x, y, z));
				}else if(line[0] == 'c'){
					// std::cout<< "forces" << std::endl;
					istringstream ss(line.substr(2));
					float x, y, z;
					ss >> x;
					ss >> y;
					ss >> z;
					ps -> useCol = true;
					ps -> color = glm::vec3(x, y, z);
				}else if(line[0] == 'g'){
					// std::cout<< "forces" << std::endl;
					istringstream ss(line.substr(2));
					float x, y, z, r;
					ss >> x;
					ss >> y;
					ss >> z;
					ss >> r;
					ps -> generators.push_back(glm::vec4(x, y, z, r));
				}else if(line[0] == 'a'){
					// std::cout<< "attractors" << std::endl;
					istringstream ss(line.substr(2));
					float x, y, z, r;
					ss >> x;
					ss >> y;
					ss >> z;
					ss >> r;
					// cout << r << endl;
					ps -> attractors.push_back(glm::vec4(x, y, z, r));
				}else if(line[0] == 'r'){
					// std::cout<< "repulsers" << std::endl;
					istringstream ss(line.substr(2));
					float x, y, z, r;
					ss >> x;
					ss >> y;
					ss >> z;
					ss >> r;
					ps -> repulsers.push_back(glm::vec4(x, y, z, r));
				}
			}
		}
	}
}

void genTexture() {
    // texture

	// Hard coding two textures for now
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &tex1);
    glBindTexture(GL_TEXTURE_2D, tex1);

    loadImage("/home/nicholasjw/Documents/Graphics/SampleOBJ/apple_textured_obj/textures/appleD.jpg");

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glGenerateMipmap(GL_TEXTURE_2D);  // mip map

    glBindTexture(GL_TEXTURE_2D, 0);


    glGenTextures(1, &tex2);
    glBindTexture(GL_TEXTURE_2D, tex2);

    loadImage("Key/key.bmp");

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glGenerateMipmap(GL_TEXTURE_2D);  // mip map

    glBindTexture(GL_TEXTURE_2D, 0);


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
	genTexture();
	glutReshapeFunc(resize);
	glutDisplayFunc(draw);
	glutKeyboardFunc(keyPressed);
	glutSpecialFunc(specialKeyPressed);
	glutTimerFunc(1000/FPS, timer, 0);

	std::cout << "Start rendering" << std::endl;
	// Go to main loop
	glutMainLoop();
}
