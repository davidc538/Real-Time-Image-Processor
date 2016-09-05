#ifndef GL_MISC_H
#define GL_MISC_H

#include "gl_misc.h"
#include <Windows.h>
#include <GL/glew.h>
#include <gl/GL.h>
#include <stdio.h>

void renderTextureOrthographic(GLuint texture, int textureWidth, int textureHeight)
{
	glViewport(0, 0, textureWidth, textureHeight);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, texture);

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);

	glTranslatef(0.0f, 0.0f,-1.0f);

	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glFlush();
	glDisable(GL_TEXTURE_2D);
}

void getFramebufferWithTexture2D(GLuint* framebuffer, GLuint* texture, int width, int height)
{
	glGenFramebuffers(1, framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer);

	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *texture, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void printGLSpecs()
{
	char* glVendor = (char*)glGetString(GL_VENDOR);
	char* glRenderer = (char*)glGetString(GL_RENDERER);
	char* glVersion = (char*)glGetString(GL_VERSION);
	char* glShadingLanguageVersion = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

	printf("Vendor: %s\r\n", glVendor);
	printf("Renderer: %s\r\n", glRenderer);
	printf("Version: %s\r\n", glVersion);
	printf("Shading Language Version: %s\r\n", glShadingLanguageVersion);
}

void checkError(GLuint error)
{
	switch (error)
	{
	case GL_NO_ERROR:
		break;
	case GL_INVALID_ENUM:
		printf("GL_ERROR: GL_INVALID_ENUM\r\n");
		break;
	case GL_INVALID_VALUE:
		printf("GL_ERROR: GL_INVALID_VALUE\r\n");
		break;
	case GL_INVALID_OPERATION:
		printf("GL_ERROR: GL_INVALID_OPERATION\r\n");
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		printf("GL_ERROR: GL_INVALID_FRAMEBUFFER_OPERATION\r\n");
		break;
	case GL_OUT_OF_MEMORY:
		printf("GL_ERROR: GL_OUT_OF_MEMORY\r\n");
		break;
	case GL_STACK_UNDERFLOW:
		printf("GL_ERROR: GL_STACK_UNDERFLOW\r\n");
		break;
	case GL_STACK_OVERFLOW:
		printf("GL_ERROR: GL_STACK_OVERFLOW\r\n");
		break;
	}
}

#endif // GL_MISC_H
