#ifndef SHADERS_H
#define SHADERS_H

#include <Windows.h>
#include <gl/GL.h>

// Will load, compile and link both a vertex and a fragment shader into a usable GPU program
GLuint loadShaders(char* vertexShader, char* fragmentShader);

// Will load and compile a single shader ready to be linked
GLuint loadShader(char* shader);

void loadGLUniformMatrix(GLint location, float* memLocation, int length);

#endif // SHADERS_H