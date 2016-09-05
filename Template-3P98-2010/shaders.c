#include <stdlib.h>
#include <Windows.h>
#include <GL/glew.h>
#include <stdio.h>
#include "shaders.h"
#include "file_handling.h"

GLuint loadShaders(char* vertexShader, char* fragmentShader)
{
	GLuint shaderProgram;
	GLuint vertexShaderProgram;
	GLuint fragmentShaderProgram;

	vertexShaderProgram = loadShader(vertexShader, GL_VERTEX_SHADER_ARB);
	fragmentShaderProgram = loadShader(fragmentShader, GL_FRAGMENT_SHADER_ARB);

	shaderProgram = glCreateProgramObjectARB();

	glAttachObjectARB(shaderProgram, vertexShaderProgram);
	glAttachObjectARB(shaderProgram, fragmentShaderProgram);

	glLinkProgramARB(shaderProgram);

	return shaderProgram;
}

GLuint loadShader(char* shader, GLenum type)
{
	char* source;
	char* error_log;

	int length;
	int status;

	int fileSize;
 
	GLenum shaderHandle;

	error_log = (char*)malloc(sizeof(char) * 5000);
 
	source = readFile(shader, &fileSize);
	
	shaderHandle = glCreateShaderObjectARB(type);
 
	glShaderSourceARB(shaderHandle, 1, &source, &fileSize);
	 
	glCompileShaderARB(shaderHandle);

	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE)
	{
		glGetInfoLogARB(shaderHandle, 5000, &length, error_log);
		
		writeToFile("shaders\\errors.log", "Compiling: ");
		writeToFile("shaders\\errors.log", shader);
		writeToFile("shaders\\errors.log", "\r\n");
		writeToFile("shaders\\errors.log", error_log);
	}
	else printf("Shader loaded and compiled\r\n");

	free(error_log);

	return shaderHandle;
}

void loadGLUniformMatrix(GLint location, float* memLocation, int length)
{
	int i;

	for (i = 0; i < length; ++i)
		glUniform1f(location++, memLocation[i]);
}
