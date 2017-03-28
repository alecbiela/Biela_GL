#pragma once

using namespace glm;

struct Vertex
{
	glm::vec3 loc;
	glm::vec2 uv;
	glm::vec3 normal;
};

struct VertInd
{
	unsigned int locInd;
	unsigned int normInd;
	unsigned int uvInd;
};

class Model
{
public:
	Model();
	~Model();
	bool BufferQuad();
	bool Buffer(std::string objFile);
	void Render();
	void InstanceModel(const glm::mat4* transformations, const GLuint amount);

private:
	unsigned int vertCount;
	GLuint vertArr;
	GLuint vertBuf;
	bool instanced;
	GLuint amountOfObjects;
};

