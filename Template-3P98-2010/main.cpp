#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <Windows.h>
#include <GL/glew.h>
#include <freeglut.h>
#include <FreeImage.h>
#include <time.h>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "default_matrices.h"
#include "glext.h"
#include "shader_instructions.h"

extern "C"
{
#include "structures.h"
#include "shaders.h"
#include "gl_misc.h"
#include "file_handling.h"
#include "rtip_functions.h"
}

GLuint framebuffer[4];
GLuint texture[4];
GLuint shader;

int width, height;
int windowWidth, windowHeight;

std::string filename = "";
std::vector<ShaderInstruction*> instructions;

using namespace cv;
VideoCapture* cap;
Mat mt;

// provides the output in a framebuffer becase glReadPixels is too slow to read pixels back in real time
void process(GLuint outputFramebuffer, pixel* input, GLuint framebuffer1, GLuint framebuffer2, GLuint texture1, GLuint texture2,
	int width, int height, std::vector<ShaderInstruction*>* instructions)
{
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, input);
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned int i;
	
	for (i = 0; i < instructions->size(); i++)
	{
		(*instructions)[i]->setParams(shader, width, height);
		
		switch (i % 2)
		{
		case 0:
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);
			renderTextureOrthographic(texture1, width, height);
			break;
		case 1:
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer1);
			renderTextureOrthographic(texture2, width, height);
			break;
		}
	}

	disableEffects(shader);
	glBindFramebuffer(GL_FRAMEBUFFER, outputFramebuffer);

	switch (i % 2)
	{
	case 0:
		renderTextureOrthographic(texture1, width, height);
		break;
	case 1:
		renderTextureOrthographic(texture2, width, height);
		break;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void render(void)
{
	cap->read(mt);

	process(framebuffer[2], (pixel*)mt.ptr(), framebuffer[0], framebuffer[1], texture[0], texture[1], mt.cols, mt.rows, &instructions);
	//process(0, (pixel*)mt.ptr(), framebuffer[0], framebuffer[1], texture[0], texture[1], mt.cols, mt.rows, &instructions);

	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (pixel*)mt.ptr());
	glBindTexture(GL_TEXTURE_2D, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(0.0f, 2.0f, 0.0f, 1.0f, 0.0f, 1.0f);

	glTranslatef(0.0f, 0.0f,-1.0f);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	
	glViewport(0, 0, windowWidth, windowHeight);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glBindTexture(GL_TEXTURE_2D, texture[3]);

	glBegin(GL_QUADS);
	
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[2]);

	glBegin(GL_QUADS);
	
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(2.0f, 1.0f, 0.0f);
	
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(2.0f, 0.0f, 0.0f);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glFlush();
	glDisable(GL_TEXTURE_2D);
}

void keyboard(unsigned char key, int x, int y)
{
	if (shader != -1)
	{
		glutPostRedisplay();
	}
}

void reshape(int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	glViewport(0, 0, width, height);
}

void timer(int value)
{
	render();
	glutTimerFunc(10, timer, 0);
}

const int NORMAL = 1;
const int BLUR = 2;
const int BLURTWICE = 3;
const int SOBEL = 4;
const int BLURTWICEANDSOBEL = 5;
const int INVERTCOLORS = 6;
const int RESTARTVIDEO = 7;


void menuCallback(int item)
{
	instructions.clear();

	switch(item)
	{
	case NORMAL:
		break;
	case BLUR:
		instructions.push_back(new CustomShader(avg, 9.0f));
		break;
	case BLURTWICE:
		instructions.push_back(new CustomShader(avg, 9.0f));
		instructions.push_back(new CustomShader(avg, 9.0f));
		break;
	case SOBEL:
		instructions.push_back(new SobelShader());
		break;
	case BLURTWICEANDSOBEL:
		instructions.push_back(new CustomShader(avg, 9.0f));
		instructions.push_back(new CustomShader(avg, 9.0f));
		instructions.push_back(new SobelShader());
		break;
	case INVERTCOLORS:
		instructions.push_back(new InvertColorsShader());
		break;
	case RESTARTVIDEO:
		cap->release();
		delete cap;
		
		if (filename == "")
			cap = new VideoCapture(0);
		else
			cap = new VideoCapture(filename);
		break;
	}
}

int main(int argc, char** argv)
{
	if (argc > 1)
	{
		filename = std::string(argv[1]);
		cap = new VideoCapture(filename); // filename from argv
	}
	else
		cap = new VideoCapture(0); // attempt to open stream from webcam
	
	// allow time for the camera to initialize
	Sleep(1000);

	cap->read(mt);

	width = mt.cols;
	height = mt.rows;

	//instructions.push_back(new CustomShader(nothing, 1.0f));
	//instructions.push_back(new CustomShader(sharpen, 4.0f));
	instructions.push_back(new CustomShader(avg, 9.0f));
	instructions.push_back(new CustomShader(avg, 9.0f));
	//instructions.push_back(new CustomShader(edgeDetect, 1.0f));
	instructions.push_back(new SobelShader());
	//instructions.push_back(new SobelShader(1,2));
	//instructions.push_back(new SmartGreyScaleShader());
	//instructions.push_back(new EdgeAlphaShader(50));
	//instructions.push_back(new EdgeAlphaShader(5));
	//instructions.push_back(new CustomShader(avg, 9.0f));
	//instructions.push_back(new SobelShader(2, 1));
	//instructions.push_back(new InvertColorsShader());
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	
	glutInitWindowSize(width * 2, height);
	glutCreateWindow("Real Time Edge Detection");

	glewInit();

	glutKeyboardFunc(keyboard);
	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutTimerFunc(50, timer, 0);

	/*
	const int NORMAL = 1;
	const int BLUR = 2;
	const int BLURTWICE = 3;
	const int SOBEL = 4;
	const int BLURTWICEANDSOBEL = 5;
	const int INVERTCOLORS = 6;
	*/

	glutCreateMenu(menuCallback);
	glutAddMenuEntry("Normal", NORMAL);
	glutAddMenuEntry("Blur", BLUR);
	glutAddMenuEntry("Blur twice", BLURTWICE);
	glutAddMenuEntry("Sobel", SOBEL);
	glutAddMenuEntry("Blur twice and sobel", BLURTWICEANDSOBEL);
	glutAddMenuEntry("Invert Colors", INVERTCOLORS);

	if (argc > 1)
		glutAddMenuEntry("Restart video", RESTARTVIDEO);
	
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	
	getFramebufferWithTexture2D(&framebuffer[0], &texture[0], width, height);
	getFramebufferWithTexture2D(&framebuffer[1], &texture[1], width, height);
	getFramebufferWithTexture2D(&framebuffer[2], &texture[2], width, height);
	getFramebufferWithTexture2D(&framebuffer[3], &texture[3], width, height);

	printGLSpecs();

	shader = loadShaders("shaders\\edge_vert_sobel.shader", "shaders\\convolutions.shader");
	glUseProgramObjectARB(shader);

	glutMainLoop();

	cap->release();
	delete cap;

	return 0;
}
