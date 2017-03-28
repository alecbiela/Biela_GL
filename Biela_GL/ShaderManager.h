#pragma once
#include "stdafx.h"

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();
	void UseProgram(const int& type);
	GLuint GetProgram() const;
	bool LoadShaders(const char* vertexFile, const char* instancedVertexFile, const char* FragmentFile);
	void Use(const int type);


private:
	GLuint activeProgram;
	GLuint prog;
	GLuint iProg;
	GLuint shader;
	GLuint LoadShader(const char* file, GLenum shaderType);
};

