#include "stdafx.h"
#include "ShaderManager.h"
#include <fstream>
#include <iostream>

using namespace std;

ShaderManager::ShaderManager()
{
}

//destructor clears program and shader from memory
ShaderManager::~ShaderManager()
{
	glDeleteProgram(prog);
	glDeleteProgram(iProg);
	glDeleteShader(shader);
}

//getter for the GL program
void ShaderManager::UseProgram(const int& type)
{
	switch (type)
	{
	case 0:	//non-instanced
		activeProgram = prog;
		glUseProgram(prog);
		break;
	case 1:	//instanced
		activeProgram = iProg;
		glUseProgram(iProg);
		break;
	}
}

GLuint ShaderManager::GetProgram() const
{
	return activeProgram;
}

//loads both shaders to be used.  Takes the file names of vertex shader and fragment shader
bool ShaderManager::LoadShaders(const char * vertexFile, const char * instancedVertexFile, const char * FragmentFile)
{
	//calls helper method to load shaders
	GLuint shad1 = LoadShader(vertexFile, GL_VERTEX_SHADER);
	if (shad1 == 0) return false;

	GLuint shad2 = LoadShader(FragmentFile, GL_FRAGMENT_SHADER);
	if (shad2 == 0) return false;

	GLuint shad3 = LoadShader(instancedVertexFile, GL_VERTEX_SHADER);
	if (shad3 == 0) return false;

	//create the program and attach the shaders
	prog = glCreateProgram();
	iProg = glCreateProgram();
	glAttachShader(prog, shad1);
	glAttachShader(prog, shad2);
	glAttachShader(iProg, shad3);
	glAttachShader(iProg, shad2);

	//finally, link the program
	glLinkProgram(prog);
	glLinkProgram(iProg);

	//check to see if the program linked correctly
	GLint success;
	glGetProgramiv(prog, GL_LINK_STATUS, &success);
	if (success != 0) {
		glUseProgram(prog);
		glGetProgramiv(iProg, GL_LINK_STATUS, &success);
		if(success != 0) return true;
		else {
			//if the shader didn't compile, get the info log
			GLint* len = new GLint();
			glGetProgramiv(iProg, GL_INFO_LOG_LENGTH, len);
			GLchar* log = new GLchar[*len];
			glGetProgramInfoLog(iProg, *len, 0, log);

			cout << log;
			delete len;
			delete[] log;
			return false;
		}
	}
	//if the shader didn't compile, get the info log
	GLint* len = new GLint();
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, len);
	GLchar* log = new GLchar[*len];
	glGetProgramInfoLog(prog, *len, 0, log);

	cout << log;

	//clearing up memory
	delete len;
	delete[] log;

	//return false
	return false;
}

void ShaderManager::Use(const int type)
{

}

//private helper method that loads in shaders from files
GLuint ShaderManager::LoadShader(const char * file, GLenum shaderType)
{
	//open the file
	ifstream fileIn(file, ios::binary);
	char* data;

	//reads the file in to data array
	if (fileIn.is_open())
	{
		fileIn.seekg(0, ios::end);
		int length = (unsigned int)fileIn.tellg();

		fileIn.seekg(0, ios::beg);

		data = new char[length + 1];

		fileIn.read(data, length);
		data[length] = 0;

		fileIn.close();
	}
	else {	//file did not open correctly
		cout << "Shader file did not open correctly!" << endl;
		return 0;
	}

	//creates and compiles the shader and deletes data
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &data, 0);
	glCompileShader(shader);
	delete[] data;

	//determine if the shader compiled
	GLint* didCompile = new GLint();
	glGetShaderiv(shader, GL_COMPILE_STATUS, didCompile);

	//if yes, return the shader
	if (*didCompile != 0)
	{
		delete didCompile;
		return shader;
	}

	//if the shader didn't compile, get the info log
	GLint* len = new GLint();
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, len);
	GLchar* log = new GLchar[*len];
	glGetShaderInfoLog(shader, *len, 0, log);
	cout << log;

	//clearing memory
	delete[] log;
	delete didCompile;
	delete len;

	//return false
	return 0;
}
