#ifndef GL_MISC_H
#define GL_MISC_H

#include <Windows.h>
#include <GL/glew.h>
#include <gl/GL.h>

void renderTextureOrthographic(GLuint texture, int textureWidth, int textureHeight);

void getFramebufferWithTexture2D(GLuint* framebuffer, GLuint* texture, int width, int height);

void printGLSpecs();

void checkError(GLuint error);

#endif // GL_MISC_H
