#include "rtip_functions.h"
#include <GL/glew.h>
#include <gl/GL.h>

void disableEffects(GLuint programID)
{
	GLuint instructionLocation;
	instructionLocation = glGetUniformLocation(programID, "instructions");
	glUniform1i(instructionLocation, 0);
}

void setWidthAndHeight(GLuint programID, int width, int height)
{
	GLuint widthLocation, heightLocation;
	widthLocation = glGetUniformLocation(programID, "width");
	heightLocation = glGetUniformLocation(programID, "height");

	glUniform1f(widthLocation, (float)width);
	glUniform1f(heightLocation, (float)height);
}
