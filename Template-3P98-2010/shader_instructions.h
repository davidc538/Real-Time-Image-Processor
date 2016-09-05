#ifndef SHADER_INSTRUCTIONS_H
#define SHADER_INSTRUCTIONS_H

#include <gl/glew.h>
#include <gl/GL.h>

class ShaderInstruction
{
public:
	virtual void setParams(GLuint programID, int width, int height) = 0;
	~ShaderInstruction(){};
};

class EmptyShader : public ShaderInstruction
{
public:
	EmptyShader();
	~EmptyShader();
	void setParams(GLuint programID, int width, int height);
};

class CustomShader : public ShaderInstruction
{
private:
	float* matrix;
	float divisor;
public:
	CustomShader(float* matrix, float divisor);
	~CustomShader(){}
	void setMatrix(float* matrix);
	void setDivisor(float divisor);
	void setParams(GLuint programID, int width, int height);
};

class SobelShader : public ShaderInstruction
{
private:
	bool twoWay;
	int way1, way2;
public:
	SobelShader();
	SobelShader(int way1, int way2);
	~SobelShader(){}
	void set4Way();
	void set2Way(int way1, int way2);
	void setParams(GLuint programID, int width, int height);
};

class InvertColorsShader : public ShaderInstruction
{
public:
	InvertColorsShader();
	~InvertColorsShader(){}
	void setParams(GLuint programID, int width, int height);
};

class GreyScaleShader : public ShaderInstruction
{
public:
	GreyScaleShader();
	~GreyScaleShader(){}
	void setParams(GLuint programID, int width, int height);
};

class SmartGreyScaleShader : public ShaderInstruction
{
public:
	SmartGreyScaleShader();
	~SmartGreyScaleShader(){}
	void setParams(GLuint programID, int width, int height);
};

class EdgeAlphaShader : public ShaderInstruction
{
private:
	int threshhold;
public:
	EdgeAlphaShader(int threshold);
	~EdgeAlphaShader(){}
	void setParams(GLuint programID, int width, int height);
};

#endif // SHADER_INSTRUCTIONS_H
