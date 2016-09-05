#include "shader_instructions.h"
#include "default_matrices.h"

#include <gl/glew.h>
#include <gl/GL.h>

extern "C"
{
#include "shaders.h"
#include "gl_misc.h"
#include "rtip_functions.h"
}

// instructions will be used to tell the shader what to do
// instructions[0] = 0 will be nothing
// instructions[0] = 1 will be a 1 way convolution with instruction 1 being a divisor and 2 being ignored
// instructions[0] = 2 will be a 2 way sobel convolution with instructions 1 and 2 indicating directions
// instructions[0] = 3 will be a 4 way sobel convolution with instructions 1 and 2 ignored
// instructions[0] = 4 will be used for to invert the colors
// instructions[0] = 5 will be used for a greyscale effect
// 0 Down, 1 Right, 2 Up, 3 Left

EmptyShader::EmptyShader()
{
}

EmptyShader::~EmptyShader()
{
}

void EmptyShader::setParams(GLuint programID, int width, int height)
{
	GLuint instructionLocation = glGetUniformLocation(programID, "instructions");

	glUniform1i(instructionLocation, 0);
}

CustomShader::CustomShader(float* matrix, float divisor)
{
	this->matrix = matrix;
	this->divisor = divisor;
}

void CustomShader::setMatrix(float* matrix)
{
	this->matrix = matrix;
}

void CustomShader::setDivisor(float divisor)
{
	this->divisor = divisor;
}

void CustomShader::setParams(GLuint programID, int width, int height)
{
	setWidthAndHeight(programID, width, height);
	GLuint instructionLocation = glGetUniformLocation(programID, "instructions");
	GLuint matrixLocation = glGetUniformLocation(programID, "custom_matrix");
	loadGLUniformMatrix(matrixLocation, matrix, 9);

	glUniform1i(instructionLocation, 1);
	glUniform1i(instructionLocation + 1, divisor);
}

SobelShader::SobelShader()
{
	twoWay = false;
}

SobelShader::SobelShader(int way1, int way2)
{
	set2Way(way1, way2);
}

void SobelShader::set4Way()
{
	twoWay = false;
}

void SobelShader::set2Way(int way1, int way2)
{
	twoWay = true;
	this->way1 = way1;
	this->way2 = way2;
}

void SobelShader::setParams(GLuint programID, int width, int height)
{
	setWidthAndHeight(programID, width, height);
	GLuint instructionLocation = glGetUniformLocation(programID, "instructions");
	GLuint matrixLocation = glGetUniformLocation(programID, "custom_matrix");
	loadGLUniformMatrix(matrixLocation, sobel, 36);

	if (twoWay)
	{
		glUniform1i(instructionLocation, 2);
		glUniform1i(instructionLocation + 1, way1);
		glUniform1i(instructionLocation + 2, way2);
	}
	else
		glUniform1i(instructionLocation, 3);
}

InvertColorsShader::InvertColorsShader()
{
}

void InvertColorsShader::setParams(GLuint programID, int width, int height)
{
	setWidthAndHeight(programID, width, height);
	GLuint instructionLocation = glGetUniformLocation(programID, "instructions");
	
	glUniform1i(instructionLocation, 4);
}

GreyScaleShader::GreyScaleShader()
{
}

void GreyScaleShader::setParams(GLuint programID, int width, int height)
{
	setWidthAndHeight(programID, width, height);
	GLuint instructionLocation = glGetUniformLocation(programID, "instructions");
	
	glUniform1i(instructionLocation, 5);
}

SmartGreyScaleShader::SmartGreyScaleShader()
{
}

void SmartGreyScaleShader::setParams(GLuint programID, int width, int height)
{
	setWidthAndHeight(programID, width, height);
	GLuint instructionLocation = glGetUniformLocation(programID, "instructions");
	
	glUniform1i(instructionLocation, 6);
}

EdgeAlphaShader::EdgeAlphaShader(int threshhold)
{
	this->threshhold = threshhold;
}

void EdgeAlphaShader::setParams(GLuint programID, int width, int height)
{
	setWidthAndHeight(programID, width, height);
	GLuint instructionLocation = glGetUniformLocation(programID, "instructions");
	
	glUniform1i(instructionLocation, 7);
	glUniform1i(instructionLocation + 1, threshhold);
}

